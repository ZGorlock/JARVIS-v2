////////////////////////////////////////////////////////////////////////////////
//File:   time.h
//Dir:    source/module/
//Date:   2014-03-19
//Author: Zachary Gill
//Interface of TIME module
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SM_TIME_H_
#define _JARVIS_SM_TIME_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>


//namespaces
using namespace std;


//namespace definitions
namespace TIME
{
  void main();

  string time();
  string date();
} ;


#endif