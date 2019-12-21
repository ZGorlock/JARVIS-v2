////////////////////////////////////////////////////////////////////////////////
//File:   resources.h
//Dir:    source/resource/
//Date:   2014-01-21
//Author: Zachary Gill
//Loads global resources
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SR_RESOURCES_H_
#define _DLA_SR_RESOURCES_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <array>
#include <vector>
#include <fstream>


//global variable definitions
std::vector<std::string> directories_;
std::vector<std::string> directories_user_;

bool                     keyboard_touch_[NUM_LETTERS][NUM_LETTERS];

std::vector<std::string> names_boy_;
std::vector<std::string> names_girl_;

std::vector<std::string> question_words_;
std::vector<std::string> question_words_basic_;
std::vector<std::string> question_words_complex_;

std::vector<std::string> words_no_;
std::vector<std::string> words_yes_;

#include "input/resources"
#include "module/resources"
#include "output/resources"


//function definitions
void loadResources();

void loadFileS_resource(std::string, std::vector<std::string>&);


//functions

void loadResources()
{
  loadFileS_resource("resource/data/directories", directories_);
  loadFileS_resource("resource/data/directories_user", directories_user_);
  
  std::fstream fs;
  fs.open("resource/data/keyboard_touch", std::fstream::in);
  int in;
  for (int i = 0; i < NUM_LETTERS; i++) {
    for (int j = 0; j < NUM_LETTERS; j++) {
      fs >> in;
      keyboard_touch_[i][j] = (in != 0);
    }
  }
  fs.close();

  loadFileS_resource("resource/data/names_boy", names_boy_);
  loadFileS_resource("resource/data/names_girl", names_girl_);

  loadFileS_resource("resource/data/question_words", question_words_);
  loadFileS_resource("resource/data/question_words_basic", question_words_basic_);  
  loadFileS_resource("resource/data/question_words_complex", question_words_complex_);

  loadFileS_resource("resource/data/words_no", words_no_);  
  loadFileS_resource("resource/data/words_yes", words_yes_);
}


void loadFileS_resource(std::string file, std::vector<std::string>& data)
{
  std::string line = "";
  std::fstream fs;
  fs.open(file.c_str(), std::fstream::in);
  if (fs.is_open()) {
    while (getline(fs, line)) {
      data.push_back(line);
    }
  }
  fs.close();
}


#endif