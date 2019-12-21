////////////////////////////////////////////////////////////////////////////////
//File:   variable.h
//Dir:    source/header/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of Variable
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_VARIABLE_H_
#define _DLA_SH_VARIABLE_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <vector>
#include <map>


//typedefs
typedef double (*VarPtr)(std::string);


//global variable definitions
extern int                           variableCount_;
extern std::vector<std::string>      variables_;
extern std::vector<std::string>      variableTypes_;
extern std::vector<std::string>      variableList_;
extern std::map<std::string, VarPtr> VariablePtrs_;


//resources
#include "variables"


//function definitions
bool hasVariable  (std::string);
bool isVariable   (std::string);


//shared function definitions
#include "common.h"


#endif