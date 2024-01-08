/*
  Fulgrum Thread Communicator
*/
#pragma once
#include <stdint.h>

typedef intptr_t FtcID;

typedef void *(*FtcPFN) (FtcID);

#define FTC_ERROR       -1
#define FTC_BADARGUMENT -2;
#define FTC_OK          0
#define FTC_NODATA      2
#define FTC_MOREDATA    1

#define FTCReturn(id, retval) \
  FTCEnd (id);                \
  return retval

int FTCCreate (FtcID *idv, int tc, FtcPFN pfn);

void FTCEnd (FtcID id);

void *FTCJoin (FtcID id);

void FTCDetatch (FtcID id);

int FTCWrite (FtcID id, void *buffer, int size);

int FTCRead (FtcID id, void **buffer, int *pSize);
