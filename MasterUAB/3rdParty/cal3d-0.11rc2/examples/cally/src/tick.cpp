//----------------------------------------------------------------------------//
// tick.cpp                                                                   //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <inttypes.h>
#endif

#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include "tick.h"

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Tick::Tick()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Tick::~Tick()
{
}

//----------------------------------------------------------------------------//
// Get the current millisecond tick count                                     //
//----------------------------------------------------------------------------//

unsigned int Tick::getTick()
{
#ifdef _WIN32
	return GetTickCount();
#else
  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}

#ifdef WIN32
double Tick::getTime()
{
  static double SCALE_COUNTER=-1;

  if (SCALE_COUNTER==-1.0) {
    LARGE_INTEGER pc;

    if(!QueryPerformanceFrequency((LARGE_INTEGER *)&pc))
	{
    //  g_log("No freqency !!!");
    }

    SCALE_COUNTER=1.0/((double)pc.QuadPart);

  }
  //unsigned int perf_cnt = pc.QuadPart;

  //double scale_time = 1.0 / perf_cnt;
  LARGE_INTEGER ct;
  double count;
  if(!QueryPerformanceCounter((LARGE_INTEGER *) &ct))
  {
    std::cerr << "profiler: timer not found\n";
    return 1;
  }
  count=(double)ct.QuadPart;
  count*=SCALE_COUNTER;
  return count;
}

#else 

#if defined(__i386__) || defined(__ia64__)

inline uint64_t TimerGetCycle() {
  uint64_t x;
  __asm__ volatile ( "RDTSC" : "=A" (x) ) ;
  return x;
}

inline uint64_t TimerGetHz()
{
  static struct timeval t1, t2;
  static struct timezone tz;
  uint64_t c1,c2;

  gettimeofday(&t1, &tz);
  c1 = TimerGetCycle();
  for (int i = 0; i < 2500000; i++);
  gettimeofday(&t2, &tz);
  c2 = TimerGetCycle();
  return (1000000 * (c2 - c1)) / ((t2.tv_usec - t1.tv_usec) + 1000000 * (t2.tv_sec - t1.tv_sec));
}


double Tick::getTime() 
{
  static double SCALE_COUNTER=-1;
  if (SCALE_COUNTER==-1.0)
    SCALE_COUNTER=1.0/(double)TimerGetHz() ;
  
  return SCALE_COUNTER*(double)TimerGetCycle() ;
}

#else

#error "no timer implemented for your plateform"
#endif
#endif

//----------------------------------------------------------------------------//
