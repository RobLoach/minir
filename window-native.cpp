#include "file.h"
#include "os.h"

#define MMAP_THRESHOLD 128*1024

//Not in the other window-* because GTK does not demand Linux, and vice versa.
#if 0
#error
#elif defined(FILEPATH_POSIX)
#include <unistd.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>

const char * window_get_proc_path()
{
	//we could lstat it, but apparently that just returns zero on /proc on Linux.
	
	ssize_t bufsize=64;
	static char * linkname=NULL;
	if (linkname) return linkname;
	
	while (true)
	{
		linkname=malloc(bufsize);
		ssize_t r=readlink("/proc/self/exe", linkname, bufsize);
		if (r<0 || r>=bufsize)
		{
			free(linkname);
			if (r<0) return NULL;
			
			bufsize*=2;
			continue;
		}
		linkname[r]='\0';
		char * end=strrchr(linkname, '/');
		if (end) *end='\0';
		
		return linkname;
	}
}

static void window_cwd_enter(const char * dir);
static void window_cwd_leave();

char * _window_native_get_absolute_path(const char * basepath, const char * path, bool allow_up)
{
	if (!basepath || !path) return NULL;
	const char * filepart=strrchr(basepath, '/');
	if (!filepart) return NULL;
	char * basedir=strndup(basepath, filepart+1-basepath);
	
	window_cwd_enter(basedir);
	char * ret=realpath(path, NULL);
	window_cwd_leave();
	
	if (!allow_up && ret && strncasecmp(basedir, ret, filepart+1-basepath)!=0)
	{
		free(ret);
		ret=NULL;
	}
	free(basedir);
	
	return ret;
}

static const char * cwd_init;
static const char * cwd_bogus;

static void window_cwd_enter(const char * dir)
{
	_int_mutex_lock(_imutex_cwd);
	char * cwd_bogus_check=getcwd(NULL, 0);
	if (strcmp(cwd_bogus, cwd_bogus_check)!=0) abort();//if this fires, someone changed the directory without us knowing - not allowed. cwd belongs to the frontend.
	free(cwd_bogus_check);
	chdir(dir);
}

static void window_cwd_leave()
{
	chdir(cwd_bogus);
	_int_mutex_unlock(_imutex_cwd);
}

const char * window_get_cwd()
{
	return cwd_init;
}

void _window_init_native()
{
	char * cwd_init_tmp=getcwd(NULL, 0);
	char * cwdend=strrchr(cwd_init_tmp, '/');
	if (!cwdend) cwd_init="/";
	else if (cwdend[1]=='/') cwd_init=cwd_init_tmp;
	else
	{
		size_t cwdlen=strlen(cwd_init_tmp);
		char * cwd_init_fixed=malloc(cwdlen+1+1);
		memcpy(cwd_init_fixed, cwd_init_tmp, cwdlen);
		cwd_init_fixed[cwdlen+0]='/';
		cwd_init_fixed[cwdlen+1]='\0';
		cwd_init=cwd_init_fixed;
		free(cwd_init_tmp);
	}
	
	//try a couple of useless directories and hope one of them works
	//this seems to be the best one:
	//- even root can't create files here
	//- it contains no files with a plausible name on a standard Ubuntu box (I have an ath9k-phy0, nothing will ever want that filename)
	//- a wild write will not do anything dangerous except turn on some lamps
	!chdir("/sys/class/leds/") ||
		//the rest are in case it's not accessible (weird chroot? not linux?), so try some random things
		!chdir("/sys/") ||
		!chdir("/dev/") ||
		!chdir("/home/") ||
		!chdir("/tmp/") ||
		!chdir("/");
	cwd_bogus=getcwd(NULL, 0);//POSIX does not specify getcwd(NULL), it's Linux-specific
}



static void* file_alloc(int fd, size_t len, bool writable)
{
	if (len <= MMAP_THRESHOLD)
	{
		uint8_t* data=malloc(len+1);
		pread(fd, data, len, 0);
		data[len]='\0';
		return data;
	}
	else
	{
		void* data=mmap(NULL, len+1, writable ? (PROT_READ|PROT_WRITE) : PROT_READ, MAP_SHARED, fd, 0);
		if (data==MAP_FAILED) return NULL;
		
		if (len % sysconf(_SC_PAGESIZE) == 0)
		{
			mmap((char*)data + len, 1, PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0);
		}
		return data;
	}
}

namespace {
	class file_fs_rd : public file {
	public:
		file_fs_rd(void* data, size_t len) : file(data, len) {}
		~file_fs_rd() { munmap((void*)this->data, this->len+1); }
	};
}

