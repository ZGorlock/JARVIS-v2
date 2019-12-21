////////////////////////////////////////////////////////////////////////////////
//File:   variable.cpp
//Dir:    source/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of Variable
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <string>
#include <vector>


//includes
#include "variable.h"


//namespaces
using namespace std;


//functions
bool hasVariable(string name)
{
  for (size_t i = 0; i < variableList_.size(); i++) {
    if (uCase(variableList_[i]) == uCase(name))
      return true;
  }
  return false;
}

bool isVariable(string s)
{
  if (s.substr(0, 1) == "%" && s.substr(s.length() - 1, 1) == "%" && s.length() == 5)
    return true;
  return false;
}