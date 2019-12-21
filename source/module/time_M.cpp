////////////////////////////////////////////////////////////////////////////////
//File:   time_M.cpp
//Dir:    source/module/
//Date:   2015-01-23
//Author: Zachary Gill
//Implementation of TIME module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <string>
#include <algorithm>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


//includes
#include "time_M.h"
#include "module.h"
#include "settings.h"


//namespaces
using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;


//functions

string TIME::main ()
{
  if (!settings_.hasSettingGroup(SETTING_MODULE, "TIME"))
    TIME::PRIVATE::defaultSettings();

  TIME::PRIVATE::loadResources();

  return "";
}


string TIME::time ()
{
  if (settings_.getSettingB(SETTING_MODULE, "TIME", "MILITARY"))
    return TIME::time24();
  ptime now = TIME::PRIVATE::currentTime();
  string time = "it is currently ";
  int h = TIME::PRIVATE::hour(now);
  bool ap = (h > 11);
  if (h > 12)
    h -= 12;
  if (h == 0)
    h = 12;
  time += str(h) + ":" + zeroset(str(TIME::PRIVATE::minute(now)), 2);
  if (settings_.getSettingB(SETTING_MODULE, "TIME", "SECONDS"))
    time += ":" + zeroset(str(TIME::PRIVATE::second(now)), 2);
  if (ap)
    time += " PM";
  else
    time += " AM";
  setOPriority("TIME", 0.4);
  return time;
}

string TIME::time24 ()
{
  ptime now = TIME::PRIVATE::currentTime();
  string time = "it is currently ";
  time += zeroset(str(TIME::PRIVATE::hour(now)), 2) + ":" + zeroset(str(TIME::PRIVATE::minute(now)), 2);
  if (settings_.getSettingB(SETTING_MODULE, "TIME", "SECONDS"))
    time += ":" + zeroset(str(TIME::PRIVATE::second(now)), 2);
  setOPriority("TIME", 0.4);
  return time;
}

string TIME::date ()
{
  ptime now = TIME::PRIVATE::currentTime();
  string date = "today is ";
  date += TIME::PRIVATE::month_str(TIME::PRIVATE::month(now)) + " " + str(TIME::PRIVATE::day(now)) + ", " + str(TIME::PRIVATE::year(now));
  setOPriority("TIME", 0.4);
  return date;
}

//string TIME::dateX ()
//{
//  return "";
//}

string TIME::today ()
{
  ptime now = TIME::PRIVATE::currentTime();
  string today = "today it is ";
  today += TIME::PRIVATE::weekday_str(TIME::PRIVATE::weekday(now));
  return today;
}

string TIME::timestamp ()
{
  return TIME::datestamp() + TIME::clockstamp();
}

string TIME::datestamp ()
{
  ptime now = TIME::PRIVATE::currentTime();
  return to_iso_string(now).substr(0, 8);
}

string TIME::clockstamp ()
{
  ptime now = TIME::PRIVATE::currentTime();
  return to_iso_string(now).substr(9);
}


string TIME::local ()
{
  settings_.getSettingGroup(SETTING_MODULE, "TIME").getSetting("UNIVERSAL").setValue(false);
  return "now using local time";
}

string TIME::universal ()
{
  settings_.getSettingGroup(SETTING_MODULE, "TIME").getSetting("UNIVERSAL").setValue(true);
  return "now using universal time";
}

string TIME::accuracyOn ()
{
  settings_.getSettingGroup(SETTING_MODULE, "TIME").getSetting("ACCURACY").setValue(true);
  return "now using microsecond-accurate time";
}

string TIME::accuracyOff ()
{
  settings_.getSettingGroup(SETTING_MODULE, "TIME").getSetting("ACCURACY").setValue(false);
  return "now using second-accurate time";
}




void TIME::PRIVATE::defaultSettings()
{
  SettingGroup sg;
  sg.setName("TIME");
  sg.setType(SETTING_MODULE);
  sg.setFile(userLoc_ + "data/setting/module/time.settings");
  sg.addSetting(Setting("UNIVERSAL", false));
  sg.addSetting(Setting("MILITARY", false));
  sg.addSetting(Setting("SECONDS", false));
  sg.addSetting(Setting("ACCURACY", false));
  settings_.addSettingGroup(sg);
  return;
}


void TIME::PRIVATE::loadResources()
{
  return;
}


ptime TIME::PRIVATE::currentTime ()
{
  if (settings_.getSettingB(SETTING_MODULE, "TIME", "UNIVERSAL"))
    if (settings_.getSettingB(SETTING_MODULE, "TIME", "ACCURACY"))
      return microsec_clock::universal_time();
   else
     return second_clock::universal_time();
  else
    if (settings_.getSettingB(SETTING_MODULE, "TIME", "ACCURACY"))
      return microsec_clock::local_time();
    else
      return second_clock::local_time();
}


