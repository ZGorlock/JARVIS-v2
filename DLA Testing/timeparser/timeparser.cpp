#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cwchar>
#include <Windows.h>

#include <algorithm>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

//#include "common.h"

ptime makeTime (string);
ptime makeTime (string, ptime);

void time_from_tokens (vector<string>&, int, int&, int&, int&);


int default_year (ptime, int, int);
int default_year (ptime, int, int, int);
int default_year (ptime, int, int, int, int, int, int);


int   weekday  (string);
int   month    (string);

int   second   (ptime);
int   minute   (ptime);
int   hour     (ptime);
int   day      (ptime);
int   weekday  (ptime);
int   month    (ptime);
int   year     (ptime);

int   daysInMonth (int);
int   daysInMonth (int, int);
int   daysInYear  (int);
bool  isLeapYear  (int);

int num_phrase_to_num (string);
int time_phrase_to_time (string);



boost::posix_time::ptime currentTime ();
boost::posix_time::ptime makeTime    (int, int, int);
boost::posix_time::ptime makeTime    (int, int, int, int, int, int);
boost::posix_time::ptime makeTime    (int, int, int, int, int, int, int);

boost::posix_time::ptime tm_to_ptime (tm);
boost::posix_time::ptime justDate    (ptime);




string uCase (string);
int    val   (string);

string removePunctuation (string);
string removeSpace       (string);
vector<string> tokenize  (string);
vector<string> tokenize  (string, bool);

bool str_is_0 (string);

void ReplaceStringInPlace(string&, const string&, const string&);

bool testAMPM (string&, int&);



int main ()
{
  int year = 0;
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;
  int second = 0;

  string a = "";
  ptime t;

  while (true) {
    getline(cin, a);
    t = makeTime(a);
    if (t < not_a_date_time)
      cout << to_simple_string(t) << endl;
  }

  return 0;
}












