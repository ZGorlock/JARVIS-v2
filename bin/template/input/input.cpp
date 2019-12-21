////////////////////////////////////////////////////////////////////////////////
//File:   input.cpp
//Dir:    source/input/
//Date:   YYYY-MM-DD
//Author: Zachary Gill
//Implementation of InputInput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//standard includes


//includes
#include "input.h"
#include "../header/settings.h"


//namespaces
using namespace std;


//Constructors

InputInput::InputInput ()
{
  InputInput::name = "INPUT";
}


//Destructors

void InputInput::free ()
{
  Input::~Input();
}


//Other Functions

void InputInput::main ()
{
  InputInput::init();

  do {
    DELAY_LOOP;
    if (InputInput::active) {
      if (InputInput::collectInput()) {
        InputInput::done = false;
        //process input
        InputInput::done = true;
      }
    }
  } while (!terminate_);
  InputInput::free();
}

void InputInput::init ()
{
  if (!settings_.hasSettingGroup("INPUT", SETTING_INPUT))
    InputInput::defaultSettings();

  return;
}

void InputInput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("INPUT");
  sg.setType(SETTING_INPUT);
  sg.setFile(userLoc_ + "data/setting/input/input.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

bool InputInput::collectInput ()
{
  if (foreground_) {
    //collect input
    //if found input
      //return true;
  }
  return false;
}