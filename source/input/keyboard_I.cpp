////////////////////////////////////////////////////////////////////////////////
//File:   keyboard_I.cpp
//Dir:    source/input/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of KeyboardInput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>


//includes
#include "keyboard_I.h"
#include "settings.h"


//namespaces
using namespace std;


//Constructors

KeyboardInput::KeyboardInput ()
{
  KeyboardInput::name = "KEYBOARD";
  KeyboardInput::last = 0;
  KeyboardInput::pos = 0;
  KeyboardInput::console = true;
}


//Destructors

void KeyboardInput::free ()
{
  Input::~Input();
}


//Accessors

char KeyboardInput::getLast () const
{
  return KeyboardInput::last;
}


//Modifiers

void KeyboardInput::setLast (char last)
{
  KeyboardInput::last = last;
}


//Other Functions

void KeyboardInput::main ()
{
  KeyboardInput::init();
  char k;
  string b;

  do {
    DELAY_LOOP;
    hotkeys();
    if (KeyboardInput::active) {
      
      if (foreground_) {
        if (GetAsyncKeyState(0x26) == KEY_ACTIVE) { //down key
          KeyboardInput::pos -= 1;
          if (KeyboardInput::pos < 0) {
            KeyboardInput::pos = 0;
          }
          else {
            KeyboardInput::clearBuffer();
            KeyboardInput::updateBuffer(iLog_[KeyboardInput::pos]);
          }
        }
        if (GetAsyncKeyState(0x28) == KEY_ACTIVE) { //up key
          KeyboardInput::pos += 1;
          if (KeyboardInput::pos > (int) iLog_.size() - 1) {
            KeyboardInput::clearBuffer();
            KeyboardInput::pos = (int) iLog_.size();
          }
          else {
            KeyboardInput::clearBuffer();
            KeyboardInput::updateBuffer(iLog_[KeyboardInput::pos]);
          }
        }
      }
      if (KeyboardInput::collectInput()) {
        k = KeyboardInput::getLast();
        if (k > 0) {
          switch (k) {
          case 3:
            if (oLog_.size())
              clipboardOut(oLog_[oLog_.size() - 1]);
            break;
          case 8:
            b = KeyboardInput::getBuffer();
            b = b.substr(0, b.length() - 1);
            KeyboardInput::buffer = b;
            break;
          case 13:
            addInput(KeyboardInput::buffer);
            KeyboardInput::clearBuffer();
            KeyboardInput::pos = iLog_.size() + 1;
            break;
          case 22:
            KeyboardInput::updateBuffer(clipboardIn());
            break;
          case 25:
            if (oLog_.size())
              clipboardOut(oLog_[oLog_.size() - 1]);
            break;
          case 27:
            break;
          case 32:
            KeyboardInput::updateBuffer(" ");
            break;
          default:
            KeyboardInput::updateBuffer(str(k));
          }
        }
        KeyboardInput::done = true;
      }

    }
  } while (!terminate_);
  KeyboardInput::free();
}

void KeyboardInput::init ()
{
  if (!settings_.hasSettingGroup(SETTING_INPUT, "KEYBOARD"))
    KeyboardInput::defaultSettings();

  return;
}

void KeyboardInput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("KEYBOARD");
  sg.setType(SETTING_INPUT);
  sg.setFile(userLoc_ + "data/setting/input/keyboard.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

bool KeyboardInput::collectInput ()
{
  if (foreground_) {
    int k;
    k = INKEY();
    if (k > 0) {
      setLast(k);
      return true;
    }
  }
  return false;
}