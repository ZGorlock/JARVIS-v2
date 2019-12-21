////////////////////////////////////////////////////////////////////////////////
//File:   core_M.cpp
//Dir:    source/module/
//Date:   2015-01-23
//Author: Zachary Gill
//Implementation of CORE module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <sstream>
#include <string>
#include <vector>


//includes
#include "core_M.h"
#include "user.h"
#include "module.h"
#include "input.h"
#include "output.h"
#include "variable.h"


//namespaces
using namespace std;


//functions

string CORE::main ()
{
  if (!settings_.hasSettingGroup(SETTING_MODULE, "CORE"))
    CORE::PRIVATE::defaultSettings();

  CORE::PRIVATE::loadResources();

  return "";
}

string CORE::sayhello()
{
  return "hello";
}

string CORE::add()
{
  if (variableCount_ == 2) {
    double result = 0;
    
    result += vald(variables_[0]);
    result += vald(variables_[1]);
    
    setOPriority("CORE", 0.3);
    return variables_[0] + " + " + variables_[1] + " is " + str(result);
  }
  return "";
}

string CORE::clear ()
{
  cls(hStdout_);
  return "";
}

string CORE::enumSettings ()
{
  stringstream ss;
  string s;
  string l;
  settings_.dump(ss);
  getline(ss, l, '\n');
  while (l > "") {
    s += l;
    getline(ss, l, '\n');
    if (l > "")
      s += "\n  ";
  }
  setOPriority("CORE", 0.0);
  return s;
}

string CORE::resetSettings ()
{
  settings_.loadDefault();
  setOPriority("CORE", 1.0);
  return "settings reset to default";
}

string CORE::revertSettings ()
{
  settings_.load();
  setOPriority("CORE", 1.0);
  return "reloading settings";
}

string CORE::log ()
{
  string loc = userLoc_;
  ReplaceStringInPlace(loc, "/", "\\");
  ShellExecute(hWnd_, "open", "log.log", NULL, loc.c_str(), SW_SHOW);
  setOPriority("CORE", 0.5);
  return "opening the log";
}

string CORE::clearLog ()
{
  log_ = "::clear::";
  if (hasOutput("LOG"))
  while (log_ > "") {
    DELAY_LOOP;
  }
  setOPriority("CORE", 0.5);
  return "clearing the log";
}

string CORE::enumModules ()
{
  string s = "";
  for (int i = 0; i < moduleCount_; i++) {
    s += modules_[i].getName();
    if (i != moduleCount_ - 1)
      s += "\n  ";
  }
  setOPriority("CORE", 0.0);
  return s;
}

string CORE::countModules ()
{
  setOPriority("CORE", 0.0);
  if (moduleCount_ == 1)
    return "there is " + str(moduleCount_) + " module installed";
  else
    return "there are " + str(moduleCount_) + " modules installed";
}

string CORE::enumCommands ()
{
  string s = "";
  for (int i = 0; i < moduleCount_; i ++) {
    s += modules_[i].getName();
    for (int j = 0; j < modules_[i].getCommandCount(); j++)
      s += "\n    " + modules_[i].getCommands()[j].getName();
    if (i != moduleCount_ - 1)
      s += "\n  ";
  }
  setOPriority("CORE", 0.0);
  return s;
}

string CORE::countCommands ()
{
  int commandCount = 0;
  for (int i = 0; i < moduleCount_; i ++)
    commandCount += modules_[i].getCommandCount();
  setOPriority("CORE", 0.0);
  if (commandCount == 1)
    return "there is " + str(commandCount) + " command available";
  else
    return "there are " + str(commandCount) + " commands available";
}

string CORE::enumInputs()
{
  string s = "";
  for (int i = 0; i < inputCount_; i++) {
    s += inputs_[i]->getName();
    if (i != inputCount_ - 1)
      s += "\n  ";
  }
  setOPriority("CORE", 0.0);
  return s;
}

string CORE::countInputs()
{
  setOPriority("CORE", 0.0);
  if (inputCount_ == 1)
    return "there is " + str(inputCount_) + " input installed";
  else
    return "there are " + str(inputCount_) + " inputs installed";
}

string CORE::enumOutputs()
{
  string s = "";
  for (int i = 0; i < outputCount_; i++) {
    s += outputs_[i]->getName();
    if (i != outputCount_ - 1)
      s += "\n  ";
  }
  setOPriority("CORE", 0.0);
  return s;
}

string CORE::countOutputs()
{
  setOPriority("CORE", 0.0);
  if (outputCount_ == 1)
    return "there is " + str(outputCount_) + " output installed";
  else
    return "there are " + str(outputCount_) + " outputs installed";
}

string CORE::hide()
{
  if (!hidden_) {
    hide_ = true;
    return "hiding the console";
  }
  return "";
}

string CORE::show()
{
  if (hidden_) {
    hide_ = false;
    return "showing the console";
  }
  return "";
}

string CORE::terminate ()
{
  terminate_ = true;
  return "";
}





void CORE::PRIVATE::defaultSettings()
{
  SettingGroup sg;
  sg.setName("CORE");
  sg.setType(SETTING_MODULE);
  sg.setFile(userLoc_ + "data/setting/module/core.settings");
  settings_.addSettingGroup(sg);
  return;
}


void CORE::PRIVATE::loadResources()
{
  return;
}