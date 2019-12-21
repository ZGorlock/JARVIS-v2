////////////////////////////////////////////////////////////////////////////////
//File:   settings.cpp
//Dir:    source/
//Date:   2014-03-29
//Author: Zachary Gill
//Implementation of Settings
////////////////////////////////////////////////////////////////////////////////


//constants
#include "resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>


//includes
#include "header/settings.h"


//namespaces
using namespace std;


//Constructors

Settings::Settings()
{
  CLOCK_UNIVERSAL = false;
  CLOCK_MILITARY =  false;
  CLOCK_ACCURACY =  false;
}


//Deconstructors

Settings::~Settings()
{
}


//Other Functions
void Settings::save()
{
  Settings::save("data/settings.ini");
}
void Settings::save(string settings)
{
  fstream settingsFile;
  settingsFile.open(settings.c_str(), fstream::out);

  if (settingsFile.is_open()) {
    settingsFile << Settings::CLOCK_UNIVERSAL << endl
                 << Settings::CLOCK_MILITARY  << endl
                 << Settings::CLOCK_ACCURACY  << endl;

    settingsFile.close();
  }
}

void Settings::load()
{
  Settings::load("data/settings.ini");
}
void Settings::load(string settings)
{
  if (fileExists(settings)) {
    fstream settingsFile;
    settingsFile.open(settings.c_str(), fstream::in);

    if (settingsFile.is_open()) {
      getbool (settingsFile, Settings::CLOCK_UNIVERSAL);
      getbool (settingsFile, Settings::CLOCK_MILITARY);
      getbool (settingsFile, Settings::CLOCK_ACCURACY);

      settingsFile.close();
    }
  }
}

void Settings::reset()
{
  Settings::CLOCK_UNIVERSAL = false;
  Settings::CLOCK_MILITARY =  false;
  Settings::CLOCK_ACCURACY =  false;
}

void Settings::dump()
{
  Settings::dump(cout);
}
void Settings::dump(ostream& stream)
{
  stream << ":::SETTINGS:::"                                       << endl
         << "CLOCK: "                                              << endl
         << "  Military Time:        " << Settings::CLOCK_MILITARY  << endl
         << "  Universal Time:       " << Settings::CLOCK_UNIVERSAL << endl
         << "  Microsecond Accuracy: " << Settings::CLOCK_ACCURACY  << endl;
}


//functions
void saveSettings()
{
  settings_.save();
}
void saveSettings(string settings)
{
  settings_.save(settings);
}

void loadSettings()
{
  settings_.load();
}
void loadSettings(string settings)
{
  settings_.load(settings);
}
