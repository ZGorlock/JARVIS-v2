////////////////////////////////////////////////////////////////////////////////
//File:   speed_O.h
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of SpeedOutput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SO_SPEED_H_
#define _DLA_SO_SPEED_H_


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <string>
#include <vector>
#include <ctime>


//includes
#include "output.h"

#include "timer.h"


//class definitions
class SpeedOutput : public TextOutput
{
  public:
    SpeedOutput                         ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectOutput   ();
    virtual inline void free            ();

  private:
    Timer  t;
    size_t oldILogSize;
    size_t oldOLogSize;
};


//global variable definitions
extern bool                     terminate_;
extern std::string              request_;
extern std::string              response_;
extern int                      oCurrent_;
extern double                   oSavePriority_;
extern std::vector<std::string> iLog_;
extern std::vector<std::string> oLog_;


//shared function definitions
#include "common.h"


#endif