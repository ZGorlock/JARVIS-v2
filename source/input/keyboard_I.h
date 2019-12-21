////////////////////////////////////////////////////////////////////////////////
//File:   keyboard_I.h
//Dir:    source/input/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of KeyboardInput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SI_KEYBOARD_H_
#define _DLA_SI_KEYBOARD_H_


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <string>
#include <vector>


//includes
#include "input.h"
#include "settings.h"


//class definitions
class KeyboardInput : public TextInput
{
  public:
    KeyboardInput                       ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectInput    ();
    virtual void        free            ();

    char                getLast         () const;
    void                setLast         (char);

    int pos;

  protected:
    char last;
    
  private:
    HANDLE hstdin;
    DWORD  modein;
} ;


//global variable definitions
extern bool                     terminate_;
extern bool                     foreground_;
extern std::vector<std::string> iLog_;
extern std::vector<std::string> oLog_;
extern int                      inputCount_;
extern std::vector<Input*>      inputs_;
extern Settings                 settings_;


#endif