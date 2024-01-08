#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ftc.h"

#define HYDROGEN_STRING
#define HYDROGEN_TIME
#include "hydrogen/hydrogen.h"
#include "include/fulgrum.h"

typedef struct ThreadCom {
  pthread_mutex_t mt;
  void*           buf;
  void*           retval;
  pthread_t       id;
  int             initid;
  int             size;
  char            mode;
} ThreadCom;

#define THREAD_COUNT 2

ThreadCom tc[THREAD_COUNT];

void InitThreadComs (ThreadCom* tcv, int tcc) {
  int i;
  for (i = 0; i < tcc; ++i) {
    tcv[i]        = (ThreadCom){0};
    tcv[i].initid = i;
    pthread_mutex_init (&tcv[i].mt, NULL);
  }
}

void InitThreads (ThreadCom* tcv, int tcc, void* (*pfn) (ThreadCom*)) {
  int i;
  for (i = 0; i < tcc; ++i) {
    pthread_create (&tcv[i].id, NULL, (void* (*)(void*))pfn, tcv + i);
  }
}

void DestroyThreadComms (ThreadCom* tcv, int tcc) {
  int i;
  for (i = 0; i < tcc; ++i) {
    pthread_mutex_destroy (&tcv[i].mt);
    pthread_join (tcv[i].id, &tcv[i].retval);
  }
}

enum {
  FD_READ = 0,
  FD_WRITE,
};

enum {
  MODE_OUT = 0,
  MODE_IN,
};

void* myFun (FtcID id) {
  if (id == 2) {
    int         size = 0;
    char*       str  = NULL;
    h_timepoint tp   = timenow();
    while (FTCRead (id, (void**)&str, &size) == FTC_NODATA) {
      microsleep (1);
      if (timeduration (timenow(), tp, seconds_e) > 5.0)
        break;
    }
    do {
      if (str) {
        printf ("%s", str);
        free (str);
        waitms (1);
      } else
        printf ("No data???\n");
    } while (FTCRead (id, (void**)&str, &size) == FTC_MOREDATA);
  }
  waitms (8); /* work */
  printf ("IDK %li\n", id);
  waitms (1);
  FTCReturn (id, NULL);
}

int main (int argc, char** argv) {
  int i;
  printf ("%lu\n", sizeof (long));
  if (argc <= 1) {
    errorf ("Provide an argument... please.\n");
    return 1;
  }


  ful_import (argv[1]);
  printf ("Ok\n");

  FtcID idv[THREAD_COUNT];
  if (FTCCreate (idv, THREAD_COUNT, myFun) == FTC_ERROR) {
    printf ("Failed\n");
    return 1;
  }

  char str[] = "Hello number 2!\n";
  /*for (i = 0; i < 10; ++i) {
    FTCWrite (2, str, sizeof (str));
  }*/
  // waitms (20);


  for (i = 0; i < THREAD_COUNT; ++i) {
    FTCJoin (idv[i]);
  }
  printf ("ok\n");

  FTCCreate (idv, 2, myFun);
  wait (1.0);
  FTCWrite (2, str, sizeof (str));
  for (i = 0; i < 2; ++i) {
    FTCJoin (idv[i]);
  }
  printf ("ok2\n");


  /*InitThreadComs (tc, THREAD_COUNT);

  InitThreads (tc, THREAD_COUNT, myFun);

  waitms (10);

  DestroyThreadComms (tc, THREAD_COUNT);*/
  return 0;
}
