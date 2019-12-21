////////////////////////////////////////////////////////////////////////////////
//File:   settings.cpp
//Dir:    source/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of Settings
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>


//includes
#include "settings.h"


//namespaces
using namespace std;


//Constructors

Setting::Setting()
{
  Setting::name = "";
  Setting::value = "";
}
Setting::Setting(string name, bool value)
{
  Setting::name = name;
  if (value)
    Setting::value = "1";
  else
    Setting::value = "0";
}
Setting::Setting(string name, int value)
{
  Setting::name = name;
  Setting::value = str(value);
}
Setting::Setting(string name, double value)
{
  Setting::name = name;
  Setting::value = str(value);
}
Setting::Setting(string name, string value)
{
  Setting::name = name;
  Setting::value = value;
}

SettingGroup::SettingGroup()
{
  SettingGroup::name = "";
  SettingGroup::type = SETTING_NULL;
  SettingGroup::file = "";
}
SettingGroup::SettingGroup(int type, string file)
{
  if (!fileExists(file) || fileSize(file) == 0) {
    SettingGroup::name = "NULL";
    SettingGroup::file = "";
    return;
  }

  string tmpName;
  string tmpValue;
  
  SettingGroup::type = type;
  SettingGroup::file = file;

  fstream fs;
  fs.open(file, fstream::in);  

  getline(fs, SettingGroup::name);
  while (getline(fs, tmpName)) {
    getline(fs, tmpValue);
    Setting tmpSetting(tmpName, tmpValue);
    SettingGroup::settings.push_back(tmpSetting);
  }
  
  fs.close();
}

Settings::Settings()
{
}


//Destructors

Setting::~Setting()
{
}

SettingGroup::~SettingGroup()
{
}

Settings::~Settings()
{
}


//Accessors

string Setting::getName()
{
  return Setting::name;
}

string Setting::getValue()
{
  return Setting::value;
}

string SettingGroup::getName()
{
  return SettingGroup::name;
}

int SettingGroup::getType()
{
  return SettingGroup::type;
}

string SettingGroup::getFile()
{
  return SettingGroup::file;
}

vector<Setting> SettingGroup::getSettings()
{
  return SettingGroup::settings;
}

vector<SettingGroup> Settings::getSettingGroups()
{
  return Settings::settingGroups;
}


//Modifiers

void Setting::setName(string name)
{
  Setting::name = name;
}

void Setting::setValue(bool value)
{
  if (value)
    Setting::value = "1";
  else
    Setting::value = "0";
}
void Setting::setValue(int value)
{
  Setting::value = str(value);
}
void Setting::setValue(double value)
{
  Setting::value = str(value);
}
void Setting::setValue(string value)
{
  Setting::value = value;
}

void SettingGroup::setName(string name)
{
  SettingGroup::name = name;
}

void SettingGroup::setType(int type)
{
  SettingGroup::type = type;
}

void SettingGroup::setFile(string file)
{
  SettingGroup::file = file;
}


//Other Functions

void Setting::dump()
{
  Setting::dump(cout);
}
void Setting::dump(ostream& stream)
{
  Setting::dump(stream, "");
}
void Setting::dump(ostream& stream, string prefix)
{
  stream << prefix << uCase(Setting::getName())
         << ": " << Setting::getValue() << endl;
}

Setting SettingGroup::getSetting(string name)
{
  for (size_t i = 0; i < SettingGroup::settings.size(); i++) {
    if (uCase(SettingGroup::settings[i].getName()) == uCase(name))
      return SettingGroup::settings[i];
  }
  Setting tmpSetting;
  return tmpSetting;
}

void SettingGroup::setSetting(string name, bool value)
{
  for (size_t i = 0; i < SettingGroup::getSettings().size(); i++) {
    if (SettingGroup::settings[i].getName() == name) {
      SettingGroup::settings[i].setValue(value);
    }
  }
}
void SettingGroup::setSetting(string name, int value)
{
  for (size_t i = 0; i < SettingGroup::getSettings().size(); i++) {
    if (SettingGroup::settings[i].getName() == name) {
      SettingGroup::settings[i].setValue(value);
    }
  }
}
void SettingGroup::setSetting(string name, double value)
{
  for (size_t i = 0; i < SettingGroup::getSettings().size(); i++) {
    if (SettingGroup::settings[i].getName() == name) {
      SettingGroup::settings[i].setValue(value);
    }
  }
}
void SettingGroup::setSetting(string name, string value)
{
  for (size_t i = 0; i < SettingGroup::getSettings().size(); i++) {
    if (SettingGroup::settings[i].getName() == name) {
      SettingGroup::settings[i].setValue(value);
    }
  }
}

