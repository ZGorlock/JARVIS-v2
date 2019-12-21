////////////////////////////////////////////////////////////////////////////////
//File:   main.h
//Dir:    source/header/
//Date:   2014-03-19
//Author: Zachary Gill
//Interface for Main Program
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_MAIN_H_
#define _JARVIS_SH_MAIN_H_


//constants
#include "../resource/const.h"


//standard includes
#include <vector>
#include <Windows.h>


//includes
#include "module.h"
#include "command.h"


//global variable definitions
bool terminate_ = false;

int moduleCount_ =  0;
int commandCount_ = 0;
int inputCount_ =   0;
int outputCount_ =  0;

HANDLE h_ = GetStdHandle(STD_OUTPUT_HANDLE);

vector<Module> modules_;


//resources
#include "../resource/resources.h"
#include "../resource/modules.h"


//function definitions
void load();
void init();


//functions
#include "../resource/common.h"


#endif