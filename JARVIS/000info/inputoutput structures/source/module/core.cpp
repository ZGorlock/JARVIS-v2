////////////////////////////////////////////////////////////////////////////////
//File:   core.cpp
//Dir:    source/module/
//Date:   2014-03-30
//Author: Zachary Gill
//Implementation of CORE module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//includes
#include "core.h"


//namespaces
using namespace std;


//functions

string CORE::main()
{
  return "";
}

string CORE::clear()
{
  cls(h_);
  cout << "JARVIS" << endl << endl;
  return "";
}

string CORE::log()
{
  ShellExecute(hWnd_, "open", "log.log", NULL, "data", SW_SHOW);
  return "opening the log";
}

string CORE::clearLog()
{
  log_.close();
  log_.open("data/log.log", fstream::out);
  log_.close();
  log_.open("data/log.log", fstream::app);
  return "clearing the log";
}

string CORE::terminate()
{
  terminate_ = true;
  return "";
}