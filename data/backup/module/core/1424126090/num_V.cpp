////////////////////////////////////////////////////////////////////////////////
//File:   num_V.cpp
//Dir:    source/variable/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of NUM variable
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <string>
#include <cmath>


//includes
#include "num_V.h"
#include "int_v.h"
#include "flt_v.h"
#include "dbl_v.h"



//namespaces
using namespace std;


//functions
double isNUM (string s)
{
  return max(max(isINT(s), isFLT(s)), isDBL(s));
}