////////////////////////////////////////////////////////////////////////////////
//File:   input.cpp
//Dir:    source/
//Date:   2014-03-31
//Author: Zachary Gill
//Implementation of Input classes
////////////////////////////////////////////////////////////////////////////////


//constants
#include "resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include <map>


//includes
#include "header/input.h"


//typedefs
typedef void (*InPtr)();


//namespaces
using namespace std;


//Constructors

Input::Input()
{
  Input::name   = "";
  Input::type   = 0;
  Input::inFunc = &Input::getInput;
  
  inputCount_ ++;
}
Input::Input(string name, int type)
{
  Input::name   = name;
  Input::type   = type;
  Input::inFunc = &Input::getInput;
  
  inputCount_ ++;
}

TextInput::TextInput()
         : Input("", 1)
{
  TextInput::buffer = "";
}
TextInput::TextInput(string name)
         : Input(name, 1)
{
  TextInput::buffer = "";
}


//Destructors

Input::~Input()
{
}

void Input::free()
{
  Input::~Input();
  inputCount_ --;
}


//Accessors

string Input::getName() const
{
  return Input::name;
}

int Input::getType() const
{
  return Input::type;
}

string TextInput::getBuffer() const
{
  return TextInput::buffer;
}


//Modifiers

void TextInput::updateBuffer (string addToBuffer)
{
  TextInput::buffer += addToBuffer;
}

void TextInput::clearBuffer ()
{
  TextInput::buffer = "";
}


//Other Functions

void Input::init()
{
}

void Input::getInput()
{
}


//functions
void initInputs()
{
  //#include "resource/initInputs"
  KeyboardInput tmpInput ();
  inputs_.push_back(tmpInput);
}