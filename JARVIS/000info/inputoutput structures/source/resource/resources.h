////////////////////////////////////////////////////////////////////////////////
//File:   resources.h
//Dir:    source/resource/
//Date:   2014-03-29
//Author: Zachary Gill
//Loads global resources
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SR_RESOURCES_H_
#define _JARVIS_SR_RESOURCES_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <vector>


//global variable definitions
std::vector<std::string> question_words_;
std::vector<std::string> question_words_basic_;
std::vector<std::string> question_words_complex_;


//function definitions
void loadResources();


//functions
#include "common.h"

void loadResources()
{
  loadFileS("resource/data/question_words", question_words_);
  loadFileS("resource/data/question_words_basic", question_words_basic_);
  loadFileS("resource/data/question_words_complex", question_words_complex_);
}


#endif