////////////////////////////////////////////////////////////////////////////////
//File:   module.cpp
//Dir:    source/module/
//Date:   YYYY-MM-DD
//Author: Zachary Gill
//Implementation of MODULE module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//standard includes
#include <string>


//includes
#include "module.h"
#include "../header/module.h"


//namespaces
using namespace std;


//functions

string MODULE::main ()
{
  if (!settings_.hasSettingGroup("MODULE", SETTING_MODULE))
    MODULE::PRIVATE::defaultSettings();

  return "";
}





void MODULE::PRIVATE::defaultSettings()
{
  SettingGroup sg;
  sg.setName("MODULE");
  sg.setType(SETTING_MODULE);
  sg.setFile(userLoc_ + "data/setting/module/module.settings");
  settings_.addSettingGroup(sg);
}