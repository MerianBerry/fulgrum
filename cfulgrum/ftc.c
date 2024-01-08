/*
  Fulgrum Thread Communicator
*/
#include "ftc.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FTC_THREAD_MAX 16

#if defined(__unix__)
#  define FTC_USE_POSIX 1
#  define FTC_USE_WIN32 0
#  include <pthread.h>
#  include <unistd.h>
#elif defined(_WIN32)
#  define FTC_USE_POSIX 0
#  define FTC_USE_WIN32 1
#  error "FTC Win32 support is pending"
#else
#  error "FTC does not support this platform"
#endif

typedef struct COM {
#if FTC_USE_POSIX
  pthread_mutex_t mt;
  pthread_t       pid;
#elif FTC_USE_WIN32

#endif
  void *retval;
  int   p[2];
  int   packCount;
  char  hasData;
  char  detached;
  char  joined;
} COM;

enum {
  P_READ = 0,
  P_WRITE,
};

#if FTC_USE_POSIX
static pthread_mutex_t mt;
#elif FTC_USE_WIN32

#endif
static char  ready = 0;
static COM   threads[FTC_THREAD_MAX];
static FtcID ids[FTC_THREAD_MAX];
static FtcID idc = 0;

FtcID checkFreeID() {
  int i;
  if (!idc)
    return 1;
  for (i = 0; i < FTC_THREAD_MAX; ++i) {
    if (ids[i] == 0) {
      return i + 1;
    }
  }
  return -1;
}

int FTCCreate (FtcID *idv, int tc, FtcPFN pfn) {
  int i;
  if (!idv || tc < 0 || !pfn)
    return FTC_BADARGUMENT;
  if (!ready) {
#if FTC_USE_POSIX
    pthread_mutex_init (&mt, NULL);
#elif FTC_USE_WIN32

#endif
  }
  int count = 0;

  for (i = 0; i < tc; ++i) {
#if FTC_USE_POSIX
    pthread_mutex_lock (&mt);
    COM c = {0};
    pthread_mutex_init (&c.mt, NULL);
    FtcID id = checkFreeID();
    if (id < 0) {
      pthread_mutex_destroy (&c.mt);
      pthread_mutex_unlock (&mt);
      return FTC_ERROR;
    }
    if (pipe (c.p) < 0) {
      pthread_mutex_destroy (&c.mt);
      pthread_mutex_unlock (&mt);
      return FTC_ERROR;
    }
    ids[id - 1] = id;
    ++idc;
    idv[i] = id;
    pthread_create (&c.pid, NULL, (void *(*)(void *))pfn, (void *)id);
    threads[id - 1] = c;
    pthread_mutex_unlock (&mt);
#elif FTC_USE_WIN32

#endif
    ++count;
  }
#if FTC_USE_POSIX

#elif FTC_USE_WIN32

#endif
  return FTC_OK;
}

void FTCEnd (FtcID id) {
#if FTC_USE_POSIX
  pthread_mutex_lock (&mt);
  if (threads[id - 1].detached) {
    ids[id - 1] = 0;
    --idc;
  } else {
    pthread_mutex_unlock (&mt);
    pthread_join (threads[id - 1].pid, &threads[id - 1].retval);
    pthread_mutex_lock (&mt);
    threads[id - 1].joined = 1;
  }
  close (threads[id - 1].p[P_READ]);
  close (threads[id - 1].p[P_WRITE]);
  pthread_mutex_unlock (&mt);
#elif FTC_USE_WIN32

#endif
}

void *FTCJoin (FtcID id) {
#if FTC_USE_POSIX
  pthread_mutex_lock (&mt);
  if (threads[id - 1].detached) {
    pthread_mutex_unlock (&mt);
    return NULL;
  }
  if (!threads[id - 1].joined) {
    pthread_mutex_unlock (&mt);
    pthread_join (threads[id - 1].pid, &threads[id - 1].retval);
    pthread_mutex_lock (&mt);
    threads[id - 1].joined = 1;
  }
  ids[id - 1] = 0;
  --idc;
  pthread_mutex_unlock (&mt);
  return threads[id - 1].retval;
#elif FTC_USE_WIN32

#endif
}

void FTCDetatch (FtcID id) {
#if FTC_USE_POSIX
  pthread_mutex_lock (&mt);
  threads[id - 1].detached = 1;
  pthread_detach (threads[id - 1].pid);
  pthread_mutex_unlock (&mt);
#elif FTC_USE_WIN32

#endif
}

int FTCWrite (FtcID id, void *buffer, int size) {
  if (!buffer || size < 0)
    return FTC_BADARGUMENT;
#if FTC_USE_POSIX
  pthread_mutex_lock (&mt);
  if (write (threads[id - 1].p[P_WRITE], &size, sizeof (size)) < 0) {
    pthread_mutex_unlock (&mt);
    return FTC_ERROR;
  }
  if (write (threads[id - 1].p[P_WRITE], buffer, size) < 0) {
    pthread_mutex_unlock (&mt);
    return FTC_ERROR;
  }
  ++threads[id - 1].packCount;
  threads[id - 1].hasData = 1;
  pthread_mutex_unlock (&mt);
  return FTC_OK;
#elif FTC_USE_WIN32

#endif
}

int FTCRead (FtcID id, void **buffer, int *pSize) {
  if (!buffer || !pSize)
    return FTC_BADARGUMENT;
#if FTC_USE_POSIX
  pthread_mutex_lock (&mt);
  if (!threads[id - 1].hasData) {
    pthread_mutex_unlock (&mt);
    return FTC_NODATA;
  }
  int bsize = 0;
  if (read (threads[id - 1].p[P_READ], &bsize, sizeof (bsize)) < 0) {
    pthread_mutex_unlock (&mt);
    return FTC_ERROR;
  }
  (*buffer) = malloc (bsize);
  if (read (threads[id - 1].p[P_READ], *buffer, bsize) < 0) {
    pthread_mutex_unlock (&mt);
    return FTC_ERROR;
  }
  if (pSize)
    (*pSize) = bsize;
  --threads[id - 1].packCount;
  threads[id - 1].hasData = 0;
  char stillHasData       = 0;
  if (threads[id - 1].packCount > 0) {
    stillHasData = (threads[id - 1].hasData = 1) ? 1 : 0;
  }

  pthread_mutex_unlock (&mt);
  return stillHasData;
#elif FTC_USE_WIN32

#endif
}