////////////////////////////////////////////////////////////////////////////////
//File:   timer.cpp
//Dir:    source/resource/
//Date:   2014-10-16
//Author: Zachary Gill
//Timer functions Implementation
////////////////////////////////////////////////////////////////////////////////

#define  NTDDI_VERSION NTDDI_WIN7
#define  WINVER        _WIN32_WINNT_WIN7
#define _WIN32_WINNT   _WIN32_WINNT_WIN7
#define _WIN32_IE      _WIN32_IE_IE50

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


//standard includes
#include <ctime>


//includes
#include "timer.h"


//namespaces
using namespace std;


//Constructors

Timer::Timer()
{
  Timer::begin = -1;
  Timer::end = -1;
}


//Destructors

Timer::~Timer()
{
}


//Other Functions

void Timer::start()
{
  Timer::begin = clock();
  Timer::end = -1;
}

void Timer::stop()
{
  Timer::end = clock();
}

double Timer::time()
{
  if (Timer::end > -1)
    return double(Timer::end - Timer::begin) / CLOCKS_PER_SEC;
  else if (Timer::begin > -1)
    return double(clock() - Timer::begin) / CLOCKS_PER_SEC;
  return 0;
}