file* file::create_fs(const char * filename)
{
	int fd=open(filename, O_RDONLY);
	if (fd<0) return NULL;
	
	struct stat st;
	if (fstat(fd, &st)<0) goto fail;
	
	void* data; data=file_alloc(fd, st.st_size, false);
	close(fd);
	if (st.st_size <= MMAP_THRESHOLD) return new file::mem(data, st.st_size);
	else return new file_fs_rd(data, st.st_size);
	
fail:
	close(fd);
	return NULL;
}

namespace {
	class file_fs_wr : public filewrite {
	public:
		int fd;
		bool truncate;
		file_fs_wr(int fd) : fd(fd) {}
		
		/*private*/ void alloc(size_t size)
		{
			this->data=file_alloc(this->fd, size, true);
			this->len=size;
			if (this->data==NULL) abort();
		}
		
		/*private*/ void dealloc()
		{
			//no msync - munmap is guaranteed to do that already (and linux tracks dirty pages anyways)
			if (this->len <= MMAP_THRESHOLD)
			{
				pwrite(this->fd, this->data, this->len, 0);
				free(this->data);
			}
			else
			{
				munmap(this->data, this->len+1);
			}
		}
		
		bool resize(size_t newsize)
		{
			if (ftruncate(this->fd, newsize) < 0) return false;
			if (this->len < MMAP_THRESHOLD && newsize < MMAP_THRESHOLD)
			{
				this->len=newsize;
				uint8_t* data=realloc(this->data, newsize+1);
				data[newsize]='\0';
				this->data=data;
				return true;
			}
			dealloc();
			alloc(newsize);
			return true;
		}
		
		void sync()
		{
			if (this->truncate)
			{
				ftruncate(this->fd, this->len);
				this->truncate=false;
			}
			msync(this->data, this->len, MS_SYNC);//no MS_INVALIDATE because I can't figure out what it's supposed to do
			                                      //on linux, it does nothing whatsoever, except in some EINVAL handlers
		}
		
		~file_fs_wr()
		{
			sync();
			dealloc();
			close(this->fd);
		}
	};
};

filewrite* filewrite::create_fs(const char * filename, bool truncate)
{
	static const int oflags[]={ O_RDWR|O_CREAT, O_WRONLY|O_CREAT };
	int fd=open(filename, oflags[truncate], 0666);//umask defaults to turning this to 644
	if (fd<0) return NULL;
	
	if (truncate)
	{
		file_fs_wr* f=new file_fs_wr(fd);
		f->truncate=true;
		return f;
	}
	else
	{
		struct stat st;
		if (fstat(fd, &st)<0) goto fail;
		
		file_fs_wr* f; f=new file_fs_wr(fd);
		f->alloc(st.st_size);
		return f;
	}
	
fail:
	close(fd);
	return NULL;
}



#elif defined(FILEPATH_WINDOWS)
#undef bind
#include <windows.h>
#define bind bind_func
#include <string.h>

static void window_cwd_enter(const char * dir);
static void window_cwd_leave();

const char * window_get_proc_path()
{
	static char path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	for (int i=0;path[i];i++)
	{
		if (path[i]=='\\') path[i]='/';
	}
	char * end=strrchr(path, '/');
	if (end) end[1]='\0';
	return path;
}

char * _window_native_get_absolute_path(const char * basepath, const char * path, bool allow_up)
{
	if (!path || !basepath) return NULL;
	
	DWORD len=GetFullPathName(basepath, 0, NULL, NULL);
	char * matchdir=malloc(len);
	char * filepart;
	GetFullPathName(basepath, len, matchdir, &filepart);
	if (filepart) *filepart='\0';
	window_cwd_enter(matchdir);
	for (unsigned int i=0;matchdir[i];i++)
	{
		if (matchdir[i]=='\\') matchdir[i]='/';
	}
	
	len=GetFullPathName(path, 0, NULL, NULL);
	char * ret=malloc(len);
	GetFullPathName(path, len, ret, NULL);
	
	window_cwd_leave();
	
	for (unsigned int i=0;i<len;i++)
	{
		if (ret[i]=='\\') ret[i]='/';
	}
	
	if (!allow_up)
	{
		if (strncasecmp(matchdir, ret, strlen(matchdir))!=0)
		{
			free(matchdir);
			free(ret);
			return NULL;
		}
	}
	free(matchdir);
	
	return ret;
}

static char * cwd_init;
static char * cwd_bogus;
static char * cwd_bogus_check;
static DWORD cwd_bogus_check_len;

static void window_cwd_enter(const char * dir)
{
	_int_mutex_lock(_imutex_cwd);
	GetCurrentDirectory(cwd_bogus_check_len, cwd_bogus_check);
	if (strcmp(cwd_bogus, cwd_bogus_check)!=0) abort();//if this fires, someone changed the directory without us knowing - not allowed. cwd belongs to the frontend.
	SetCurrentDirectory(dir);
}

static void window_cwd_leave()
{
	SetCurrentDirectory(cwd_bogus);
	_int_mutex_unlock(_imutex_cwd);
}

