////////////////////////////////////////////////////////////////////////////////
//File:   output.cpp
//Dir:    source/output/
//Date:   YYYY-MM-DD
//Author: Zachary Gill
//Implementation of OutputOutput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//standard includes


//includes
#include "output.h"
#include "../header/settings.h"


//namespaces
using namespace std;


//Constructors

OutputOutput::OutputOutput ()
{
  OutputOutput::name = "OUTPUT";
}


//Destructors

void OutputOutput::free ()
{
  Output::~Output();
}


//Other Functions

void OutputOutput::main ()
{
  OutputOutput::init();

  do {
    DELAY_LOOP;
    if (OutputOutput::active) {
      if (OutputOutput::collectOutput()) {
        OutputOutput::done = false;
        //process input
        OutputOutput::done = true;
      }
    }
  } while (!terminate_);
  OutputOutput::free();
}

void OutputOutput::init ()
{
  if (!settings_.hasSettingGroup("OUTPUT", SETTING_OUTPUT))
    OutputOutput::defaultSettings();

  return;
}

void OutputOutput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("OUTPUT");
  sg.setType(SETTING_OUTPUT);
  sg.setFile(userLoc_ + "data/setting/output/output.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

bool OutputOutput::collectOutput ()
{
  //collect output
  //if found output
    //return true;
  }
  return false;
}