#include "minir.h"
#ifdef THREAD_PTHREAD
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//list of synchronization points: http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap04.html#tag_04_10

struct threaddata_pthread {
	void(*startpos)(void* userdata);
	void* userdata;
};
static void * threadproc(void * userdata)
{
	struct threaddata_pthread * thdat=userdata;
	thdat->startpos(thdat->userdata);
	free(thdat);
	return NULL;
}

void thread_create(void(*startpos)(void* userdata), void* userdata)
{
	struct threaddata_pthread * thdat=malloc(sizeof(struct threaddata_pthread));
	thdat->startpos=startpos;
	thdat->userdata=userdata;
	pthread_t thread;
	if (pthread_create(&thread, NULL, threadproc, thdat)) abort();
	pthread_detach(thread);
}

unsigned int thread_ideal_count()
{
	//for more OSes: https://qt.gitorious.org/qt/qt/source/HEAD:src/corelib/thread/qthread_unix.cpp#L411, idealThreadCount()
	return sysconf(_SC_NPROCESSORS_ONLN);
}


struct mutex_pthread {
	struct mutex i;
	
	pthread_mutex_t lock;
};

static void mutex_lock(struct mutex * this_)
{
	struct mutex_pthread * this=(struct mutex_pthread*)this_;
	pthread_mutex_lock(&this->lock);
}

static bool mutex_try_lock(struct mutex * this_)
{
	struct mutex_pthread * this=(struct mutex_pthread*)this_;
	return (pthread_mutex_trylock(&this->lock)==0);
}

static void mutex_unlock(struct mutex * this_)
{
	struct mutex_pthread * this=(struct mutex_pthread*)this_;
	pthread_mutex_unlock(&this->lock); 
}

static void mutex_free_(struct mutex * this_)
{
	struct mutex_pthread * this=(struct mutex_pthread*)this_;
	pthread_mutex_destroy(&this->lock);
	free(this);
}

const struct mutex mutex_pthread_base = {
	mutex_lock, mutex_try_lock, mutex_unlock, mutex_free_
};
struct mutex * mutex_create()
{
	struct mutex_pthread * this=malloc(sizeof(struct mutex_pthread));
	memcpy(&this->i, &mutex_pthread_base, sizeof(struct mutex));
	
	pthread_mutex_init(&this->lock, NULL);
	
	return (struct mutex*)this;
}


struct event_pthread {
	struct event i;
	
	sem_t ev;
FIXME: handle the changed semantics
};

static void event_signal(struct event * this_)
{
	struct event_pthread * this=(struct event_pthread*)this_;
	int active;
	sem_getvalue(&this->ev, &active);
	if (active==0)
	{
		sem_post(&this->ev);
	}
}

static void event_wait(struct event * this_)
{
	struct event_pthread * this=(struct event_pthread*)this_;
	while (sem_wait(&this->ev)==EINTR) {} //the more plentiful one of user and implementation shall be
	                                      // simpler (minimizes the bug risk), so why does EINTR exist
}

static void event_free_(struct event * this_)
{
	struct event_pthread * this=(struct event_pthread*)this_;
	sem_destroy(&this->ev);
	free(this);
}

struct event * event_create()
{
	struct event_pthread * this=malloc(sizeof(struct event_pthread));
	this->i.signal=event_signal;
	this->i.wait=event_wait;
	this->i.free=event_free_;
	
	sem_init(&this->ev, 0, 0);
	
	return (struct event*)this;
}
#endif