const char * window_get_cwd()
{
	return cwd_init;
}

void _window_init_native()
{
	DWORD len=GetCurrentDirectory(0, NULL);
	cwd_init=malloc(len+1);
	GetCurrentDirectory(len, cwd_init);
	len=strlen(cwd_init);
	for (unsigned int i=0;i<len;i++)
	{
		if (cwd_init[i]=='\\') cwd_init[i]='/';
	}
	if (cwd_init[len-1]!='/')
	{
		cwd_init[len+0]='/';
		cwd_init[len+1]='\0';
	}
	
	//try a couple of useless directories and hope one of them works
	//(this code is downright Perl-like, but the alternative is a pile of ugly nesting)
	SetCurrentDirectory("\\Users") ||
	SetCurrentDirectory("\\Documents and Settings") ||
	SetCurrentDirectory("\\Windows") ||
	(SetCurrentDirectory("C:\\") && false) ||
	SetCurrentDirectory("\\Users") ||
	SetCurrentDirectory("\\Documents and Settings") ||
	SetCurrentDirectory("\\Windows") ||
	SetCurrentDirectory("\\");
	
	len=GetCurrentDirectory(0, NULL);
	cwd_bogus=malloc(len);
	cwd_bogus_check=malloc(len);
	cwd_bogus_check_len=len;
	GetCurrentDirectory(len, cwd_bogus);
}



//static void* file_alloc(int fd, size_t len, bool writable)
//{

//}
//static size_t pagesize()
//{
//	SYSTEM_INFO inf;
//	GetSystemInfo(&inf);
//	return inf.dwPageSize;//dwAllocationGranularity?
//}
//static size_t allocgran()
//{
//	SYSTEM_INFO inf;
//	GetSystemInfo(&inf);
//	return inf.dwAllocationGranularity;
//}

namespace {
	class file_fs_rd : public file::impl {
	public:
		HANDLE handle;
		file_fs_rd(HANDLE handle, size_t len) : file::impl(len), handle(handle) {}
		
		void read(size_t start, void* target, size_t len)
		{
			LARGE_INTEGER pos;
			pos.QuadPart=start;
			SetFilePointerEx(this->handle, &pos, NULL, FILE_BEGIN);
			DWORD ignore;
		more:
			ReadFile(this->handle, target, len, &ignore, NULL);
			if (len>0xFFFFFFFF)//you shouldn't be reading multiple gigabytes at all...
			{
				len-=0xFFFFFFFF;
				target+=0xFFFFFFFF;
				goto more;
			}
		}
		
		void* map(size_t start, size_t len)
		{
			HANDLE mem=CreateFileMapping(handle, NULL, PAGE_READONLY, 0, 0, NULL);
			void* ptr=MapViewOfFile(mem, FILE_MAP_READ, (SIZE_MAX>0xFFFFFFFF ? start>>32 : 0), start&0xFFFFFFFF, len);
			CloseHandle(mem);
			return ptr;
		}
		
		void unmap(const void* data, size_t len) { UnmapViewOfFile((void*)data); }
		~file_fs_rd() { CloseHandle(handle); }
	};
}

file::impl* file::create_fs(const char * filename)
{
	HANDLE file=CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return NULL;
	LARGE_INTEGER size;
	GetFileSizeEx(file, &size);
	return new file_fs_rd(file, size.QuadPart);
}

//namespace {
//	class file_fs_wr : public filewrite {
//	public:
//		int fd;
//		file_fs_wr(int fd) : fd(fd) {}
		
//		/*private*/ void alloc(size_t size)
//		{

//		}
		
//		/*private*/ void dealloc()
//		{

//		}
		
//		bool resize(size_t newsize)
//		{

//		}
		
//		void sync()
//		{

//		}
		
//		~file_fs_wr()
//		{
//			sync();
//			dealloc();
//			close(this->fd);
//		}
//	};
//};

//filewrite* filewrite::create_fs(const char * filename, bool truncate)
//{

//}
#else
	//Mac OS X: _NSGetExecutablePath() http://stackoverflow.com/questions/799679/#1024933
	//Mac OS X, some BSDs, Solaris: dlinfo https://gist.github.com/kode54/8534201
	//Linux: readlink /proc/self/exe
	//Solaris: getexecname() http://download.oracle.com/docs/cd/E19253-01/816-5168/6mbb3hrb1/index.html
	//FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1 http://stackoverflow.com/questions/799679
	//FreeBSD if it has procfs: readlink /proc/curproc/file (FreeBSD doesn't have procfs by default)
	//NetBSD: readlink /proc/curproc/exe
	//DragonFly BSD: readlink /proc/curproc/file
	//Windows: GetModuleFileName() with hModule = NULL http://msdn.microsoft.com/en-us/library/ms683197.aspx
#error Unsupported OS
#endif
