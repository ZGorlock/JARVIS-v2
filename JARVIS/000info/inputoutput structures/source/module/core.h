////////////////////////////////////////////////////////////////////////////////
//File:   core.h
//Dir:    source/module/
//Date:   2014-03-30
//Author: Zachary Gill
//Interface of CORE module
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SM_CORE_H_
#define _JARVIS_SM_CORE_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>


//includes
#include "../header/settings.h"


//namespace definitions
namespace CORE
{
  std::string main();

  std::string clear();
  std::string log();
  std::string clearLog();
  std::string terminate();
} ;


//global variable definitions
extern bool         terminate_;
extern HANDLE       h_;
extern HWND         hWnd_;
extern std::fstream log_;
extern Settings     settings_;


#endif