////////////////////////////////////////////////////////////////////////////////
//File:   bol_V.cpp
//Dir:    source/variable/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of BOL variable
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <string>


//includes
#include "bol_V.h"


//namespaces
using namespace std;


//functions
double isBOL(string s)
{
  const double maxScore = 100.0;

  double score = 0.0;
  if (isNo(s) || isYes(s))
    score += 100.0;
  return score / maxScore;
}