ptime TIME::PRIVATE::makeTime (int year, int month, int day)
{
  return TIME::PRIVATE::makeTime(year, month, day, 0, 0, 0);
}
ptime TIME::PRIVATE::makeTime (int year, int month, int day, int hour, int minute, int second)
{
  return TIME::PRIVATE::makeTime(year, month, day, hour, minute, second, 0);
}
ptime TIME::PRIVATE::makeTime (int year, int month, int day, int hour, int minute, int second, int millisecond)
{
  ptime t (boost::gregorian::date(year, month, day), time_duration(hour, minute, second, millisecond));
  return t;
}
ptime TIME::PRIVATE::makeTime (string str)
{
  return TIME::PRIVATE::makeTime(str, currentTime());
}
ptime TIME::PRIVATE::makeTime (string str, ptime ref)
{
  string s = uCase(str);
  replace(s.begin(), s.end(), ':', ' ');
  replace(s.begin(), s.end(), '-', ' ');
  ReplaceStringInPlace(s, "AM", " AM");
  ReplaceStringInPlace(s, "PM", " PM");
  ReplaceStringInPlace(s, "  AM", " AM");
  ReplaceStringInPlace(s, "  PM", " PM");

  vector<string> tokens = tokenize(s);
  ptime t;
  ptime nadt (not_a_date_time);

  int when = 0;
  if (tokens[0] == "LAST")
    when = 1;
  else if (tokens[0] == "NEXT")
    when = 2;
  else if (tokens[0] == "THIS")
    when = 3;
  if (when > 0)
    tokens.erase(tokens.begin());


  vector<string> remwords;
  remwords.push_back("OF");
  remwords.push_back("THE");
  remwords.push_back("IS");
  remwords.push_back("IN");
  remwords.push_back("AT");

  size_t i = 0;
  bool remflag;
  while (i < tokens.size()) {
    remflag = false;
    for (size_t j = 0; j < remwords.size(); j ++)
      if (tokens[i] == remwords[j])
        remflag = true;
    if (remflag)
      tokens.erase(tokens.begin() + i);
    else
      i ++;
  }


  if (tokens.size() == 0)
    return nadt;


  if (tokens.size() == 1) {
    if (tokens[0] == "NOW") {
     t = ref;
      return t;
    }
    if (tokens[0] == "TODAY") {
      t = ref;
      t = TIME::PRIVATE::justDate(t);
      return t;
    }
    if (tokens[0] == "YESTERDAY") {
      t = ref;
      t -= days(1);
      t = TIME::PRIVATE::justDate(t);
      return t;
    }
    if (tokens[0] == "TOMORROW") {
      t = ref;
      t += days(1);
      t = TIME::PRIVATE::justDate(t);
      return t;
    }
  }


  {
    int h = time_phrase_to_time(tokens[0]);
    if (h > 0) {
      t = ref;
      if (TIME::PRIVATE::hour(t) >= h) {
        if (when == 2)
          t += days(1);
      }
      else {
        if (when == 1)
          t -= days(1);
      }
      t = TIME::PRIVATE::justDate(t);
      t += hours(h);
      return t;
    }
  }


  if (tokens[0] == "TODAY") {
    t = ref;
    t = TIME::PRIVATE::justDate(t);
    if (tokens.size() > 1) {
      int h = 0;
      int n = 0;
      int s = 0;
      h = TIME::PRIVATE::time_phrase_to_time(tokens[1]);
      if (h > 0) {
        t += hours(h);
      }
      else {
        TIME::PRIVATE::time_from_tokens(tokens, 1, h, n, s);
        t += hours(h);
        t += minutes(n);
        t += seconds(s);
      }
    }
    return t;
  }
  if (tokens[0] == "YESTERDAY") {
    t = ref;
    t -= days(1);
    t = TIME::PRIVATE::justDate(t);
    if (tokens.size() > 1) {
      int h = 0;
      int n = 0;
      int s = 0;
      h = TIME::PRIVATE::time_phrase_to_time(tokens[1]);
      if (h > 0) {
        t += hours(h);
      }
      else {
        TIME::PRIVATE::time_from_tokens(tokens, 1, h, n, s);
        t += hours(h);
        t += minutes(n);
        t += seconds(s);
      }
    }
    return t;
  }
  if (tokens[0] == "TOMORROW") {
    t = ref;
    t += days(1);
    t = TIME::PRIVATE::justDate(t);
    if (tokens.size() > 1) {
      int h = 0;
      int n = 0;
      int s = 0;
      h = TIME::PRIVATE::time_phrase_to_time(tokens[1]);
      if (h > 0) {
        t += hours(h);
      }
      else {
        TIME::PRIVATE::time_from_tokens(tokens, 1, h, n, s);
        t += hours(h);
        t += minutes(n);
        t += seconds(s);
      }
    }
    return t;
  }


  {
    int wd = TIME::PRIVATE::weekday(tokens[0]);
    if (wd > -1) {
      t = ref;
      t = TIME::PRIVATE::justDate(t);
      if (tokens.size() > 1) {
        if (tokens.size() > 2 && (tokens[1] == "LAST" || tokens[1] == "THIS" || tokens[1] == "NEXT")) {
          if (tokens[2] == "WEEK") {
            if (tokens[1] == "LAST") {
              while (TIME::PRIVATE::weekday(t) != 1)
                  t -= days(1);
              do {
                t -= days(1);
              } while (TIME::PRIVATE::weekday(t) != wd);
              return t;
            }
            else if (tokens[1] == "THIS") {
              while (TIME::PRIVATE::weekday(t) != 0)
                  t -= days(1);
              do {
                t += days(1);
              } while (TIME::PRIVATE::weekday(t) != wd);
              return t;
            }
            else if (tokens[1] == "NEXT") {
              while (TIME::PRIVATE::weekday(t) != 0)
                  t += days(1);
              do {
                t += days(1);
              } while (TIME::PRIVATE::weekday(t) != wd);
              return t;
            }
          }
        }
      }
      switch (when) {
        case 1 :
          if (tokens.size() > 1) {
            int y = 0;
            int m = 0;
            int h = 0;
            int n = 0;
            int s = 0;
            m = TIME::PRIVATE::month(tokens[1]);
            if (m > 0) {
              if (tokens.size() > 2) {
                y = val(tokens[2]);
                if (y < 1601)
                  y = 0;
                if (y == 0)
                  TIME::PRIVATE::time_from_tokens(tokens, 2, h, n, s);
                else
                  TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
              }
            }
            if (y == 0)
              y = TIME::PRIVATE::default_year(t, when, m);
            t = TIME::PRIVATE::makeTime(y, m, TIME::PRIVATE::daysInMonth(m, y), h, n, s);
            while (TIME::PRIVATE::weekday(t) != wd)
              t -= days(1);
            return t;
          }
          do {
            t -= days(1);
          } while (TIME::PRIVATE::weekday(t) != wd);
          break;
        case 2 :
          do {
            t += days(1);
          } while (TIME::PRIVATE::weekday(t) != wd);
          break;
        case 3 :
          while (TIME::PRIVATE::weekday(t) != wd)
            t += days(1);
          break;
        default :
          do {
            t -= days(1);
          } while (TIME::PRIVATE::weekday(t) != wd);
      }
      if (tokens.size() > 1) {
        int h = 0;
        int n = 0;
        int s = 0;
        h = TIME::PRIVATE::time_phrase_to_time(tokens[1]);
        if (h > 0) {
          t += hours(h);
        }
        else {
          TIME::PRIVATE::time_from_tokens(tokens, 1, h, n, s);
          t += hours(h);
          t += minutes(n);
          t += seconds(s);
        }
      }
      return t;
    }
  }

  if (tokens[0] == "DAY" && when == 1) {
    if (tokens.size() > 1) {
      t = ref;
      int y = 0;
      int m = TIME::PRIVATE::month(tokens[1]);
      if (m > 0) {
        if (tokens.size() > 2) {
          y = val(tokens[2]);
          if (y < 1601)
            y = 0;
        }
        if (y == 0)
          y = TIME::PRIVATE::year(t);
        t = TIME::PRIVATE::makeTime(y, m, TIME::PRIVATE::daysInMonth(m, y));
        return t;
      }
    }
  }

  if (tokens[0] == "WEEK" && when > 0) {
    t = ref;
    t = TIME::PRIVATE::justDate(t);
    do {
      if (when == 2)
        t += days(1);
      else if (when == 1 || when == 3)
        t -= days(1);
    } while (TIME::PRIVATE::weekday(t) != 1);
    return t;
  }
  if (tokens[0] == "WEEKEND") {
    t = ref;
    t = TIME::PRIVATE::justDate(t);
    do {
      if (when == 2 || when == 3)
        t += days(1);
      else
        t -= days(1);
    } while (TIME::PRIVATE::weekday(t) != 6);
    return t;
  }
  if (tokens.size() > 1) {
    if (tokens[0] == "WEEK" && tokens[1] == "END") {
      t = ref;
      t = TIME::PRIVATE::justDate(t);
      do {
        if (when == 2 || when == 3)
          t += days(1);
        else
          t -= days(1);
      } while (TIME::PRIVATE::weekday(t) != 6);
      return t;
    }
  }


  {
    int m = TIME::PRIVATE::month(tokens[0]);
    if (m > 0) {
      t = ref;
      t = TIME::PRIVATE::justDate(t);
      int y = 0;
      int d = 0;
      int h = 0;
      int n = 0;
      int s = 0;
      if (tokens.size() > 1) {
        if (tokens.size() > 2 || (tokens[1] == "LAST" || tokens[1] == "THIS" || tokens[1] == "NEXT")) {
          if (tokens[2] == "YEAR") {
            y = TIME::PRIVATE::year(t);
            if (tokens[1] == "LAST")
              y --;
            else if (tokens[1] == "NEXT")
              y ++;
            t = TIME::PRIVATE::makeTime(y, m, 1);
            return t;
          }
        }
        d = val(tokens[1]);
        if (d == 0)
          d = num_phrase_to_num(tokens[1]);
      }
      if (d == 0)
        d = 1;
      if (d > TIME::PRIVATE::daysInMonth(m)) {
        if (d > 1600)
          y = d;
        else
          y = 0;
        d = 1;
      }
      if (y == 0) {
        if (tokens.size() > 2) {
          if (tokens.size() > 3 || (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
            if (tokens[3] == "YEAR") {
              y = TIME::PRIVATE::year(t);
              if (tokens[2] == "LAST")
                y --;
              else if (tokens[2] == "NEXT")
                y ++;
              t = TIME::PRIVATE::makeTime(y, m, d);
              return t;
            }
          }
          h = TIME::PRIVATE::time_phrase_to_time(tokens[2]);
          if (h == 0) {
            y = val(tokens[2]);
            if (y < 1601)
              y = 0;
            if (y == 0)
              TIME::PRIVATE::time_from_tokens(tokens, 2, h, n, s);
            else
              TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
          }
        }        
        if (y == 0)
          y = TIME::PRIVATE::default_year(t, when, m, d);
      }
      t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
      return t;
    }
  }

  if (tokens[0] == "MONTH" && when > 0) {
    t = ref;
    t = TIME::PRIVATE::justDate(t);
    int y = 0;
    switch (when) {
      case 1 : 
        if (tokens.size() > 1) {
          y = val(tokens[1]);
          if (y < 1601)
            y = 0;
        }
        if (y == 0) {
          if (TIME::PRIVATE::month(t) == 1)
            t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t) - 1, 12, 1);
          else
            t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), TIME::PRIVATE::month(t) - 1, 1);
        }
        else {
          t = TIME::PRIVATE::makeTime(y, 12, 1);
        }
        break;
      case 2 : 
        if (TIME::PRIVATE::month(t) == 12)
          t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t) + 1, 1, 1);
        else
          t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), TIME::PRIVATE::month(t) + 1, 1);
        break;
      case 3 : 
        t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), TIME::PRIVATE::month(t), 1);
        break;
    }
    return t;
  }


  {
    int i = num_phrase_to_num(tokens[0]);
    if (i == -1)
      i = val(tokens[0]);
    if (i > -1) {
      t = ref;
      int y = 0;
      int m = 0;
      int d = 0;
      int h = 0;
      int n = 0;
      int s = 0;
      if (tokens.size() > 1) {
        if (tokens[1] == "DAY") {
          d = i;
          if (tokens.size() > 2) {
            if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
              if (tokens[3] == "MONTH") {
                m = TIME::PRIVATE::month(t);
                y = TIME::PRIVATE::year(t);
                if (tokens[2] == "LAST") {
                  if (m == 1) {
                    m = 12;
                    y --;
                  }
                  else
                    m --;
                }
                else if (tokens[2] == "NEXT") {
                  if (m == 12) {
                    m = 1;
                    y ++;
                  }
                  else
                    m ++;
                }
                if (d <= TIME::PRIVATE::daysInMonth(m, y)) {
                  TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                  t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
                  return t;
                }
                else
                  return nadt;
              }
              if (tokens[3] == "YEAR") {
                y = TIME::PRIVATE::year(t);
                if (tokens[2] == "LAST")
                  y --;
                else if (tokens[2] == "NEXT")
                  y ++;
                TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                if (d <= TIME::PRIVATE::daysInYear(y)) {
                  t = TIME::PRIVATE::makeTime(y, 1, 1, h, n, s);
                  t += days(d - 1);
                  return t;
                }
                else
                  return nadt;
              }
            }
            m = TIME::PRIVATE::month(tokens[2]);
            y = TIME::PRIVATE::year(t);
            int v = val(tokens[2]);
            if (m > 0) {
              if (tokens.size() > 3) {
                if (tokens.size() > 4 && (tokens[3] == "LAST" || tokens[3] == "THIS" || tokens[3] == "NEXT")) {
                  if (tokens[4] == "YEAR") {
                    y = TIME::PRIVATE::year(t);
                    if (tokens[3] == "LAST")
                      y --;
                    else if (tokens[2] == "NEXT")
                      y ++;
                    if (d <= TIME::PRIVATE::daysInMonth(m, y)) {
                      TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                      t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
                      return t;
                    }
                    else
                      return nadt;
                  }
                }
                y = val(tokens[3]);
                if (y < 1601)
                  y = 0;
                if (y == 0)
                  TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
                else
                  TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                if (y == 0)
                  y = TIME::PRIVATE::default_year(t, when, m, d);
              }
              if (d <= TIME::PRIVATE::daysInMonth(m, y)) {
                t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
                return t;
              }
              else
                return nadt;
            }
            if (tokens[2] == "MONTH") {
              TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
              if (d <= TIME::PRIVATE::daysInMonth(TIME::PRIVATE::month(t))) {
                t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), TIME::PRIVATE::month(t), 1, h, n, s);
                t += days(d - 1);
                return t;
              }
              else
                return nadt;
            }
            if (v > 1600) {
              y = v;
              TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
              if (d <= TIME::PRIVATE::daysInYear(y)) {
                t = TIME::PRIVATE::makeTime(y, 1, 1, h, n, s);
                t += days(d - 1);
                return t;
              }
              else
                return nadt;
            }
            if (tokens[2] == "YEAR") {
              TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
              if (d <= TIME::PRIVATE::daysInYear(TIME::PRIVATE::year(t))) {
                t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), 1, 1, h, n, s);
                t += days(d - 1);
                return t;
              }
              else
                return nadt;
            }
          }
        }
        if (tokens[1] == "WEEK") {
          int w = i;
          if (tokens.size() > 2) {
            if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
              if (tokens[3] == "MONTH") {                
                if (w < 6) {
                  m = TIME::PRIVATE::month(t);
                  y = TIME::PRIVATE::year(t);
                  if (tokens[2] == "LAST") {
                    if (m == 1) {
                      m = 12;
                      y --;
                    }
                    else
                      m --;
                  }
                  else if (tokens[2] == "NEXT") {
                    if (m == 12) {
                      m = 11;
                      y ++;
                    }
                    else
                      m ++;
                  }
                  int weeks = 1;
                  t = TIME::PRIVATE::makeTime(y, m, 1);
                  for (d = 2; d <= TIME::PRIVATE::daysInMonth(m, y); d ++) {
                    if (TIME::PRIVATE::weekday(t) == 1)
                      weeks ++;
                    if (weeks == w)
                      break;
                    t += days(1);
                  }
                  if (weeks == w)
                    return t;
                  else
                    return nadt;
                }
              }
              else if (tokens[3] == "YEAR") {
                if (w < 55) {
                  y = TIME::PRIVATE::year(t);
                  if (tokens[2] == "LAST")
                    y --;
                  else if (tokens[2] == "NEXT")
                    y ++;
                  int weeks = 1;
                  t = TIME::PRIVATE::makeTime(y, 1, 1);
                  for (d = 2; d <= TIME::PRIVATE::daysInYear(y); d ++) {
                    if (TIME::PRIVATE::weekday(t) == 1)
                      weeks ++;
                    if (weeks == w)
                      break;
                    t += days(1);
                  }
                  if (weeks == w)
                    return t;
                  else
                    return nadt;
                }
              }
            }
            m = TIME::PRIVATE::month(tokens[2]);
            y = TIME::PRIVATE::year(t);
            int v = val(tokens[2]);
            if (m > 0) {
              if (w < 6) {
                if (tokens.size() > 3) {
                  if (tokens.size() > 4 && (tokens[3] == "LAST" || tokens[3] == "THIS" || tokens[3] == "NEXT")) {
                    if (tokens[4] == "YEAR") {
                      y = TIME::PRIVATE::year(t);
                      if (tokens[3] == "LAST")
                        y --;
                      else if (tokens[2] == "NEXT")
                        y ++;
                    int weeks = 1;
                    t = TIME::PRIVATE::makeTime(y, m, 1);
                    for (d = 2; d <= TIME::PRIVATE::daysInMonth(m, y); d ++) {
                      if (TIME::PRIVATE::weekday(t) == 1)
                        weeks ++;
                      if (weeks == w)
                        break;
                      t += days(1);
                    }
                    if (weeks == w)
                      return t;
                    else
                      return nadt;
                    }
                  }
                  y = val(tokens[3]);
                  if (y < 1601)
                    y = TIME::PRIVATE::year(t);
                }
                int weeks = 1;
                t = TIME::PRIVATE::makeTime(y, m, 1);
                for (d = 2; d <= TIME::PRIVATE::daysInMonth(m, y); d ++) {
                  if (TIME::PRIVATE::weekday(t) == 1)
                    weeks ++;
                  if (weeks == w)
                    break;
                  t += days(1);
                }
                if (weeks == w)
                  return t;
                else
                  return nadt;
              }
            }
            if (v > 1600) {
              if (w < 55) {
                y = v;
                int weeks = 1;
                t = TIME::PRIVATE::makeTime(y, 1, 1);
                for (d = 2; d <= TIME::PRIVATE::daysInYear(y); d ++) {
                  if (TIME::PRIVATE::weekday(t) == 1)
                    weeks ++;
                  if (weeks == w)
                    break;
                  t += days(1);
                }
                if (weeks == w)
                  return t;
                else
                  return nadt;
              }
            }
            if (tokens[2] == "YEAR") {
              if (w < 55) {
                y = TIME::PRIVATE::year(t);
                int weeks = 1;
                t = TIME::PRIVATE::makeTime(y, 1, 1);
                for (d = 2; d <= TIME::PRIVATE::daysInYear(y); d ++) {
                  if (TIME::PRIVATE::weekday(t) == 1)
                    weeks ++;
                  if (weeks == w)
                    break;
                  t += days(1);
                }
                if (weeks == w)
                  return t;
                else
                  return nadt;
              }
            }
          }
        }
        if (tokens[1] == "MONTH") {
          m = i;
          if (m < 13) {
            if (tokens.size() > 2) {
              if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
                if (tokens[3] == "YEAR") {
                  y = TIME::PRIVATE::year(t);
                  if (tokens[2] == "LAST")
                    y --;
                  else if (tokens[2] == "NEXT")
                    y ++;
                  t = TIME::PRIVATE::makeTime(y, m, 1);
                  return t;
                }
              }
              y = val(tokens[2]);
              if (y > 0) {
                if (y < 1601)
                  y = TIME::PRIVATE::default_year(t, when, m);
                t = TIME::PRIVATE::makeTime(y, m, 1);
                return t;
              }
              else if (tokens[2] == "YEAR") {
                t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), m, 1);
                return t;
              }
            }
          }
        }
        {
          int wd = TIME::PRIVATE::weekday(tokens[1]);
          if (wd > -1) {
            int wdc = i;
            if (tokens.size() > 2) {
              if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
                if (tokens[3] == "MONTH") {                
                  if (wdc < 6) {
                    m = TIME::PRIVATE::month(t);
                    y = TIME::PRIVATE::year(t);
                    if (tokens[2] == "LAST") {
                      if (m == 1) {
                        m = 12;
                        y --;
                      }
                      else
                        m --;
                    }
                    else if (tokens[2] == "NEXT") {
                      if (m == 12) {
                        m = 11;
                        y ++;
                      }
                      else
                        m ++;
                    }
                    int weekdays = 0;
                    t = TIME::PRIVATE::makeTime(y, m, 1);
                    for (d = 1; d <= TIME::PRIVATE::daysInMonth(m, y); d ++) {
                      if (TIME::PRIVATE::weekday(t) == wd)
                        weekdays ++;
                      if (weekdays == wdc)
                        break;
                      t += days(1);
                    }
                    if (weekdays == wdc) {
                      TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                      t += time_duration(h, n, s);
                      return t;
                    }
                    else
                      return nadt;
                  }
                }
                else if (tokens[3] == "YEAR") {
                  if (wdc < 55) {
                    y = TIME::PRIVATE::year(t);
                    if (tokens[2] == "LAST")
                      y --;
                    else if (tokens[2] == "NEXT")
                      y ++;                  
                    int weekdays = 1;
                    t = TIME::PRIVATE::makeTime(y, 1, 1);
                    for (d = 1; d <= TIME::PRIVATE::daysInYear(y); d ++) {
                      if (TIME::PRIVATE::weekday(t) == wd)
                        weekdays ++;
                      if (weekdays == wdc)
                        break;
                      t += days(1);
                    }
                    if (weekdays == wdc) {
                      TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                      t += time_duration(h, n, s);
                      return t;
                    }
                    else
                      return nadt;
                  }
                }
              }
              m = TIME::PRIVATE::month(tokens[2]);
              y = TIME::PRIVATE::year(t);
              int v = val(tokens[2]);
              if (m > 0) {
                if (wdc < 6) {
                  if (tokens.size() > 3) {
                    if (tokens.size() > 4 && (tokens[3] == "LAST" || tokens[3] == "THIS" || tokens[3] == "NEXT")) {
                      if (tokens[4] == "YEAR") {
                        y = TIME::PRIVATE::year(t);
                        if (tokens[3] == "LAST")
                          y --;
                        else if (tokens[2] == "NEXT")
                          y ++;
                      int weekdays = 0;
                      t = TIME::PRIVATE::makeTime(y, m, 1);
                      for (d = 1; d <= TIME::PRIVATE::daysInMonth(m, y); d ++) {
                        if (TIME::PRIVATE::weekday(t) == wd)
                          weekdays ++;
                        if (weekdays == wdc)
                          break;
                        t += days(1);
                      }
                      if (weekdays == wdc) {
                        TIME::PRIVATE::time_from_tokens(tokens, 5, h, n, s);
                        t += time_duration(h, n, s);
                        return t;
                      }
                      else
                        return nadt;
                      }
                    }
                    y = val(tokens[3]);
                    if (y < 1601)
                      y = 0;
                  }
                  if (y == 0) {
                    TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
                    y = TIME::PRIVATE::year(t);
                  }
                  else
                    TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                  int weeks = 0;
                  t = TIME::PRIVATE::makeTime(y, m, 1);
                  for (d = 1; d <= TIME::PRIVATE::daysInMonth(m, y); d ++) {
                    if (TIME::PRIVATE::weekday(t) == wd)
                      weeks ++;
                    if (weeks == wdc)
                      break;
                    t += days(1);
                  }
                  if (weeks == wdc) {
                    t += time_duration(h, n, s);
                    return t;
                  }
                  else
                    return nadt;
                }
              }
              if (v > 1600) {
                if (wdc < 55) {
                  y = v;
                  int weeks = 0;
                  t = TIME::PRIVATE::makeTime(y, 1, 1);
                  for (d = 1; d <= TIME::PRIVATE::daysInYear(y); d ++) {
                    if (TIME::PRIVATE::weekday(t) == wd)
                      weeks ++;
                    if (weeks == wdc)
                      break;
                    t += days(1);
                  }
                  if (weeks == wdc) {
                    TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
                    t += time_duration(h, n, s);
                    return t;
                  }
                  else
                    return nadt;
                }
              }
              if (tokens[2] == "YEAR") {
                if (wdc < 55) {
                  y = TIME::PRIVATE::year(t);
                  int weeks = 0;
                  t = TIME::PRIVATE::makeTime(y, 1, 1);
                  for (d = 1; d <= TIME::PRIVATE::daysInYear(y); d ++) {
                    if (TIME::PRIVATE::weekday(t) == wd)
                      weeks ++;
                    if (weeks == wdc)
                      break;
                    t += days(1);
                  }
                  if (weeks == wdc) {
                    TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
                    t += time_duration(h, n, s);
                    return t;
                  }
                  else
                    return nadt;
                }
              }
            }
          }
        }
      }
    }
  }


  {
    int d = num_phrase_to_num(tokens[0]);
    if (d == 0)
      d = val(tokens[0]);
    if (d > 0) {
      if (tokens.size() > 1) {
        if (tokens.size() > 2 && (tokens[1] == "LAST" || tokens[1] == "THIS" || tokens[1] == "NEXT")) {
          if (tokens[2] == "MONTH") {
            int y = 0;
            int m = 0;
            int h = 0;
            int n = 0;
            int s = 0;
            m = TIME::PRIVATE::month(t);
            y = TIME::PRIVATE::year(t);
            if (tokens[1] == "LAST") {
              if (m == 1) {
                m = 12;
                y --;
              }
              else
                m --;
            }
            else if (tokens[1] == "NEXT") {
              if (m == 12) {
                m = 11;
                y ++;
              }
              else
                m ++;
            }
            if (d <= TIME::PRIVATE::daysInMonth(m, y)) {
              TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
              t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
              return t;
            }
            else
              return nadt;
          }
        }
        int m = TIME::PRIVATE::month(tokens[1]);
        if (m > 0 && d <= TIME::PRIVATE::daysInMonth(m)) {
          t = ref;
          int y = 0;
          int h = 0;
          int n = 0;
          int s = 0;
          if (tokens.size() > 2) {
            if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
              if (tokens[3] == "YEAR") {
                y = TIME::PRIVATE::year(t);
                if (tokens[2] == "LAST")
                  y --;
                else if (tokens[2] == "NEXT")
                  y ++;
                TIME::PRIVATE::time_from_tokens(tokens, 4, h, n, s);
                t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
                return t;
              }
            }
            h = TIME::PRIVATE::time_phrase_to_time(tokens[2]);
            if (h == 0) {
              y = val(tokens[2]);
              if (y < 1601)
                y = 0;
              if (y == 0)
                TIME::PRIVATE::time_from_tokens(tokens, 2, h, n, s);
              else
                TIME::PRIVATE::time_from_tokens(tokens, 3, h, n, s);
            }
          }
          if (y == 0)
            y = TIME::PRIVATE::default_year(t, when, m, d);
          t = TIME::PRIVATE::makeTime(y, m, d, h, n, s);
          return t;
        }
      }
    }
  }


  {
    int v = val(tokens[0]);
    if (v > 0 || str_is_0(tokens[0])) {
      if (v > 1600)
        t = TIME::PRIVATE::makeTime(v, 1, 1);
      else if (v < 25) {
        ptime ct = ref;
        t = TIME::PRIVATE::justDate(ct);
        int h = 0;
        int n = 0;
        int s = 0;
        bool ampm = false;
        h = v;
        if (tokens.size() > 1) {
          ampm = TIME::PRIVATE::testAMPM(tokens[1], h);
          if (!ampm) {
            n = val(tokens[1]);
            if ((n > 0 || str_is_0(tokens[1])) && tokens.size() > 2) {
              ampm = TIME::PRIVATE::testAMPM(tokens[2], h);
              if (!ampm) {
                s = val(tokens[2]);
                if (tokens.size() > 3) {
                  ampm = TIME::PRIVATE::testAMPM(tokens[3], h);
                }
              }
            }
          }
        }
        if (when == 2 || when == 3) {
          if (h < 13) {
            if (TIME::PRIVATE::hour(ct) > 11 && !ampm) {
              if (TIME::PRIVATE::hour(ct) < h + 12) {
                h += 12;
              }
              else if (TIME::PRIVATE::hour(ct) == h + 12) {
                if (TIME::PRIVATE::minute(ct) < n) {
                  h += 12;
                }
                else if (TIME::PRIVATE::minute(ct) == n) {
                  if (TIME::PRIVATE::second(ct) < s) {
                    h += 12;
                  }
                }
              }
            }
            else {
              if (TIME::PRIVATE::hour(ct) > h) {
                t += days(1);
              }
              else if (TIME::PRIVATE::hour(ct) == h) {
                if (TIME::PRIVATE::minute(ct) > n) {
                  t += days(1);
                }
                else if (TIME::PRIVATE::minute(ct) == n) {
                  if (TIME::PRIVATE::second(ct) > s)
                    t += days(1);
                }
              }
            }
          }
          else {
            if (TIME::PRIVATE::hour(ct) > h) {
              t += days(1);
            }
            else if (TIME::PRIVATE::hour(ct) == h) {
              if (TIME::PRIVATE::minute(ct) > n) {
                t += days(1);
              }
              else if (TIME::PRIVATE::minute(ct) == n) {
                if (TIME::PRIVATE::second(ct) > s)
                  t += days(1);
              }
            }
          }
        }
        else {
          if (TIME::PRIVATE::hour(ct) > 11 && !ampm) {
            if (TIME::PRIVATE::hour(ct) > h + 12) {
              h += 12;
            }
            else if (TIME::PRIVATE::hour(ct) == h + 12) {
              if (TIME::PRIVATE::minute(ct) > n) {
                h += 12;
              }
              else if (TIME::PRIVATE::minute(ct) == n) {
                if (TIME::PRIVATE::second(ct) > s) {
                  h += 12;
                }
              }
            }
          }
          else {
            if (TIME::PRIVATE::hour(ct) < h) {
              t -= days(1);
            }
            else if (TIME::PRIVATE::hour(ct) == h) {
              if (TIME::PRIVATE::minute(ct) < n) {
                t -= days(1);
              }
              else if (TIME::PRIVATE::minute(ct) == n) {
                if (TIME::PRIVATE::second(ct) < s)
                  t -= days(1);
              }
            }
          }
        }
        t += hours(h);
        t += minutes(n);
        t += seconds(s);
      }
      return t;
    }
  }

  if (tokens[0] == "YEAR" && when > 0) {
    t = ref;
    t = TIME::PRIVATE::justDate(t);
    if (when == 3)
      t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t), 1, 1);
    else if (when == 2)
      t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t) + 1, 1, 1);
    else if (when == 1)
      t = TIME::PRIVATE::makeTime(TIME::PRIVATE::year(t) - 1, 1, 1);
    return t;
  }


  return nadt;
}


