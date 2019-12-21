////////////////////////////////////////////////////////////////////////////////
//File:   core.cpp
//Dir:    source/module/
//Date:   2014-03-19
//Author: Zachary Gill
//Implementation of CORE module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//includes
#include "core.h"


//global variable definitions
extern bool terminate_;


//functions

void CORE::main()
{
}

void CORE::terminate()
{
  terminate_ = true;
}