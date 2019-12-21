////////////////////////////////////////////////////////////////////////////////
//File:   time.h
//Dir:    source/module/
//Date:   2014-03-29
//Author: Zachary Gill
//Interface of TIME module
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SM_TIME_H_
#define _JARVIS_SM_TIME_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


//includes
#include "../header/settings.h"


//namespace definitions
namespace TIME
{
  std::string main();

  std::string time();
  std::string time24();
  std::string date();

  std::string year();
  std::string month();
  std::string day();
  std::string hour();
  std::string hour24();
  std::string minute();
  std::string second();
  std::string millisecond();

  std::string timestamp();
  std::string datestamp();
  std::string clockstamp();

  std::string local();
  std::string universal();
  std::string accuracyOn();
  std::string accuracyOff();
} ;


//global variable definitions
extern Settings settings_;


//function definitions
boost::posix_time::ptime currentTime();


#endif