void SettingGroup::addSetting(Setting s)
{
  SettingGroup::settings.push_back(s);
}

void SettingGroup::dump()
{
  SettingGroup::dump(cout);
}
void SettingGroup::dump(ostream& stream)
{
  SettingGroup::dump(stream, "");
}
void SettingGroup::dump(ostream& stream, string prefix)
{
  if (SettingGroup::settings.size() > 0) {
    stream << prefix << uCase(SettingGroup::getName()) << ":" << endl;
    for (size_t i = 0; i < SettingGroup::settings.size(); i++)
      SettingGroup::settings[i].dump(stream, prefix + "  ");
  }
}

void SettingGroup::save()
{
  fstream fs;
  fs.open(SettingGroup::getFile(), fstream::out);

  fs << SettingGroup::getName() << endl;
  for (size_t i = 0; i < SettingGroup::settings.size(); i++)
    fs << SettingGroup::settings[i].getName()  << endl
       << SettingGroup::settings[i].getValue() << endl;

  fs.close();
}

SettingGroup Settings::getSettingGroup(int type, string name)
{
  for (size_t i = 0; i < Settings::settingGroups.size(); i++) {
    if (uCase(Settings::settingGroups[i].getName()) == uCase(name) && Settings::settingGroups[i].getType() == type)
        return Settings::settingGroups[i];
  }
  SettingGroup tmpSettingGroup;
  return tmpSettingGroup;
}

string Settings::getSetting(int type, string group, string name)
{
  if (Settings::hasSetting(type, group, name))
    return Settings::getSettingGroup(type, group).getSetting(name).getValue();
  return 0;
}

void Settings::setSetting(int type, string group, string name, bool value)
{
  if (Settings::hasSetting(type, group, name)) {
    for (size_t i = 0; i < Settings::getSettingGroups().size(); i++) {
      if (Settings::settingGroups[i].getName() == group && Settings::settingGroups[i].getType() == type) {
        Settings::settingGroups[i].setSetting(name, value);
      }
    }
  }
}
void Settings::setSetting(int type, string group, string name, int value)
{
  if (Settings::hasSetting(type, group, name)) {
    for (size_t i = 0; i < Settings::getSettingGroups().size(); i++) {
      if (Settings::settingGroups[i].getName() == group && Settings::settingGroups[i].getType() == type) {
        Settings::settingGroups[i].setSetting(name, value);
      }
    }
  }
}
void Settings::setSetting(int type, string group, string name, double value)
{
  if (Settings::hasSetting(type, group, name)) {
    for (size_t i = 0; i < Settings::getSettingGroups().size(); i++) {
      if (Settings::settingGroups[i].getName() == group && Settings::settingGroups[i].getType() == type) {
        Settings::settingGroups[i].setSetting(name, value);
      }
    }
  }
}
void Settings::setSetting(int type, string group, string name, string value)
{
  if (Settings::hasSetting(type, group, name)) {
    for (size_t i = 0; i < Settings::getSettingGroups().size(); i++) {
      if (Settings::settingGroups[i].getName() == group && Settings::settingGroups[i].getType() == type) {
        Settings::settingGroups[i].setSetting(name, value);
      }
    }
  }
}

bool Settings::getSettingB(int type, string group, string name)
{
  if (Settings::hasSetting(type, group, name))
    return (val(Settings::getSettingGroup(type, group).getSetting(name).getValue()) != 0);
  return false;
}

int Settings::getSettingI(int type, string group, string name)
{
  if (Settings::hasSetting(type, group, name))
    return val(Settings::getSettingGroup(type, group).getSetting(name).getValue());
  return 0;
}

double Settings::getSettingD(int type, string group, string name)
{
  if (Settings::hasSetting(type, group, name))
    return vald(Settings::getSettingGroup(type, group).getSetting(name).getValue());
  return 0.0;
}

string Settings::getSettingS(int type, string group, string name)
{
  if (Settings::hasSetting(type, group, name))
    return Settings::getSettingGroup(type, group).getSetting(name).getValue();
  return "";
}

void Settings::addSettingGroup(SettingGroup sg)
{
  Settings::settingGroups.push_back(sg);
}


void Settings::clear()
{
  for (size_t i = 0; i < Settings::getSettingGroups().size(); i++)
    Settings::getSettingGroups()[i].~SettingGroup();
  Settings::getSettingGroups().clear();
}


