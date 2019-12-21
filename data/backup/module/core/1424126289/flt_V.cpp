////////////////////////////////////////////////////////////////////////////////
//File:   flt_V.cpp
//Dir:    source/variable/
//Date:   2014-12-31
//Author: Zachary Gill
//Implementation of FLT variable
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <string>


//includes
#include "flt_V.h"


//namespaces
using namespace std;


//functions
double isFLT (string s)
{
  const double maxScore = 150.0;
  
  double score = 0.0;
  int value = val(s);
  float valueF = (float)vald(s);
  string restr = str(valueF);
  if (value > 0 OR str_is_0(s)) {
    score += 20.0;
    if ((float)value != valueF)
      score += 30.0;
      int sizeDif = abs((int)(restr.length() - s.length()));
    if (sizeDif == 0)
      score += 50.0;
    else
      score += 25.0 / sizeDif;
  }

  int badChars = 0;
  for (size_t i = 0; i < s.length(); i++) {
    if (NOT( ((s[i] >= '0') AND (s[i] <= '9')) OR (s[i] == '.') ))
      badChars++;
  }
  if (badChars == 0)
    score += 50.0;
  else
    score += 10 * (1 - ((double)badChars / s.length()));
  return score / maxScore;
}