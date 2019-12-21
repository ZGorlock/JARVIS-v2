////////////////////////////////////////////////////////////////////////////////
//File:   log_O.h
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of LogOutput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SO_LOG_H_
#define _DLA_SO_LOG_H_


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


//includes
#include "output.h"
#include "settings.h"
#include "user.h"


//class definitions
class LogOutput : public TextOutput
{
  public:
    LogOutput                           ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectOutput   ();
    virtual inline void free            ();
};


//global variable definitions
extern bool                 terminate_;
extern std::string          log_;
extern int                  outputCount_;
extern std::vector<Output*> outputs_;
extern User                 user_;


//function definitions
void clearLog();


#endif