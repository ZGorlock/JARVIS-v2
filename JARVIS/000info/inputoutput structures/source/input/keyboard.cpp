////////////////////////////////////////////////////////////////////////////////
//File:   keyboard.cpp
//Dir:    source/input/
//Date:   2014-03-31
//Author: Zachary Gill
//Implementation of KeyboardInput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "../resource/const.h"


//standard includes
#include <string>


//includes
#include "keyboard.h"


//typedefs
typedef void (*InPtr)();


//namespaces
using namespace std;


//Constructors

KeyboardInput::KeyboardInput()
             : TextInput("keyboard")
{
  Input::inFunc = &KeyboardInput::getInput;
}


//Other Functions

void KeyboardInput::init()
{
}

void KeyboardInput::getInput()
{
}