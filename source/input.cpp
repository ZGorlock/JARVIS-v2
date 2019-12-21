////////////////////////////////////////////////////////////////////////////////
//File:   input.cpp
//Dir:    source/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of Input
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>


//includes
#include "input.h"


//namespaces
using namespace std;


//Constructors

Input::Input()
{
  Input::type = INPUT_INPUT;
  Input::name = "";
  Input::active = true;
  Input::console = false;
  Input::done = true;

  inputCount_++;
}

TextInput::TextInput()
{
  TextInput::type = INPUT_TEXT;
  TextInput::buffer = "";
}

AudioInput::AudioInput()
{
  AudioInput::type = INPUT_AUDIO;
}

GraphicInput::GraphicInput()
{
  GraphicInput::type = INPUT_GRAPHIC;
  GraphicInput::graphic = 0;
}


//Destructors

Input::~Input()
{
}

inline void Input::free()
{
  Input::~Input();
  inputCount_--;
}


//Accessors

int Input::getType() const
{
  return Input::type;
}

string Input::getName() const
{
  return Input::name;
}

bool Input::getActive() const
{
  return Input::active;
}

bool Input::getConsole() const
{
  return Input::console;
}

string TextInput::getBuffer() const
{
  return TextInput::buffer;
}

int* GraphicInput::getGraphic() const
{
  return GraphicInput::graphic;
}


//Modifiers

void Input::setType(int type)
{
  Input::type = type;
}

void Input::setName(string name)
{
  Input::name = name;
}

void TextInput::updateBuffer(string update)
{
  TextInput::buffer += update;
}

void TextInput::clearBuffer()
{
  TextInput::buffer = "";
}

void TextInput::setBuffer(string buffer)
{
  TextInput::buffer = buffer;
}

void GraphicInput::setGraphic(int* graphic)
{
  GraphicInput::graphic = graphic;
}


//Other Functions

void Input::main()
{
  return;
}

inline void Input::init()
{
  return;
}

inline void Input::defaultSettings()
{
  return;
}

inline bool Input::collectInput()
{
  return false;
}

void Input::start()
{
  Input::active = true;
}

void Input::pause()
{
  Input::active = false;
}


//functions
void loadInputs()
{
  loadInputs("");
}
void loadInputs(string username)
{
  if (username > "")
    loadFileS("data/user/" + username + "/data/inputs", inputList_);
  else
    loadFileS("data/inputs", inputList_);
  for (size_t i = 0; i < inputList_.size(); i++)
    inputList_[i] = uCase(inputList_[i]);
  
  #include "../data/input/ptrs"
}

bool isInput(string name, vector<string>& inputList)
{
  name = uCase(name);
  for (size_t i = 0; i < inputList.size(); i++) {
    if (name == inputList[i])
      return true;
  }
  return false;
}

bool hasInput(string name)
{
  for (int i = 0; i < inputCount_; i++)
  if (inputs_[i]->getName() == name)
    return true;
  return false;
}

void hotkeys()
{
  if (foreground_) {
    //Escape
    if (GetAsyncKeyState(0x1B) == KEY_ACTIVE)
      terminate_ = true;

    //Help/F1
    if ((GetAsyncKeyState(0x70) == KEY_ACTIVE) || (GetAsyncKeyState(0x2F) == KEY_ACTIVE))
      help_ = true;
  }
}


void addInput(string i)
{
  addInput(i, 0.0);
}
void addInput(string i, double p)
{
  if (i.length() > 0) {
    iQueue_.push_back(i);
    iPriority_.push_back(p);
    iCount_ ++;
  }
}


void startInputs()
{
  for (int i = 0; i < inputCount_; i++)
    inputs_[i]->start();
}

void pauseInputs()
{
  for (int i = 0; i < inputCount_; i++)
    inputs_[i]->pause();
}


