////////////////////////////////////////////////////////////////////////////////
//File:   time_M.h
//Dir:    source/module/
//Date:   2015-01-23
//Author: Zachary Gill
//Interface of TIME module
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SM_TIME_H_
#define _JARVIS_SM_TIME_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <algorithm>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


//includes
#include "settings.h"


//namespace definitions
namespace TIME
{
  std::string main        ();

  std::string time        ();
  std::string time24      ();
  std::string date        ();

  std::string today       ();

  std::string timestamp   ();
  std::string datestamp   ();
  std::string clockstamp  ();

  std::string local       ();
  std::string universal   ();
  std::string accuracyOn  ();
  std::string accuracyOff ();

  namespace PRIVATE
  {
    void                     defaultSettings     ();
    void                     loadResources       ();

    boost::posix_time::ptime currentTime         ();

    boost::posix_time::ptime makeTime            (int, int, int);
    boost::posix_time::ptime makeTime            (int, int, int, int, int, int);
    boost::posix_time::ptime makeTime            (int, int, int, int, int, int, int);
    boost::posix_time::ptime makeTime            (std::string);
    boost::posix_time::ptime makeTime            (std::string, boost::posix_time::ptime);

    int                      daysInMonth         (int);
    int                      daysInMonth         (int, int);
    int                      daysInYear          (int);
    bool                     isLeapYear          (int);

    int                      default_year        (boost::posix_time::ptime, int, int);
    int                      default_year        (boost::posix_time::ptime, int, int, int);
    int                      default_year        (boost::posix_time::ptime, int, int, int, int, int, int);

    boost::posix_time::ptime justDate            (boost::posix_time::ptime);

    int                      second              (boost::posix_time::ptime);
    int                      minute              (boost::posix_time::ptime);
    int                      hour                (boost::posix_time::ptime);
    int                      day                 (boost::posix_time::ptime);
    int                      weekday             (boost::posix_time::ptime);
    int                      month               (boost::posix_time::ptime);
    int                      year                (boost::posix_time::ptime);

    int                      time_phrase_to_time (std::string);
    int                      weekday             (std::string);
    int                      month               (std::string);
    std::string              weekday_str         (int);
    std::string              month_str           (int);

    void                     time_from_tokens    (std::vector<std::string>&, int, int&, int&, int&);

    bool                     testAMPM            (std::string&, int&);

    boost::posix_time::ptime tm_to_ptime         (tm);

  } ;
} ;


//global variable definitions
extern Settings settings_;


#endif