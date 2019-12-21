////////////////////////////////////////////////////////////////////////////////
//File:   common.h
//Dir:    source/resource/
//Date:   2014-03-30
//Author: Zachary Gill
//Common functions Interface
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SR_COMMON_H_
#define _JARVIS_SR_COMMON_H_


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>


//function definitions
bool        fileExists (std::string);

bool        getint     (std::fstream&, int&);
bool        getdouble  (std::fstream&, double&);
bool        getbool    (std::fstream&, bool&);

void        loadFileS  (std::string, std::vector<std::string>&);
void        loadFileI  (std::string, std::vector<int>&);
void        loadFileD  (std::string, std::vector<double>&);

void        cls        (HANDLE);

std::string uCase      (std::string);
std::string lCase      (std::string);

int         val        (std::string);
double      vald       (std::string);
std::string str        (int);
std::string str        (double);


#endif