////////////////////////////////////////////////////////////////////////////////
//File:   output.h
//Dir:    source/header/
//Date:   2014-03-31
//Author: Zachary Gill
//Interface of Output
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_OUTPUT_H_
#define _JARVIS_SH_OUTPUT_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include <vector>


//class definitions
class Output
{  
  public:
    //Constructors
    Output ();
    Output (std::string, int);

    //Destructors
    ~Output   ();
    void free ();

    //Accessors
    std::string getName   () const;
    int         getType   () const;

    //Other Functions
    void init      ();
    void putOutput ();

  protected:
    std::string name;
    int         type;
} ;


//global variable definitions
extern std::string        response_;
extern int                outputCount_;
extern std::vector<Output> outputs_;


//function definitions
void loadOutputs ();


//shared function definitions
#include "../resource/common.h"


#endif