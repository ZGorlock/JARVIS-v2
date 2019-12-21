////////////////////////////////////////////////////////////////////////////////
//File:   chr_V.cpp
//Dir:    source/variable/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of CHR variable
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <string>


//includes
#include "chr_V.h"


//namespaces
using namespace std;


//functions
double isCHR (string s)
{
  const double maxScore = 100.0;

  double score = 0.0;
  if (!(val(s) != 0 || str_is_0(s)))
    score += 20.0;
  score += 80.0 * (1 / s.length());
  return score / maxScore;
}