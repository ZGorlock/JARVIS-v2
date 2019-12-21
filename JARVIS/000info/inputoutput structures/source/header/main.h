////////////////////////////////////////////////////////////////////////////////
//File:   main.h
//Dir:    source/header/
//Date:   2014-03-31
//Author: Zachary Gill
//Interface for Main Program
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_MAIN_H_
#define _JARVIS_SH_MAIN_H_


//constants
#include "../resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <Windows.h>


//includes
#include "input.h"
#include "output.h"
#include "module.h"
#include "command.h"
#include "settings.h"


//typedefs
typedef std::string (*ComPtr)();


//global variable definitions
bool terminate_ = false;

std::string request_  = "";
std::string response_ = "";

int moduleCount_  = 0;
int commandCount_ = 0;
int inputCount_   = 0;
int outputCount_  = 0;

HANDLE h_  = GetStdHandle(STD_OUTPUT_HANDLE);
HWND hWnd_ = (HWND)h_;

std::vector<Input>  inputs_;
std::vector<Output> outputs_;
std::vector<Module> modules_;

std::map<std::string, ComPtr> CommandPtrs_;

std::fstream log_;

Settings settings_;


//resources
#include "../resource/inputs"
#include "../resource/outputs"
#include "../resource/modules"
#include "../resource/resources.h"


//function definitions
int  main    ();

void load    ();
void init    ();

void input   ();
void respond ();
void respond (std::string);
void output  ();

void cleanup ();


//shared function definitions
#include "../resource/common.h"


#endif