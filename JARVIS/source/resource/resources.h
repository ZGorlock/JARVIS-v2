////////////////////////////////////////////////////////////////////////////////
//File:   resources.h
//Dir:    source/resource/
//Date:   2014-03-18
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


//namespaces
using namespace std;


//global variable definitions
vector<string> question_words_;
vector<string> question_words_basic_;
vector<string> question_words_complex_;


//function definitions
void loadResources();


//functions
void loadResources()
{
  string line = "";
  fstream fs;

  //load question words
  fs.open("resource/data/question_words", fstream::in);
  if (fs.is_open())
    while (getline(fs, line)) {
      question_words_.push_back(line);
    }
  fs.close();
  fs.clear();

  //load basic question words
  fs.open("resource/data/question_words_basic", fstream::in);
  if (fs.is_open())
    while (getline(fs, line)) {
      question_words_basic_.push_back(line);
    }
  fs.close();
  fs.clear();

  //load complex question words
  fs.open("resource/data/question_words_complex", fstream::in);
  if (fs.is_open())
    while (getline(fs, line)) {
      question_words_complex_.push_back(line);
    }
  fs.close();
  fs.clear();
}


#endif