////////////////////////////////////////////////////////////////////////////////
//File:   settings.h
//Dir:    source/header/
//Date:   2014-03-29
//Author: Zachary Gill
//Interface of Settings
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_SETTINGS_H_
#define _JARVIS_SH_SETTINGS_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>


//class definitions
class Settings
{
  public:
    bool CLOCK_UNIVERSAL;
    bool CLOCK_MILITARY;
    bool CLOCK_ACCURACY;

    //Constructors
    Settings ();

    //Destructors
    ~Settings ();

    //Other Functions
    void save ();
    void save (std::string);
    void load ();
    void load (std::string);
    void reset();
    void dump ();
    void dump (std::ostream&);
} ;


//global variable definitions
extern Settings settings_;


//function definitions
void saveSettings();
void saveSettings(std::string);
void loadSettings();
void loadSettings(std::string);


//shared function definitions
#include "../resource/common.h"


#endif