int TIME::PRIVATE::daysInMonth (int m)
{
  return TIME::PRIVATE::daysInMonth(m, 1);
}
int TIME::PRIVATE::daysInMonth (int m, int y)
{
  if (m == 2)
    return 28 + TIME::PRIVATE::isLeapYear(y);
  if (m == 4 || m == 6 || m == 9 || m == 11)
    return 30;
  return 31;
}

int TIME::PRIVATE::daysInYear (int y)
{
  return 365 + TIME::PRIVATE::isLeapYear(y);
}

bool TIME::PRIVATE::isLeapYear (int y)
{
  return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}


int TIME::PRIVATE::default_year (ptime t, int when, int m)
{
  return TIME::PRIVATE::default_year(t, when, m, 1);
}
int TIME::PRIVATE::default_year (ptime t, int when, int m, int d)
{
  return TIME::PRIVATE::default_year(t, when, m, d, 0, 0, 0);
}
int TIME::PRIVATE::default_year (ptime t, int when, int m, int d, int h, int n, int s)
{
  int y = TIME::PRIVATE::year(t);
  if (when == 2 || when == 3) {
    if (TIME::PRIVATE::month(t) > m)
        y += 1;
    else if (TIME::PRIVATE::month(t) == m) {
      if (TIME::PRIVATE::day(t) > d)
        y += 1;
      else if (TIME::PRIVATE::day(t) == d) {
        if (TIME::PRIVATE::hour(t) > h)
          y += 1;
        else if (TIME::PRIVATE::hour(t) == h) {
          if (TIME::PRIVATE::minute(t) > n)
            y += 1;
          else if (TIME::PRIVATE::minute(t) == n) {
            if (TIME::PRIVATE::second(t) > s)
              y += 1;
          }
        }
      }
    }
  }
  else {
    if (TIME::PRIVATE::month(t) < m)
        y -= 1;
    else if (TIME::PRIVATE::month(t) == m) {
      if (TIME::PRIVATE::day(t) < d)
        y -= 1;
      else if (TIME::PRIVATE::day(t) == d) {
        if (TIME::PRIVATE::hour(t) < h)
          y -= 1;
        else if (TIME::PRIVATE::hour(t) == h) {
          if (TIME::PRIVATE::minute(t) < n)
            y -= 1;
          else if (TIME::PRIVATE::minute(t) == n) {
            if (TIME::PRIVATE::second(t) < s)
              y -= 1;
          }
        }
      }
    }
  }
  return y;
}


