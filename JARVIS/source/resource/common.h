////////////////////////////////////////////////////////////////////////////////
//File:   common.h
//Dir:    source/resource/
//Date:   2014-03-19
//Author: Zachary Gill
//Common functions
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SR_COMMON_H_
#define _JARVIS_SR_COMMON_H_


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>


//namespaces
using namespace std;


//function definitions
bool fileExists(string name);
void getint(fstream& fs, int& outVar);
void getdouble(fstream& fs, double& outVar);


//functions

bool fileExists(string name)
{
  fstream file(name.c_str());
  if(!file)
    return false;
  else
    return true;
}

void getint(fstream& fs, int& outVar)
{
  if (fs.is_open())
  {
    string tmp;
    fs >> outVar;
    getline(fs, tmp);
  }
}

void getdouble(fstream& fs, double& outVar)
{
  if (fs.is_open())
  {
    string tmp;
    fs >> outVar;
    getline(fs, tmp);
  }
}


#endif