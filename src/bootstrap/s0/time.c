#include "include/common.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

timepoint_t timenow()
{
	#if defined(_WIN32)
	timepoint_t tp;
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	tp.freq = (double)(li.QuadPart)/1000.0;
	QueryPerformanceCounter(&li);
	tp.c = li.QuadPart;
	return tp;
	#elif defined(__GNUC__)
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	timepoint_t tp;
	tp.s = ts.tv_sec;
	tp.ns = ts.tv_nsec;
	return tp;
	#endif
}

void microsleep(long usec) {

	#ifdef __unix__
	usleep(usec);
	#elif defined(_WIN32)
	HANDLE timer; 
	LARGE_INTEGER ft; 

	ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL); 
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
	WaitForSingleObject(timer, INFINITE); 
	CloseHandle(timer); 
	#endif
}

double timeduration(timepoint_t end,
	timepoint_t start, double ratio)
{
	#if defined(_WIN32)
		double ndif = (double)(end.c-end.c)/end.freq;
		return ndif * ratio;
	#elif defined(__GNUC__)
	double t1 = (double)end.s + (double)end.ns / 1000000000.0;
	double t2 = (double)start.s + (double)start.ns / 1000000000.0;

	return (t1-t2) * ratio;
	#endif
}

void wait(double seconds) {
	#ifdef __unix__
  timepoint_t s = timenow();
  timespec_t ts, tsr = {2000, 0};
  while (timeduration(timenow(), s, seconds_e) < seconds)
	{
		nanosleep(&ts, &tsr);
	}
	#elif defined(_WIN32)
	__int64 us = (__int64)(seconds * 1000000.0);
	HANDLE timer;
	LARGE_INTEGER ft;
	ft.QuadPart = -(10*us);
	timer = CreateWaitableTimer(NULL, TRUE, NULL); 
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
	WaitForSingleObject(timer, INFINITE); 
	CloseHandle(timer); 
	#endif
	
}

void waitms(double ms) {
	#ifdef __unix__
  timepoint_t s = timenow();
  timespec_t ts = {2000, 0};
  ts.tv_sec = ms / 1000.0;
  ts.tv_nsec = fmodf(ms, 1000) * 1000000.0;
  while (nanosleep(&ts, &ts) == -1);
	#elif defined(_WIN32)
	__int64 us = (__int64)(ms * 1000.0);
	HANDLE timer;
	LARGE_INTEGER ft;
	ft.QuadPart = -(10*us);
	timer = CreateWaitableTimer(NULL, TRUE, NULL); 
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
	WaitForSingleObject(timer, INFINITE); 
	CloseHandle(timer); 
	#endif
}