ptime TIME::PRIVATE::justDate(ptime t)
{
  tm pt_tm = to_tm(t);
  ptime d = makeTime(pt_tm.tm_year + 1900, pt_tm.tm_mon + 1, pt_tm.tm_mday, 0, 0, 0);
  return d;
}


int TIME::PRIVATE::second (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_sec;
}
int TIME::PRIVATE::minute (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_min;
}

int TIME::PRIVATE::hour (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_hour;
}

int TIME::PRIVATE::day (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_mday;
}

int TIME::PRIVATE::weekday (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_wday;
}

int TIME::PRIVATE::month (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_mon + 1;
}

int TIME::PRIVATE::year (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_year + 1900;
}


int TIME::PRIVATE::time_phrase_to_time (string s)
{
  if (s == "MORNING" || s == "DAWN")
    return 6;
  if (s == "NOON")
    return 12;
  if (s == "EVENING" || s == "DUSK")
    return 18;
  if (s == "NIGHT")
    return 21;
  if (s == "MIDNIGHT")
    return 24;
  return 0;
}

int TIME::PRIVATE::weekday (string s)
{
  s = uCase(removePunctuation(removeSpace(s)));
  if (s == "SUNDAY"    || s == "SUN")
    return 0;
  if (s == "MONDAY"    || s == "MON")
    return 1;
  if (s == "TUESDAY"   || s == "TUE")
    return 2;
  if (s == "WEDNESDAY" || s == "WED")
    return 3;
  if (s == "THURSDAY"  || s == "THU")
    return 4;
  if (s == "FRIDAY"    || s == "FRI")
    return 5;
  if (s == "SATURDAY"  || s == "SAT")
    return 6;
  return -1;
}

