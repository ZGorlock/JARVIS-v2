////////////////////////////////////////////////////////////////////////////////
//File:   timer.h
//Dir:    source/resource/
//Date:   2014-12-30
//Author: Zachary Gill
//Timer functions Interface
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SR_TIMER_H_
#define _DLA_SR_TIMER_H_


//constants
#include "const.h"


//standard includes
#include <ctime>


//class definitions
class Timer
{
  public:
    Timer        ();
    ~Timer       ();

    void   start ();
    void   stop  ();
    double time  ();

    clock_t begin;
    clock_t end;
};


#endif