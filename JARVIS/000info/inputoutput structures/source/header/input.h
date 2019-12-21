////////////////////////////////////////////////////////////////////////////////
//File:   input.h
//Dir:    source/header/
//Date:   2014-03-31
//Author: Zachary Gill
//Interface of Input classes
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_INPUT_H_
#define _JARVIS_SH_INPUT_H_


//constants
#include "../resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include <map>


//typedefs
typedef void (*InPtr)();


//class definitions
class Input
{  
  public:
    //Constructors
    Input ();
    Input (std::string, int);

    //Destructors
    ~Input    ();
    void free ();

    //Accessors
    std::string getName () const;
    int         getType () const;

    //Other Functions
    void init     ();
    void getInput ();

  protected:
    std::string name;
    int         type;
    InPtr       inFunc;
} ;

class TextInput : public Input
{  
  public:
    //Constructors
    TextInput ();
    TextInput (std::string);

    //Accessors
    std::string getBuffer () const;

    //Modifiers
    void updateBuffer (std::string);
    void clearBuffer  ();

  protected:
    std::string buffer;
} ;


//global variable definitions
extern std::string                  request_;
extern int                          inputCount_;
extern std::vector<Input>           inputs_;


//function definitions
void initInputs ();


//shared function definitions
#include "../resource/common.h"


#endif