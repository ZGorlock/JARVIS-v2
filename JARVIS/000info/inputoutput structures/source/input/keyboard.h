////////////////////////////////////////////////////////////////////////////////
//File:   keyboard.h
//Dir:    source/input/
//Date:   2014-03-31
//Author: Zachary Gill
//Interface of KeyboardInput
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SI_KEYBOARD_H_
#define _JARVIS_SI_KEYBOARD_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include <vector>


//includes
#include "../header/input.h"


//typedefs
typedef void (*InPtr)();


//class definitions
class KeyboardInput : public TextInput
{  
  public:
    //Constructors
    KeyboardInput ();

    //Other Functions
    void init     ();
    void getInput ();
} ;


//global variable definitions
extern std::string        request_;
extern int                inputCount_;
extern std::vector<Input> inputs_;


//shared function definitions
#include "../resource/common.h"


#endif