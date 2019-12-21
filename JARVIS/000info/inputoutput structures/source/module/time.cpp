////////////////////////////////////////////////////////////////////////////////
//File:   time.cpp
//Dir:    source/module/
//Date:   2014-03-29
//Author: Zachary Gill
//Implementation of TIME module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


//includes
#include "time.h"


//namespaces
using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;


//functions

string TIME::main()
{
  return "";
}


string TIME::time()
{
  if (settings_.CLOCK_MILITARY)
    return TIME::time24();

  ptime now = currentTime();
  string time = to_iso_extended_string(now).substr(11);
  time = TIME::hour() + time.substr(2);
  return time;
}

string TIME::time24()
{
  ptime now = currentTime();
  return to_iso_extended_string(now).substr(11);
}

string TIME::date()
{
  ptime now = currentTime();
  return to_iso_extended_string(now).substr(0, 10);
}


string TIME::year()
{
  return TIME::datestamp().substr(0, 4);
}

string TIME::month()
{
  return TIME::datestamp().substr(4, 2);
}

string TIME::day()
{
  return TIME::datestamp().substr(6, 2);
}

string TIME::hour()
{
  int hour = val(TIME::clockstamp().substr(0, 2));
  if (hour > 12)
    hour -= 12;
  if (hour == 0)
    hour = 12;
  return str(hour);
}

string TIME::hour24()
{
  return TIME::clockstamp().substr(0, 2);
}

string TIME::minute()
{
  return TIME::clockstamp().substr(2, 2);
}

string TIME::second()
{
  return TIME::clockstamp().substr(4, 2);
}

string TIME::millisecond()
{
  string clockstamp = TIME::clockstamp();
  if (clockstamp.length() == 6)
    return "0";
  else
    return TIME::clockstamp().substr(6);
}


string TIME::timestamp()
{
  return TIME::datestamp() + TIME::clockstamp();
}

string TIME::datestamp()
{
  ptime now = currentTime();
  return to_iso_string(now).substr(0, 8);
}

string TIME::clockstamp()
{
  ptime now = currentTime();
  return to_iso_string(now).substr(9);
}


string TIME::local()
{
  settings_.CLOCK_UNIVERSAL = false;
  return "now using local time";
}

string TIME::universal()
{
  settings_.CLOCK_UNIVERSAL = true;
  return "now using universal time";
}

string TIME::accuracyOn()
{
  settings_.CLOCK_ACCURACY = true;
  return "now using microsecond-accurate time";
}

string TIME::accuracyOff()
{
  settings_.CLOCK_ACCURACY = false;
  return "now using second-accurate time";
}



ptime currentTime()
{
  if (settings_.CLOCK_UNIVERSAL)
    if (settings_.CLOCK_ACCURACY)
      return microsec_clock::universal_time();
   else
     return second_clock::universal_time();
  else
    if (settings_.CLOCK_ACCURACY)
      return microsec_clock::local_time();
    else
      return second_clock::local_time();
}