int TIME::PRIVATE::month (string s)
{
  s = uCase(removePunctuation(removeSpace(s)));
  if (s == "JANUARY"   || s == "JAN")
    return 1;
  if (s == "FEBRUARY"  || s == "FEB")
    return 2;
  if (s == "MARCH"     || s == "MAR")
    return 3;
  if (s == "APRIL"     || s == "APR")
    return 4;
  if (s == "MAY"       || s == "MAY")
    return 5;
  if (s == "JUNE"      || s == "JUN")
    return 6;
  if (s == "JULY"      || s == "JUL")
    return 7;
  if (s == "AUGUST"    || s == "AUG")
    return 8;
  if (s == "SEPTEMBER" || s == "SEP")
    return 9;
  if (s == "OCTOBER"   || s == "OCT")
    return 10;
  if (s == "NOVEMBER"  || s == "NOV")
    return 11;
  if (s == "DECEMBER"  || s == "DEC")
    return 12;
  return -1;
}

string TIME::PRIVATE::weekday_str (int i)
{
  switch (i)
  {
    case 0 : 
      return "Sunday";
    case 1 : 
      return "Monday";
    case 2 : 
      return "Tuesday";
    case 3 : 
      return "Wednesday";
    case 4 : 
      return "Thursday";
    case 5 : 
      return "Friday";
    case 6 : 
      return "Saturday";
  }
  return "";
}

