////////////////////////////////////////////////////////////////////////////////
//File:   module.h
//Dir:    source/module/
//Date:   YYYY-MM-DD
//Author: Zachary Gill
//Interface of MODULE module
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SM_MODULE_H_
#define _DLA_SM_MODULE_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include <vector>


//includes
#include "../header/module.h"
#include "../header/settings.h"


//namespace definitions
namespace MODULE
{
  std::string main       ();

  namespace PRIVATE
  {
    void defaultSettings ();
  } ;
} ;


//global variable definitions
extern bool                 terminate_;
extern std::string          userLoc_;
extern int                  moduleCount_;
extern std::vector<Module>  modules_;
extern Settings             settings_;


#endif