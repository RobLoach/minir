#include "io.h"
#ifdef VIDEO_DDRAW
#define video cvideo
#undef bind
#include <windows.h>
#define bind bind_func

//force some year-old C code to compile properly as C++ - I decided to switch long ago but still haven't finished.
#define this This

//http://msdn.microsoft.com/en-us/library/windows/desktop/gg426116%28v=vs.85%29.aspx
//we can not use Ex, because it is not present on a clean installation of XP.

struct video_ddraw {
	struct video i;
	
	
};

static void reinit(struct video * this_, unsigned int screen_width, unsigned int screen_height, videoformat depth, double fps)
{
	//struct video_ddraw * this=(struct video_ddraw*)this_;
	
	
}

static void draw(struct video * this_, unsigned int width, unsigned int height, const void * data, unsigned int pitch)
{
	//struct video_ddraw * this=(struct video_ddraw*)this_;
	
	
}

static bool set_sync(struct video * this_, bool sync)
{
	//struct video_ddraw * this=(struct video_ddraw*)this_;
	
	return false;
}

static bool has_sync(struct video * this_)
{
	return true;
}

static void free_(struct video * this_)
{
	struct video_ddraw * this=(struct video_ddraw*)this_;
	
	
	
	free(this);
}

static struct video * cvideo_create_ddraw(uintptr_t windowhandle, unsigned int screen_width, unsigned int screen_height,
                                   videoformat depth, double fps)
{
	struct video_ddraw * this=malloc(sizeof(struct video_ddraw));
	this->i.reinit=reinit;
	this->i.draw=draw;
	this->i.set_sync=set_sync;
	this->i.has_sync=has_sync;
	this->i.free=free_;
	
	if (true) goto cancel;
	
	return (struct video*)this;
	
cancel:
	free_((struct video*)this);
	return NULL;
}

#undef video
static video* video_create_ddraw(uintptr_t windowhandle)
{
	return video_create_compat(cvideo_create_ddraw(windowhandle, 256, 256, fmt_xrgb1555, 60));
}
const video::driver video::driver_ddraw = {"DirectDraw (unimplemented)", video_create_ddraw, NULL, video::f_vsync};
#endif
