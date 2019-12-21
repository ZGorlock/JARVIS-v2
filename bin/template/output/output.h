////////////////////////////////////////////////////////////////////////////////
//File:   output.h
//Dir:    source/output/
//Date:   YYYY-MM-DD
//Author: Zachary Gill
//Interface of OutputOutput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SO_OUTPUT_H_
#define _DLA_SO_OUTPUT_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include <vector>

//includes
#include "../header/output.h"
#include "../header/settings.h"


//class definitions
class OutputOutput : public TextOutput
{
  public:
    OutputOutput                        ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectOutput   ();
    virtual void        free            ();
} ;


//global variable definitions
extern bool                     terminate_;
extern bool                     foreground_;
extern std::string              userLoc_;
extern std::vector<std::string> iLog_;
extern std::vector<std::string> oLog_;
extern int                      outputCount_;
extern std::vector<Output*>     outputs_;
extern Settings                 settings_;


#endif