ptime makeTime (string str)
{
  return makeTime(str, currentTime());
}
ptime makeTime (string str, ptime ref)
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

  for (size_t i = 0; i < tokens.size(); i ++)
    cout << tokens[i] << endl;


  if (tokens.size() == 0)
    return nadt;


  if (tokens.size() == 1) {
    if (tokens[0] == "NOW") {
     t = ref;
      return t;
    }
    if (tokens[0] == "TODAY") {
      t = ref;
      t = justDate(t);
      return t;
    }
    if (tokens[0] == "YESTERDAY") {
      t = ref;
      t -= days(1);
      t = justDate(t);
      return t;
    }
    if (tokens[0] == "TOMORROW") {
      t = ref;
      t += days(1);
      t = justDate(t);
      return t;
    }
  }


  {
    int h = time_phrase_to_time(tokens[0]);
    if (h > 0) {
      t = ref;
      if (hour(t) >= h) {
        if (when == 2)
          t += days(1);
      }
      else {
        if (when == 1)
          t -= days(1);
      }
      t = justDate(t);
      t += hours(h);
      return t;
    }
  }


  if (tokens[0] == "TODAY") {
    t = ref;
    t = justDate(t);
    if (tokens.size() > 1) {
      int h = 0;
      int n = 0;
      int s = 0;
      h = time_phrase_to_time(tokens[1]);
      if (h > 0) {
        t += hours(h);
      }
      else {
        time_from_tokens(tokens, 1, h, n, s);
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
    t = justDate(t);
    if (tokens.size() > 1) {
      int h = 0;
      int n = 0;
      int s = 0;
      h = time_phrase_to_time(tokens[1]);
      if (h > 0) {
        t += hours(h);
      }
      else {
        time_from_tokens(tokens, 1, h, n, s);
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
    t = justDate(t);
    if (tokens.size() > 1) {
      int h = 0;
      int n = 0;
      int s = 0;
      h = time_phrase_to_time(tokens[1]);
      if (h > 0) {
        t += hours(h);
      }
      else {
        time_from_tokens(tokens, 1, h, n, s);
        t += hours(h);
        t += minutes(n);
        t += seconds(s);
      }
    }
    return t;
  }


  {
    int wd = weekday(tokens[0]);
    if (wd > -1) {
      t = ref;
      t = justDate(t);
      if (tokens.size() > 1) {
        if (tokens.size() > 2 && (tokens[1] == "LAST" || tokens[1] == "THIS" || tokens[1] == "NEXT")) {
          if (tokens[2] == "WEEK") {
            if (tokens[1] == "LAST") {
              while (weekday(t) != 1)
                  t -= days(1);
              do {
                t -= days(1);
              } while (weekday(t) != wd);
              return t;
            }
            else if (tokens[1] == "THIS") {
              while (weekday(t) != 0)
                  t -= days(1);
              do {
                t += days(1);
              } while (weekday(t) != wd);
              return t;
            }
            else if (tokens[1] == "NEXT") {
              while (weekday(t) != 0)
                  t += days(1);
              do {
                t += days(1);
              } while (weekday(t) != wd);
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
            m = month(tokens[1]);
            if (m > 0) {
              if (tokens.size() > 2) {
                y = val(tokens[2]);
                if (y < 1601)
                  y = 0;
                if (y == 0)
                  time_from_tokens(tokens, 2, h, n, s);
                else
                  time_from_tokens(tokens, 3, h, n, s);
              }
            }
            if (y == 0)
              y = default_year(t, when, m);
            t = makeTime(y, m, daysInMonth(m, y), h, n, s);
            while (weekday(t) != wd)
              t -= days(1);
            return t;
          }
          do {
            t -= days(1);
          } while (weekday(t) != wd);
          break;
        case 2 :
          do {
            t += days(1);
          } while (weekday(t) != wd);
          break;
        case 3 :
          while (weekday(t) != wd)
            t += days(1);
          break;
        default :
          do {
            t -= days(1);
          } while (weekday(t) != wd);
      }
      if (tokens.size() > 1) {
        int h = 0;
        int n = 0;
        int s = 0;
        h = time_phrase_to_time(tokens[1]);
        if (h > 0) {
          t += hours(h);
        }
        else {
          time_from_tokens(tokens, 1, h, n, s);
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
      int m = month(tokens[1]);
      if (m > 0) {
        if (tokens.size() > 2) {
          y = val(tokens[2]);
          if (y < 1601)
            y = 0;
        }
        if (y == 0)
          y = year(t);
        t = makeTime(y, m, daysInMonth(m, y));
        return t;
      }
    }
  }

  if (tokens[0] == "WEEK" && when > 0) {
    t = ref;
    t = justDate(t);
    do {
      if (when == 2)
        t += days(1);
      else if (when == 1 || when == 3)
        t -= days(1);
    } while (weekday(t) != 1);
    return t;
  }
  if (tokens[0] == "WEEKEND") {
    t = ref;
    t = justDate(t);
    do {
      if (when == 2 || when == 3)
        t += days(1);
      else
        t -= days(1);
    } while (weekday(t) != 6);
    return t;
  }
  if (tokens.size() > 1) {
    if (tokens[0] == "WEEK" && tokens[1] == "END") {
      t = ref;
      t = justDate(t);
      do {
        if (when == 2 || when == 3)
          t += days(1);
        else
          t -= days(1);
      } while (weekday(t) != 6);
      return t;
    }
  }


  {
    int m = month(tokens[0]);
    if (m > 0) {
      t = ref;
      t = justDate(t);
      int y = 0;
      int d = 0;
      int h = 0;
      int n = 0;
      int s = 0;
      if (tokens.size() > 1) {
        if (tokens.size() > 2 || (tokens[1] == "LAST" || tokens[1] == "THIS" || tokens[1] == "NEXT")) {
          if (tokens[2] == "YEAR") {
            y = year(t);
            if (tokens[1] == "LAST")
              y --;
            else if (tokens[1] == "NEXT")
              y ++;
            t = makeTime(y, m, 1);
            return t;
          }
        }
        d = val(tokens[1]);
        if (d == 0)
          d = num_phrase_to_num(tokens[1]);
      }
      if (d == 0)
        d = 1;
      if (d > daysInMonth(m)) {
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
              y = year(t);
              if (tokens[2] == "LAST")
                y --;
              else if (tokens[2] == "NEXT")
                y ++;
              t = makeTime(y, m, d);
              return t;
            }
          }
          h = time_phrase_to_time(tokens[2]);
          if (h == 0) {
            y = val(tokens[2]);
            if (y < 1601)
              y = 0;
            if (y == 0)
              time_from_tokens(tokens, 2, h, n, s);
            else
              time_from_tokens(tokens, 3, h, n, s);
          }
        }        
        if (y == 0)
          y = default_year(t, when, m, d);
      }
      t = makeTime(y, m, d, h, n, s);
      return t;
    }
  }

  if (tokens[0] == "MONTH" && when > 0) {
    t = ref;
    t = justDate(t);
    int y = 0;
    switch (when) {
      case 1 : 
        if (tokens.size() > 1) {
          y = val(tokens[1]);
          if (y < 1601)
            y = 0;
        }
        if (y == 0) {
          if (month(t) == 1)
            t = makeTime(year(t) - 1, 12, 1);
          else
            t = makeTime(year(t), month(t) - 1, 1);
        }
        else {
          t = makeTime(y, 12, 1);
        }
        break;
      case 2 : 
        if (month(t) == 12)
          t = makeTime(year(t) + 1, 1, 1);
        else
          t = makeTime(year(t), month(t) + 1, 1);
        break;
      case 3 : 
        t = makeTime(year(t), month(t), 1);
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
                m = month(t);
                y = year(t);
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
                if (d <= daysInMonth(m, y)) {
                  time_from_tokens(tokens, 4, h, n, s);
                  t = makeTime(y, m, d, h, n, s);
                  return t;
                }
                else
                  return nadt;
              }
              if (tokens[3] == "YEAR") {
                y = year(t);
                if (tokens[2] == "LAST")
                  y --;
                else if (tokens[2] == "NEXT")
                  y ++;
                time_from_tokens(tokens, 4, h, n, s);
                if (d <= daysInYear(y)) {
                  t = makeTime(y, 1, 1, h, n, s);
                  t += days(d - 1);
                  return t;
                }
                else
                  return nadt;
              }
            }
            m = month(tokens[2]);
            y = year(t);
            int v = val(tokens[2]);
            if (m > 0) {
              if (tokens.size() > 3) {
                if (tokens.size() > 4 && (tokens[3] == "LAST" || tokens[3] == "THIS" || tokens[3] == "NEXT")) {
                  if (tokens[4] == "YEAR") {
                    y = year(t);
                    if (tokens[3] == "LAST")
                      y --;
                    else if (tokens[2] == "NEXT")
                      y ++;
                    if (d <= daysInMonth(m, y)) {
                      time_from_tokens(tokens, 4, h, n, s);
                      t = makeTime(y, m, d, h, n, s);
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
                  time_from_tokens(tokens, 3, h, n, s);
                else
                  time_from_tokens(tokens, 4, h, n, s);
                if (y == 0)
                  y = default_year(t, when, m, d);
              }
              if (d <= daysInMonth(m, y)) {
                t = makeTime(y, m, d, h, n, s);
                return t;
              }
              else
                return nadt;
            }
            if (tokens[2] == "MONTH") {
              time_from_tokens(tokens, 3, h, n, s);
              if (d <= daysInMonth(month(t))) {
                t = makeTime(year(t), month(t), 1, h, n, s);
                t += days(d - 1);
                return t;
              }
              else
                return nadt;
            }
            if (v > 1600) {
              y = v;
              time_from_tokens(tokens, 3, h, n, s);
              if (d <= daysInYear(y)) {
                t = makeTime(y, 1, 1, h, n, s);
                t += days(d - 1);
                return t;
              }
              else
                return nadt;
            }
            if (tokens[2] == "YEAR") {
              time_from_tokens(tokens, 3, h, n, s);
              if (d <= daysInYear(year(t))) {
                t = makeTime(year(t), 1, 1, h, n, s);
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
                  m = month(t);
                  y = year(t);
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
                  t = makeTime(y, m, 1);
                  for (d = 2; d <= daysInMonth(m, y); d ++) {
                    if (weekday(t) == 1)
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
                  y = year(t);
                  if (tokens[2] == "LAST")
                    y --;
                  else if (tokens[2] == "NEXT")
                    y ++;
                  int weeks = 1;
                  t = makeTime(y, 1, 1);
                  for (d = 2; d <= daysInYear(y); d ++) {
                    if (weekday(t) == 1)
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
            m = month(tokens[2]);
            y = year(t);
            int v = val(tokens[2]);
            if (m > 0) {
              if (w < 6) {
                if (tokens.size() > 3) {
                  if (tokens.size() > 4 && (tokens[3] == "LAST" || tokens[3] == "THIS" || tokens[3] == "NEXT")) {
                    if (tokens[4] == "YEAR") {
                      y = year(t);
                      if (tokens[3] == "LAST")
                        y --;
                      else if (tokens[2] == "NEXT")
                        y ++;
                    int weeks = 1;
                    t = makeTime(y, m, 1);
                    for (d = 2; d <= daysInMonth(m, y); d ++) {
                      if (weekday(t) == 1)
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
                    y = year(t);
                }
                int weeks = 1;
                t = makeTime(y, m, 1);
                for (d = 2; d <= daysInMonth(m, y); d ++) {
                  if (weekday(t) == 1)
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
                t = makeTime(y, 1, 1);
                for (d = 2; d <= daysInYear(y); d ++) {
                  if (weekday(t) == 1)
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
                y = year(t);
                int weeks = 1;
                t = makeTime(y, 1, 1);
                for (d = 2; d <= daysInYear(y); d ++) {
                  if (weekday(t) == 1)
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
                  y = year(t);
                  if (tokens[2] == "LAST")
                    y --;
                  else if (tokens[2] == "NEXT")
                    y ++;
                  t = makeTime(y, m, 1);
                  return t;
                }
              }
              y = val(tokens[2]);
              if (y > 0) {
                if (y < 1601)
                  y = default_year(t, when, m);
                t = makeTime(y, m, 1);
                return t;
              }
              else if (tokens[2] == "YEAR") {
                t = makeTime(year(t), m, 1);
                return t;
              }
            }
          }
        }
        {
          int wd = weekday(tokens[1]);
          if (wd > -1) {
            int wdc = i;
            if (tokens.size() > 2) {
              if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
                if (tokens[3] == "MONTH") {                
                  if (wdc < 6) {
                    m = month(t);
                    y = year(t);
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
                    t = makeTime(y, m, 1);
                    for (d = 1; d <= daysInMonth(m, y); d ++) {
                      if (weekday(t) == wd)
                        weekdays ++;
                      if (weekdays == wdc)
                        break;
                      t += days(1);
                    }
                    if (weekdays == wdc) {
                      time_from_tokens(tokens, 4, h, n, s);
                      t += time_duration(h, n, s);
                      return t;
                    }
                    else
                      return nadt;
                  }
                }
                else if (tokens[3] == "YEAR") {
                  if (wdc < 55) {
                    y = year(t);
                    if (tokens[2] == "LAST")
                      y --;
                    else if (tokens[2] == "NEXT")
                      y ++;                  
                    int weekdays = 1;
                    t = makeTime(y, 1, 1);
                    for (d = 1; d <= daysInYear(y); d ++) {
                      if (weekday(t) == wd)
                        weekdays ++;
                      if (weekdays == wdc)
                        break;
                      t += days(1);
                    }
                    if (weekdays == wdc) {
                      time_from_tokens(tokens, 4, h, n, s);
                      t += time_duration(h, n, s);
                      return t;
                    }
                    else
                      return nadt;
                  }
                }
              }
              m = month(tokens[2]);
              y = year(t);
              int v = val(tokens[2]);
              if (m > 0) {
                if (wdc < 6) {
                  if (tokens.size() > 3) {
                    if (tokens.size() > 4 && (tokens[3] == "LAST" || tokens[3] == "THIS" || tokens[3] == "NEXT")) {
                      if (tokens[4] == "YEAR") {
                        y = year(t);
                        if (tokens[3] == "LAST")
                          y --;
                        else if (tokens[2] == "NEXT")
                          y ++;
                      int weekdays = 0;
                      t = makeTime(y, m, 1);
                      for (d = 1; d <= daysInMonth(m, y); d ++) {
                        if (weekday(t) == wd)
                          weekdays ++;
                        if (weekdays == wdc)
                          break;
                        t += days(1);
                      }
                      if (weekdays == wdc) {
                        time_from_tokens(tokens, 5, h, n, s);
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
                    time_from_tokens(tokens, 3, h, n, s);
                    y = year(t);
                  }
                  else
                    time_from_tokens(tokens, 4, h, n, s);
                  int weeks = 0;
                  t = makeTime(y, m, 1);
                  for (d = 1; d <= daysInMonth(m, y); d ++) {
                    if (weekday(t) == wd)
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
                  t = makeTime(y, 1, 1);
                  for (d = 1; d <= daysInYear(y); d ++) {
                    if (weekday(t) == wd)
                      weeks ++;
                    if (weeks == wdc)
                      break;
                    t += days(1);
                  }
                  if (weeks == wdc) {
                    time_from_tokens(tokens, 3, h, n, s);
                    t += time_duration(h, n, s);
                    return t;
                  }
                  else
                    return nadt;
                }
              }
              if (tokens[2] == "YEAR") {
                if (wdc < 55) {
                  y = year(t);
                  int weeks = 0;
                  t = makeTime(y, 1, 1);
                  for (d = 1; d <= daysInYear(y); d ++) {
                    if (weekday(t) == wd)
                      weeks ++;
                    if (weeks == wdc)
                      break;
                    t += days(1);
                  }
                  if (weeks == wdc) {
                    time_from_tokens(tokens, 3, h, n, s);
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
            m = month(t);
            y = year(t);
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
            if (d <= daysInMonth(m, y)) {
              time_from_tokens(tokens, 3, h, n, s);
              t = makeTime(y, m, d, h, n, s);
              return t;
            }
            else
              return nadt;
          }
        }
        int m = month(tokens[1]);
        if (m > 0 && d <= daysInMonth(m)) {
          t = ref;
          int y = 0;
          int h = 0;
          int n = 0;
          int s = 0;
          if (tokens.size() > 2) {
            if (tokens.size() > 3 && (tokens[2] == "LAST" || tokens[2] == "THIS" || tokens[2] == "NEXT")) {
              if (tokens[3] == "YEAR") {
                y = year(t);
                if (tokens[2] == "LAST")
                  y --;
                else if (tokens[2] == "NEXT")
                  y ++;
                time_from_tokens(tokens, 4, h, n, s);
                t = makeTime(y, m, d, h, n, s);
                return t;
              }
            }
            h = time_phrase_to_time(tokens[2]);
            if (h == 0) {
              y = val(tokens[2]);
              if (y < 1601)
                y = 0;
              if (y == 0)
                time_from_tokens(tokens, 2, h, n, s);
              else
                time_from_tokens(tokens, 3, h, n, s);
            }
          }
          if (y == 0)
            y = default_year(t, when, m, d);
          t = makeTime(y, m, d, h, n, s);
          return t;
        }
      }
    }
  }


  {
    int v = val(tokens[0]);
    if (v > 0 || str_is_0(tokens[0])) {
      if (v > 1600)
        t = makeTime(v, 1, 1);
      else if (v < 25) {
        ptime ct = ref;
        t = justDate(ct);
        int h = 0;
        int n = 0;
        int s = 0;
        bool ampm = false;
        h = v;
        if (tokens.size() > 1) {
          ampm = testAMPM(tokens[1], h);
          if (!ampm) {
            n = val(tokens[1]);
            if ((n > 0 || str_is_0(tokens[1])) && tokens.size() > 2) {
              ampm = testAMPM(tokens[2], h);
              if (!ampm) {
                s = val(tokens[2]);
                if (tokens.size() > 3) {
                  ampm = testAMPM(tokens[3], h);
                }
              }
            }
          }
        }
        if (when == 2 || when == 3) {
          if (h < 13) {
            if (hour(ct) > 11 && !ampm) {
              if (hour(ct) < h + 12) {
                h += 12;
              }
              else if (hour(ct) == h + 12) {
                if (minute(ct) < n) {
                  h += 12;
                }
                else if (minute(ct) == n) {
                  if (second(ct) < s) {
                    h += 12;
                  }
                }
              }
            }
            else {
              if (hour(ct) > h) {
                t += days(1);
              }
              else if (hour(ct) == h) {
                if (minute(ct) > n) {
                  t += days(1);
                }
                else if (minute(ct) == n) {
                  if (second(ct) > s)
                    t += days(1);
                }
              }
            }
          }
          else {
            if (hour(ct) > h) {
              t += days(1);
            }
            else if (hour(ct) == h) {
              if (minute(ct) > n) {
                t += days(1);
              }
              else if (minute(ct) == n) {
                if (second(ct) > s)
                  t += days(1);
              }
            }
          }
        }
        else {
          if (hour(ct) > 11 && !ampm) {
            if (hour(ct) > h + 12) {
              h += 12;
            }
            else if (hour(ct) == h + 12) {
              if (minute(ct) > n) {
                h += 12;
              }
              else if (minute(ct) == n) {
                if (second(ct) > s) {
                  h += 12;
                }
              }
            }
          }
          else {
            if (hour(ct) < h) {
              t -= days(1);
            }
            else if (hour(ct) == h) {
              if (minute(ct) < n) {
                t -= days(1);
              }
              else if (minute(ct) == n) {
                if (second(ct) < s)
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
    t = justDate(t);
    if (when == 3)
      t = makeTime(year(t), 1, 1);
    else if (when == 2)
      t = makeTime(year(t) + 1, 1, 1);
    else if (when == 1)
      t = makeTime(year(t) - 1, 1, 1);
    return t;
  }


  return nadt;
}



void time_from_tokens (vector<string>& tokens, int i, int& h, int& n, int& s)
{
  size_t j = (size_t) i;
  if (tokens.size() > j) {
    h = val(tokens[j]);
    if (h > 24 || h < 0)
      h = 0;
    if ((h > 0 || str_is_0(tokens[j])) && tokens.size() > j + 1) {
      if (!testAMPM(tokens[j + 1], h)) {
        n = val(tokens[j + 1]);
        if (n > 60 || n < 0)
          n = 0;
        if ((n > 0 || str_is_0(tokens[j + 1])) && tokens.size() > j + 2) {
          if (!testAMPM(tokens[j + 2], h)) {
            s = val(tokens[j + 2]);
            if (s > 60 || s < 0)
              s = 0;
            if (tokens.size() > j + 3)
              testAMPM(tokens[j + 3], h);
          }
        }
      }
    }
  }
}



int default_year (ptime t, int when, int m)
{
  return default_year(t, when, m, 1);
}
int default_year (ptime t, int when, int m, int d)
{
  return default_year(t, when, m, d, 0, 0, 0);
}
int default_year (ptime t, int when, int m, int d, int h, int n, int s)
{
  int y = year(t);
  if (when == 2 || when == 3) {
    if (month(t) > m)
        y += 1;
    else if (month(t) == m) {
      if (day(t) > d)
        y += 1;
      else if (day(t) == d) {
        if (hour(t) > h)
          y += 1;
        else if (hour(t) == h) {
          if (minute(t) > n)
            y += 1;
          else if (minute(t) == n) {
            if (second(t) > s)
              y += 1;
          }
        }
      }
    }
  }
  else {
    if (month(t) < m)
        y -= 1;
    else if (month(t) == m) {
      if (day(t) < d)
        y -= 1;
      else if (day(t) == d) {
        if (hour(t) < h)
          y -= 1;
        else if (hour(t) == h) {
          if (minute(t) < n)
            y -= 1;
          else if (minute(t) == n) {
            if (second(t) < s)
              y -= 1;
          }
        }
      }
    }
  }
  return y;
}



int weekday (string s)
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

int month (string s)
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

int daysInMonth (int m)
{
  return daysInMonth(m, 1);
}
int daysInMonth (int m, int y)
{
  if (m == 2)
    return 28 + isLeapYear(y);
  if (m == 4 || m == 6 || m == 9 || m == 11)
    return 30;
  return 31;
}
int daysInYear (int y)
{
  return 365 + isLeapYear(y);
}

bool isLeapYear (int y)
{
  return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}


int num_phrase_to_num (string s)
{
  s = uCase(removePunctuation(removeSpace(s)));
  if (s == "ZERO"        || s == "0"  || s == "ZEROTH"                     || s == "0TH" )
    return 0;
  if (s == "ONE"         || s == "1"  || s == "FIRST"                      || s == "1ST" )
    return 1;
  if (s == "TWO"         || s == "2"  || s == "SECOND"                     || s == "2ND" )
    return 2;
  if (s == "THREE"       || s == "3"  || s == "THIRD"                      || s == "3RD" )
    return 3;
  if (s == "FOUR"        || s == "4"  || s == "FOURTH"                     || s == "4TH" )
    return 4;
  if (s == "FIVE"        || s == "5"  || s == "FIFTH"                      || s == "5TH" )
    return 5;
  if (s == "SIX"         || s == "6"  || s == "SIXTH"                      || s == "6TH" )
    return 6;
  if (s == "SEVEN"       || s == "7"  || s == "SEVENTH"                    || s == "7TH" )
    return 7;
  if (s == "EIGHT"       || s == "8"  || s == "EIGHTH"                     || s == "8TH" )
    return 8;
  if (s == "NINE"        || s == "9"  || s == "NINTH"                      || s == "9TH" )
    return 9;
  if (s == "TEN"         || s == "10" || s == "TENTH"                      || s == "10TH")
    return 10;
  if (s == "ELEVEN"      || s == "11" || s == "ELEVENTH"                   || s == "11TH")
    return 11;
  if (s == "TWELVE"      || s == "12" || s == "TWELFTH" || s == "TWELVETH" || s == "12TH")
    return 12;
  if (s == "THIRTEEN"    || s == "13" || s == "THIRTEENTH"                 || s == "13TH")
    return 13;
  if (s == "FOURTEEN"    || s == "14" || s == "FOURTEENTH"                 || s == "14TH")
    return 14;
  if (s == "FIFTEEN"     || s == "15" || s == "FIFTEENTH"                  || s == "15TH")
    return 15;
  if (s == "SIXTEEN"     || s == "16" || s == "SIXTEENTH"                  || s == "16TH")
    return 16;
  if (s == "SEVENTEEN"   || s == "17" || s == "SEVENTEENTH"                || s == "17TH")
    return 17;
  if (s == "EIGHTEEN"    || s == "18" || s == "EIGHTEENTH"                 || s == "18TH")
    return 18;
  if (s == "NINETEEN"    || s == "19" || s == "NINTEENTH"                  || s == "19TH")
    return 19;
  if (s == "TWENTY"      || s == "20" || s == "TWENTIETH"                  || s == "20TH")
    return 20;
  if (s == "TWENTYONE"   || s == "21" || s == "TWENTYFIRST"                || s == "21ST")
    return 21;
  if (s == "TWENTYTWO"   || s == "22" || s == "TWENTYSECOND"               || s == "22ND")
    return 22;
  if (s == "TWENTYTHREE" || s == "23" || s == "TWENTYTHIRD"                || s == "23RD")
    return 23;
  if (s == "TWENTYFOUR"  || s == "24" || s == "TWENTYFOURTH"               || s == "24TH")
    return 24;
  if (s == "TWENTYFIVE"  || s == "25" || s == "TWENTYFIFTH"                || s == "25TH")
    return 25;
  if (s == "TWENTYSIX"   || s == "26" || s == "TWENTYSIXTH"                || s == "26TH")
    return 26;
  if (s == "TWENTYSEVEN" || s == "27" || s == "TWENTYSEVENTH"              || s == "27TH")
    return 27;
  if (s == "TWENTYEIGHT" || s == "28" || s == "TWENTYEIGHTH"               || s == "28TH")
    return 28;
  if (s == "TWENTYNINE"  || s == "29" || s == "TWENTYNINTH"                || s == "29TH")
    return 29;
  if (s == "THIRTY"      || s == "30" || s == "THIRTIETH"                  || s == "30TH")
    return 30;
  if (s == "THIRYONE"    || s == "31" || s == "THIRYFIRST"                 || s == "31ST")
    return 31;
  return -1;
}

int time_phrase_to_time (string s)
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












int second (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_sec;
}

int minute (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_min;
}

int hour (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_hour;
}

int day (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_mday;
}

int weekday (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_wday;
}

int month (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_mon + 1;
}

int year (ptime t)
{
  tm pt_tm = to_tm(t);
  return pt_tm.tm_year + 1900;
}


















time_duration timeBetween (ptime a, ptime b)
{
  time_duration c = b - a;
  return c;
}

ptime timeAfter (ptime a, ptime b)
{
  ptime c (a);
  c += years(year(b));
  c += months(month(b));
  c += days(day(b));
  c += hours(hour(b));
  c += minutes(minute(b));
  c += seconds(second(b));
  return c;
}

ptime timeBefore (ptime a, ptime b)
{
  ptime c (a);
  c -= years(year(b));
  c -= months(month(b));
  c -= days(day(b));
  c -= hours(hour(b));
  c -= minutes(minute(b));
  c -= seconds(second(b));
  return c;
}









ptime currentTime ()
{
  return second_clock::local_time();
}

ptime makeTime (int year, int month, int day)
{
  return makeTime(year, month, day, 0, 0, 0);
}
ptime makeTime (int year, int month, int day, int hour, int minute, int second)
{
  return makeTime(year, month, day, hour, minute, second, 0);
}
ptime makeTime (int year, int month, int day, int hour, int minute, int second, int millisecond)
{
  ptime t (date(year, month, day), time_duration(hour, minute, second, millisecond));
  return t;
}



ptime tm_to_ptime (tm tm)
{
  ptime t = makeTime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  return t;
}



ptime justDate(ptime t)
{
  tm pt_tm = to_tm(t);
  ptime d = makeTime(pt_tm.tm_year + 1900, pt_tm.tm_mon + 1, pt_tm.tm_mday, 0, 0, 0);
  return d;
}














string uCase (string s)
{
  boost::to_upper(s);
  return s;
}


int val (string s)
{
  int i = 0;
  size_t j;
  for (j = 0; j < s.length(); j ++)
    if (s.at(j) != '0')
      break;
  s = s.substr(j);
  istringstream(s) >> i;
  return i;
}




string removePunctuation (string text)
{
  for(size_t i = 0; i < text.length(); i ++)
    if(ispunct(text[i]))
      text.erase(i--, 1);
  return text;
}

string removeSpace (string text)
{
  for(size_t i = 0; i < text.length(); i ++)
    if(text[i] == ' ')
      text.erase(i--, 1);
  return text;
}

vector<string> tokenize (string s)
{
  return tokenize(s, false);
}
vector<string> tokenize (string s, bool punc)
{
  vector<string> tokens;
  if (!punc)
    s = removePunctuation(s);
  boost::algorithm::split(tokens, s, boost::is_any_of(" "));
  return tokens;
}

bool str_is_0 (string s)
{
  int    a = 0;
  double b = 0.0;
  for (size_t i = 0; i < s.length(); i ++)
    a += (int) s.at(i);
  b = (double) a / s.length();
  if (b == 48.0)
    return true;
  return false;
}

void ReplaceStringInPlace(string& subject, const string& search, const string& replace)
{
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

bool testAMPM (string& s, int& h)
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