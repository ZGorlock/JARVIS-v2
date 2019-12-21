////////////////////////////////////////////////////////////////////////////////
//File:   log_O.cpp
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of LogOutput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


//includes
#include "log_O.h"
#include "module.h"
#include "settings.h"
#include "user.h"
#include "../module/time_M.h"


//namespaces
using namespace std;


//Constructors

LogOutput::LogOutput()
{
  LogOutput::name = "LOG";
}


//Destructors

inline void LogOutput::free()
{
  TextOutput::file.close();
  Output::~Output();
}


//Other Functions

void LogOutput::main()
{
  LogOutput::init();
  do {
    DELAY_LOOP;
    if (LogOutput::active) {

      if (LogOutput::collectOutput()) {
        string b = getBuffer();
        stringstream ss(b);
        string s;
        getline(ss, s, '\n');
        TextOutput::file << s << endl;
        while (getline(ss, s, '\n'))
          TextOutput::file << "              " << s << endl;
        clearBuffer();
        LogOutput::done = true;
      }

    }
  } while (!terminate_);
  LogOutput::free();
}

inline void LogOutput::init()
{
  if (!settings_.hasSettingGroup(SETTING_OUTPUT, "LOG"))
    LogOutput::defaultSettings();

  TextOutput::file.open(user_.getLoc() + "log.log", fstream::app);
  TextOutput::file << "--------------------------------------------------------------------------------" << endl;
}

inline void LogOutput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("LOG");
  sg.setType(SETTING_OUTPUT);
  sg.setFile(userLoc_ + "data/setting/output/log.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

inline bool LogOutput::collectOutput()
{
  if (log_ > "") {
    LogOutput::done = false;
    if (log_ == "::clear::") {
      TextOutput::file.close();
      TextOutput::file.open(user_.getLoc() + "log.log", fstream::out);
      TextOutput::file.close();
      TextOutput::file.open(user_.getLoc() + "log.log", fstream::app);
      TextOutput::file << "--------------------------------------------------------------------------------";
    }
    else {
      if (hasModule("TIME") && settings_.hasSettingGroup(SETTING_MODULE, "TIME"))
        setBuffer(TIME::timestamp() + log_);
      else
        setBuffer(log_);
    }
    log_ = "";
    return true;
  }
  return false;
}