////////////////////////////////////////////////////////////////////////////////
//File:   module_installer.h
//Dir:    *
//Date:   2015-01-04
//Author: Zachary Gill
//Interface of Module Installer
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_MI_H
#define _DLA_MI_H


//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>


//includes
//#include "resource/filesystem.h"


//global variable definitions
HANDLE h_  = GetStdHandle(STD_OUTPUT_HANDLE);


//function definitions
int         main        ();
void        readOutFile (std::fstream&, int, int, std::string);


//shared function definitions
#include "resource/common.h"


#endif