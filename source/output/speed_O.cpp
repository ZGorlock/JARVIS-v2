////////////////////////////////////////////////////////////////////////////////
//File:   speed_O.cpp
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of SpeedOutput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <string>
#include <vector>
#include <ctime>


//includes
#include "speed_O.h"
#include "settings.h"

#include "timer.h"


//namespaces
using namespace std;


//Constructors

SpeedOutput::SpeedOutput()
{
  SpeedOutput::name = "SPEED";
  SpeedOutput::console = true;
}


//Destructors

inline void SpeedOutput::free()
{
  Output::~Output();
}


//Other Functions

void SpeedOutput::main()
{
  SpeedOutput::init();

  do {
    DELAY_LOOP;
    if (SpeedOutput::active) {

      SpeedOutput::collectOutput();
      TextOutput::done = true;

    }
  } while (!terminate_);
  SpeedOutput::free();
}

inline void SpeedOutput::init()
{
  if (!settings_.hasSettingGroup(SETTING_OUTPUT, "SPEED"))
    SpeedOutput::defaultSettings();
}

inline void SpeedOutput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("SPEED");
  sg.setType(SETTING_OUTPUT);
  sg.setFile(userLoc_ + "data/setting/output/speed.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

inline bool SpeedOutput::collectOutput()
{
  if (request_ > "" && iLog_.size() != SpeedOutput::oldILogSize) {
    SpeedOutput::t.start();
    SpeedOutput::oldILogSize = iLog_.size();
  }
  if (response_ > "" && oLog_.size() != SpeedOutput::oldOLogSize) {
    TextOutput::clearBuffer();
    SpeedOutput::oldOLogSize = oLog_.size();
    if (SpeedOutput::t.begin > -1) {
      SpeedOutput::t.stop();
      string speed = "in " + str(t.time()) + "s";
      TextOutput::setBuffer(speed);
      Output::done = false;
    }
    return true;
  }
  return false;
}