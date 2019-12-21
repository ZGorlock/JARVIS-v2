////////////////////////////////////////////////////////////////////////////////
//File:   output.cpp
//Dir:    source/
//Date:   2014-03-31
//Author: Zachary Gill
//Implementation of Output
////////////////////////////////////////////////////////////////////////////////


//constants
#include "resource/const.h"


//standard includes
#include <string>


//includes
#include "header/output.h"


//namespaces
using namespace std;


//Constructors

Output::Output()
{
  Output::name   = "";
  Output::type   = 0;
  
  outputCount_ ++;
}
Output::Output(string name, int type)
{
  Output::name   = name;
  Output::type   = type;
  
  outputCount_ ++;
}


//Destructors

Output::~Output()
{
}

void Output::free()
{
  Output::~Output();
  outputCount_ --;
}


//Accessors

string Output::getName() const
{
  return Output::name;
}

int Output::getType() const
{
  return Output::type;
}


//Other Functions

void Output::init()
{
}

void Output::putOutput()
{
}


//functions
void loadOutputs()
{
}