////////////////////////////////////////////////////////////////////////////////
//File:   main.h
//Dir:    source/header/
//Date:   2015-01-04
//Author: Zachary Gill
//Interface for Main Program
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_MAIN_H_
#define _DLA_SH_MAIN_H_


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <map>

#include <boost/thread/thread.hpp>

#include "tre/regex.h"


//includes
#include "module.h"
#include "command.h"
#include "input.h"
#include "output.h"
#include "variable.h"
#include "settings.h"
#include "contact.h"
#include "user.h"


//typedefs
typedef std::string (*CmdPtr)();
typedef double      (*VarPtr)(std::string);


//global variable definitions
std::string                   name_          = "NULL";
int                           gender_        = GENDER_DEFAULT;

std::string                   os_            = "";
std::string                   architecture_  = ""; 

std::string                   alias_         = "";

bool                          terminate_     = false;
bool                          help_          = false;
bool                          hidden_        = false;
bool                          hide_          = false;
bool                          foreground_    = false;

std::string                   request_       = "";
std::string                   response_      = "";
std::string                   log_           = "";

std::string                   loc_           = "";
std::string                   userLoc_       = "";

bool                          showPassword_;
WORD                          FGColor_;
WORD                          FGIntensity_;
WORD                          BGColor_;
WORD                          BGIntensity_;


std::vector<std::string>      iQueue_;
std::vector<std::string>      oQueue_;

std::vector<double>           iPriority_;
std::vector<double>           oPriority_;
double                        oSavePriority_;

std::vector<std::string>      iLog_;
std::vector<std::string>      oLog_;

int                           iCount_        = 0;
int                           oCount_        = 0;

int                           iCurrent_      = 0;
int                           oCurrent_      = 0;

int                           moduleCount_   = 0;
int                           commandCount_  = 0;
int                           inputCount_    = 0;
int                           outputCount_   = 0;
int                           variableCount_ = 0;

HINSTANCE                     hinst_         = (HINSTANCE)GetModuleHandle(NULL);
HWND                          hWnd_          = GetConsoleWindow();
HANDLE                        hStdout_       = GetStdHandle(STD_OUTPUT_HANDLE);

std::vector<Module>           modules_;
std::vector<Input*>           inputs_;
std::vector<Output*>          outputs_;

std::vector<std::string>      variables_;
std::vector<std::string>      variableTypes_;

std::vector<std::string>      moduleList_;
std::vector<std::string>      inputList_;
std::vector<std::string>      outputList_;
std::vector<std::string>      userList_;
std::vector<std::string>      variableList_;

regaparams_t                  reg_params_;
regmatch_t                    reg_pmatch_[REG_PMATCH_SIZE];
regamatch_t                   reg_match_;

boost::thread_group           mThread_;
boost::thread_group           iThread_;
boost::thread_group           oThread_;

boost::thread                 fgmonitor_;
boost::thread                 hdmonitor_;

std::map<std::string, CmdPtr> CommandPtrs_;
std::map<std::string, VarPtr> VariablePtrs_;

Settings                      settings_;
Settings                      defaultSettings_;

User                          user_;


//resources
#include "modules"
#include "inputs"
#include "outputs"
#include "resources.h"


//function definitions
int         main               ();

inline void setup              ();
inline void setConsole         ();
inline void load               ();
inline void init               ();
inline void startup            ();

inline void foreground         ();
inline void hidden             ();

void        input              ();

void        respond            ();
void        respond            (std::string);
void        output             ();

void        defaultSettings    ();
void        loadSharedSettings ();

void        log                (std::string);

void        cleanup            ();


//shared function definitions
#include "common.h"


#endif