int INKEY()
{
  bool shift = (GetAsyncKeyState(VK_SHIFT)   < 0);
  bool ctrl = (GetAsyncKeyState(VK_CONTROL) < 0);
  bool alt = (GetAsyncKeyState(VK_MENU)    < 0);

  if (ctrl) {
    for (int i = 0x41; i < 0x5B; i++) {
      if (GetAsyncKeyState(i) == KEY_ACTIVE)
        return i - 0x41 + 1;
    }
  }
  if (GetAsyncKeyState(0x08) == KEY_ACTIVE)
    return 8;
  if (GetAsyncKeyState(0x09) == KEY_ACTIVE)
    return 9;
  if (GetAsyncKeyState(0x0D) == KEY_ACTIVE)
    return 13;

  if (GetAsyncKeyState(0x1B) == KEY_ACTIVE)
    return 27;
  if (GetAsyncKeyState(0x20) == KEY_ACTIVE)
    return 32;

  if (shift) {
    if (GetAsyncKeyState(0xC0) == KEY_ACTIVE)
      return 126;

    if (GetAsyncKeyState(0x31) == KEY_ACTIVE)
      return 33;
    if (GetAsyncKeyState(0x32) == KEY_ACTIVE)
      return 64;
    if (GetAsyncKeyState(0x33) == KEY_ACTIVE)
      return 35;
    if (GetAsyncKeyState(0x34) == KEY_ACTIVE)
      return 36;
    if (GetAsyncKeyState(0x35) == KEY_ACTIVE)
      return 37;
    if (GetAsyncKeyState(0x36) == KEY_ACTIVE)
      return 94;
    if (GetAsyncKeyState(0x37) == KEY_ACTIVE)
      return 38;
    if (GetAsyncKeyState(0x38) == KEY_ACTIVE)
      return 42;
    if (GetAsyncKeyState(0x39) == KEY_ACTIVE)
      return 40;
    if (GetAsyncKeyState(0x30) == KEY_ACTIVE)
      return 41;
    if (GetAsyncKeyState(0xBD) == KEY_ACTIVE)
      return 95;
    if (GetAsyncKeyState(0xBB) == KEY_ACTIVE)
      return 43;

    if (GetAsyncKeyState(0xBA) == KEY_ACTIVE)
      return 58;
    if (GetAsyncKeyState(0xBC) == KEY_ACTIVE)
      return 60;
    if (GetAsyncKeyState(0xBE) == KEY_ACTIVE)
      return 62;
    if (GetAsyncKeyState(0xBF) == KEY_ACTIVE)
      return 63;

    if (GetAsyncKeyState(0xDB) == KEY_ACTIVE)
      return 123;
    if (GetAsyncKeyState(0xDC) == KEY_ACTIVE)
      return 124;
    if (GetAsyncKeyState(0xDD) == KEY_ACTIVE)
      return 125;

    if (GetAsyncKeyState(0xDE) == KEY_ACTIVE)
      return 34;
  }
  if (GetAsyncKeyState(0xDE) == KEY_ACTIVE)
    return 39;

  if (GetAsyncKeyState(0xBC) == KEY_ACTIVE)
    return 44;
  if (GetAsyncKeyState(0xBD) == KEY_ACTIVE)
    return 45;
  if (GetAsyncKeyState(0xBE) == KEY_ACTIVE)
    return 46;
  if (GetAsyncKeyState(0xBF) == KEY_ACTIVE)
    return 47;

  if (GetAsyncKeyState(0x6A) == KEY_ACTIVE)
    return 42;
  if (GetAsyncKeyState(0x6B) == KEY_ACTIVE)
    return 43;
  if (GetAsyncKeyState(0x6D) == KEY_ACTIVE)
    return 45;
  if (GetAsyncKeyState(0x6E) == KEY_ACTIVE)
    return 46;
  if (GetAsyncKeyState(0x6F) == KEY_ACTIVE)
    return 47;

  if (GetAsyncKeyState(0xC0) == KEY_ACTIVE)
    return 96;

  if (shift == false) {
    if ((GetAsyncKeyState(0x30) == KEY_ACTIVE) || (GetAsyncKeyState(0x60) == KEY_ACTIVE))
      return 48;
    if ((GetAsyncKeyState(0x31) == KEY_ACTIVE) || (GetAsyncKeyState(0x61) == KEY_ACTIVE))
      return 49;
    if ((GetAsyncKeyState(0x32) == KEY_ACTIVE) || (GetAsyncKeyState(0x62) == KEY_ACTIVE))
      return 50;
    if ((GetAsyncKeyState(0x33) == KEY_ACTIVE) || (GetAsyncKeyState(0x63) == KEY_ACTIVE))
      return 51;
    if ((GetAsyncKeyState(0x34) == KEY_ACTIVE) || (GetAsyncKeyState(0x64) == KEY_ACTIVE))
      return 52;
    if ((GetAsyncKeyState(0x35) == KEY_ACTIVE) || (GetAsyncKeyState(0x65) == KEY_ACTIVE))
      return 53;
    if ((GetAsyncKeyState(0x36) == KEY_ACTIVE) || (GetAsyncKeyState(0x66) == KEY_ACTIVE))
      return 54;
    if ((GetAsyncKeyState(0x37) == KEY_ACTIVE) || (GetAsyncKeyState(0x67) == KEY_ACTIVE))
      return 55;
    if ((GetAsyncKeyState(0x38) == KEY_ACTIVE) || (GetAsyncKeyState(0x68) == KEY_ACTIVE))
      return 56;
    if ((GetAsyncKeyState(0x39) == KEY_ACTIVE) || (GetAsyncKeyState(0x69) == KEY_ACTIVE))
      return 57;
  }

  if (GetAsyncKeyState(0xBA) == KEY_ACTIVE)
    return 59;
  if (GetAsyncKeyState(0xBB) == KEY_ACTIVE)
    return 61;

  for (int i = 0x41; i < 0x5B; i++) {
    if (GetAsyncKeyState(i) == KEY_ACTIVE) {
      if (shift)
        return 65 + i - 0x41;
      else
        return 97 + i - 0x41;
    }
  }

  if (GetAsyncKeyState(0xDB) == KEY_ACTIVE)
    return 91;
  if (GetAsyncKeyState(0xDC) == KEY_ACTIVE)
    return 92;
  if (GetAsyncKeyState(0xDD) == KEY_ACTIVE)
    return 93;

  return 0;
}

char INKEYC()
{
  return INKEY();
}

string INKEYS()
{
  return str(INKEYC());
}