void Settings::dump()
{
  Settings::dump(cout);
}
void Settings::dump(ostream& stream)
{
  Settings::dump(stream, "");
}
void Settings::dump(ostream& stream, string prefix)
{
  stream << prefix << "::SETTINGS::" << endl;
  for (size_t i = 0; i < Settings::settingGroups.size(); i++) {
    if (Settings::settingGroups[i].getType() == SETTING_METRIC)
      Settings::settingGroups[i].dump(stream, prefix + "  ");
  }
  
  stream << prefix << "::MODULES::" << endl;
  for (size_t i = 0; i < Settings::settingGroups.size(); i++) {
    if (Settings::settingGroups[i].getType() == SETTING_MODULE) {
      Settings::settingGroups[i].dump(stream, prefix + "  ");
    }
  }
  
  stream << prefix << "::INPUTS::" << endl;
  for (size_t i = 0; i < Settings::settingGroups.size(); i++) {
    if (Settings::settingGroups[i].getType() == SETTING_INPUT) {
      Settings::settingGroups[i].dump(stream, prefix + "  ");
    }
  }
  
  stream << prefix << "::OUTPUTS::" << endl;
  for (size_t i = 0; i < Settings::settingGroups.size(); i++) {
    if (Settings::settingGroups[i].getType() == SETTING_OUTPUT) {
      Settings::settingGroups[i].dump(stream, prefix + "  ");
    }
  }
}

void Settings::save()
{
  for (size_t i = 0; i < Settings::settingGroups.size(); i++)
    Settings::settingGroups[i].save();
}

void Settings::load()
{
  Settings::clear();

  string file = "";

  file = userLoc_ + "data/setting/settings.settings";
  if (fileExists(file) && fileSize(file) > 0) {
    SettingGroup metric(SETTING_METRIC, file);
    Settings::addSettingGroup(metric);
  }

  for (int i = 0; i < moduleCount_; i++) {
    file = userLoc_ + "data/setting/module/" + lCase(moduleList_[i]) + ".settings";
    if (fileExists(file) && fileSize(file) > 0) {
      SettingGroup module(SETTING_MODULE, file);
      Settings::addSettingGroup(module);
    }
  }
  
  for (int i = 0; i < inputCount_; i++) {
    file = userLoc_ + "data/setting/input/" + lCase(inputList_[i]) + ".settings";
    if (fileExists(file) && fileSize(file) > 0) {
      SettingGroup input(SETTING_INPUT, file);
      Settings::addSettingGroup(input);
    }
  }
  
  for (int i = 0; i < outputCount_; i++) {
    file = userLoc_ + "data/setting/output/" + lCase(outputList_[i]) + ".settings";
    if (fileExists(file) && fileSize(file) > 0) {
      SettingGroup output(SETTING_OUTPUT, file);
      Settings::addSettingGroup(output);
    }
  }
}

void Settings::loadDefault()
{
  Settings::clear();

  string file = "";

  file = "data/setting/settings.settings";
  if (fileExists(file) && fileSize(file) > 0) {
    SettingGroup metric(SETTING_METRIC, file);
    Settings::addSettingGroup(metric);
  }

  for (int i = 0; i < moduleCount_; i++) {
    file = "data/setting/module/" + lCase(moduleList_[i]) + ".settings";
    if (fileExists(file) && fileSize(file) > 0) {
      SettingGroup module(SETTING_MODULE, file);
      Settings::addSettingGroup(module);
    }
  }
  
  for (int i = 0; i < inputCount_; i++) {
    file = "data/setting/input/" + lCase(inputList_[i]) + ".settings";
    if (fileExists(file) && fileSize(file) > 0) {
      SettingGroup input(SETTING_INPUT, file);
      Settings::addSettingGroup(input);
    }
  }
  
  for (int i = 0; i < outputCount_; i++) {
    file = "data/setting/output/" + lCase(outputList_[i]) + ".settings";
    if (fileExists(file) && fileSize(file) > 0) {
      SettingGroup output(SETTING_OUTPUT, file);
      Settings::addSettingGroup(output);
    }
  }
}

bool Settings::hasSettingGroup(int type, string name)
{
  for (size_t i = 0; i < Settings::settingGroups.size(); i++) {
    if (uCase(Settings::settingGroups[i].getName()) == uCase(name) && Settings::settingGroups[i].getType() == type)
      return true;
  }
  return false;
}

bool Settings::hasSetting(int type, string group, string name)
{
  if (Settings::hasSettingGroup(type, group)) {
    for (size_t i = 0; i < Settings::getSettingGroup(type, group).getSettings().size(); i++) {
      if (uCase(Settings::getSettingGroup(type, group).getSettings()[i].getName()) == uCase(name))
        return true;
    }
  }
  return false;
}