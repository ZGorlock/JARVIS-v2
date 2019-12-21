////////////////////////////////////////////////////////////////////////////////
//File:   output.cpp
//Dir:    source/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of Output
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>


//includes
#include "output.h"


//namespaces
using namespace std;


//Constructors

Output::Output()
{
  Output::type = OUTPUT_OUTPUT;
  Output::name = "";
  Output::active = true;
  Output::done = true;
  Output::console = false;

  outputCount_++;
}

TextOutput::TextOutput()
{
  TextOutput::type = OUTPUT_TEXT;
  TextOutput::buffer = "";
  TextOutput::last = "";
}

AudioOutput::AudioOutput()
{
  AudioOutput::type = OUTPUT_AUDIO;
  AudioOutput::currentOutput = 0;
  AudioOutput::speaking = false;
  AudioOutput::location = 0;
}

GraphicOutput::GraphicOutput()
{
  GraphicOutput::type = OUTPUT_GRAPHIC;
}


//Destructors

Output::~Output()
{
}

inline void Output::free()
{
  Output::~Output();
  outputCount_--;
}


//Accessors

int Output::getType() const
{
  return Output::type;
}

string Output::getName() const
{
  return Output::name;
}

bool Output::getActive() const
{
  return Output::active;
}

bool Output::getConsole() const
{
  return Output::console;
}

string TextOutput::getBuffer() const
{
  return TextOutput::buffer;
}

string TextOutput::getLast() const
{
  return TextOutput::last;
}

int AudioOutput::getLocation() const
{
  return AudioOutput::location;
}


//Modifiers

void Output::setType(int type)
{
  Output::type = type;
}

void Output::setName(string name)
{
  Output::name = name;
}

void TextOutput::setBuffer(string buffer)
{
  TextOutput::buffer = buffer;
}

void TextOutput::clearBuffer()
{
  TextOutput::buffer = "";
}

void TextOutput::setLast(string last)
{
  TextOutput::last = last;
}

void AudioOutput::setLocation(int location)
{
  AudioOutput::location = location;
}


//Other Functions

void Output::main()
{
  return;
}

inline void Output::init()
{
  return;
}

inline void Output::defaultSettings()
{
  return;
}

inline bool Output::collectOutput()
{
  return false;
}

void Output::start()
{
  Output::active = true;
}

void Output::pause()
{
  Output::active = false;
}

bool AudioOutput::isSpeaking()
{
  return AudioOutput::speaking;
}


//functions
void loadOutputs()
{
  loadOutputs("");
}
void loadOutputs(string username)
{
  if (username > "")
    loadFileS("data/user/" + username + "/data/outputs", outputList_);
  else
    loadFileS("data/outputs", outputList_);
  for (size_t i = 0; i < outputList_.size(); i++)
    outputList_[i] = uCase(outputList_[i]);
  
  #include "../data/output/ptrs"
}

bool isOutput(string name, vector<string>& outputList)
{
  name = uCase(name);
  for (size_t i = 0; i < outputList.size(); i++) {
    if (name == outputList[i])
      return true;
  }
  return false;
}

bool hasOutput(string name)
{
  for (int i = 0; i < outputCount_; i++)
  if (outputs_[i]->getName() == name)
    return true;
  return false;
}


void addOutput(string i)
{
  addOutput(i, 0.0);
}
void addOutput(string i, double p)
{
  if (i.length() > 0) {
    oQueue_.push_back(i);
    oPriority_.push_back(p);
    oCount_ ++;
  }
}


void startOutputs()
{
  for (int i = 0; i < outputCount_; i++)
    outputs_[i]->start();
}

void pauseOutputs()
{
  for (int i = 0; i < outputCount_; i++)
    outputs_[i]->pause();
}