////////////////////////////////////////////////////////////////////////////////
//File:   input.h
//Dir:    source/input/
//Date:   YYYY-MM-DD
//Author: Zachary Gill
//Interface of InputInput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SI_INPUT_H_
#define _DLA_SI_INPUT_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include <vector>


//includes
#include "../header/input.h"
#include "../header/settings.h"


//class definitions
class InputInput : public TextInput
{
  public:
    InputInput                          ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectInput    ();
    virtual void        free            ();
} ;


//global variable definitions
extern bool                     terminate_;
extern bool                     foreground_;
extern std::string              userLoc_;
extern std::vector<std::string> iLog_;
extern std::vector<std::string> oLog_;
extern int                      inputCount_;
extern std::vector<Input*>      inputs_;
extern Settings                 settings_;


#endif