string TIME::PRIVATE::month_str (int i)
{
  switch (i)
  {
    case 1 : 
      return "January";
    case 2 : 
      return "February";
    case 3 : 
      return "March";
    case 4 : 
      return "April";
    case 5 : 
      return "May";
    case 6 : 
      return "June";
    case 7 : 
      return "July";
    case 8 : 
      return "August";
    case 9 : 
      return "September";
    case 10 : 
      return "October";
    case 11 : 
      return "November";
    case 12 : 
      return "December";
  }
  return "";
}


void TIME::PRIVATE::time_from_tokens (vector<string>& tokens, int i, int& h, int& n, int& s)
{
  size_t j = (size_t) i;
  if (tokens.size() > j) {
    h = val(tokens[j]);
    if (h > 24 || h < 0)
      h = 0;
    if ((h > 0 || str_is_0(tokens[j])) && tokens.size() > j + 1) {
      if (!TIME::PRIVATE::testAMPM(tokens[j + 1], h)) {
        n = val(tokens[j + 1]);
        if (n > 60 || n < 0)
          n = 0;
        if ((n > 0 || str_is_0(tokens[j + 1])) && tokens.size() > j + 2) {
          if (!TIME::PRIVATE::testAMPM(tokens[j + 2], h)) {
            s = val(tokens[j + 2]);
            if (s > 60 || s < 0)
              s = 0;
            if (tokens.size() > j + 3)
              TIME::PRIVATE::testAMPM(tokens[j + 3], h);
          }
        }
      }
    }
  }
}


bool TIME::PRIVATE::testAMPM (string& s, int& h)
{
  if (s == "AM") {
    if (h == 12)
      h = 0;  
  }
  else if (s == "PM") {
    if (h < 12)
      h += 12;
  }
  else {
    return false;
  }
  return true;
}


ptime TIME::PRIVATE::tm_to_ptime (tm tm)
{
  ptime t = TIME::PRIVATE::makeTime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  return t;
}