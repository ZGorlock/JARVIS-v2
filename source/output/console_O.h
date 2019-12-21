////////////////////////////////////////////////////////////////////////////////
//File:   console_O.h
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of ConsoleOutput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SO_CONSOLE_H_
#define _DLA_SO_CONSOLE_H_


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>


//includes
#include "input.h"
#include "output.h"
#include "settings.h"


//class definitions
class ConsoleOutput : public TextOutput
{
  public:
    ConsoleOutput                       ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectOutput   ();
    virtual void        start           ();
    virtual void        pause           ();
    virtual inline void free            ();

  protected:
    void                display         ();

    HANDLE     hScreenBuffer;
    SMALL_RECT srctReadRect;
    SMALL_RECT srctWriteRect;
    CHAR_INFO  chiBuffer[2000];
    COORD      coordBufSize;
    COORD      coordBufCoord;
};


//global variable definitions
extern std::string          name_;
extern bool                 terminate_;
extern std::string          response_;
extern int                  inputCount_;
extern int                  outputCount_;
extern HANDLE               hStdout_;
extern std::vector<Input*>  inputs_;
extern std::vector<Output*> outputs_;


#endif