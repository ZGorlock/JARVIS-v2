////////////////////////////////////////////////////////////////////////////////
//File:   console_O.cpp
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of ConsoleOutput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>


//includes
#include "console_O.h"
#include "settings.h"


//namespaces
using namespace std;


//Constructors

ConsoleOutput::ConsoleOutput()
{
  ConsoleOutput::name = "CONSOLE";
  ConsoleOutput::console = true;
}


//Destructors

inline void ConsoleOutput::free()
{
  SetConsoleActiveScreenBuffer(hStdout_);
  Output::~Output();
}


//Other Functions

void ConsoleOutput::main()
{
  ConsoleOutput::init();
  do {
    DELAY_LOOP;
    if (ConsoleOutput::active) {

      if (ConsoleOutput::collectOutput()) {
        cout << uCase(name_) << endl << endl;
        for (int i = 0; i < inputCount_; i++) {
          int t = inputs_[i]->getType();
          if (t == INPUT_TEXT || t == INPUT_AUDIO) {
            TextInput* ti = (TextInput*)inputs_[i];
            if (ti->getConsole() && ti->getActive())
              cout << "<<" << ti->getBuffer() << endl;
          }
        }
        for (int i = 0; i < outputCount_; i++) {
          int t = outputs_[i]->getType();
          if (t == OUTPUT_TEXT) {
            TextOutput* to = (TextOutput*)outputs_[i];
            if (to->getConsole() && to->getActive())
              cout << ">>" << to->getBuffer() << endl;
          }
        }
        ConsoleOutput::display();
        ConsoleOutput::done = true;
      }

    }
  } while (!terminate_);
  ConsoleOutput::free();
}

inline void ConsoleOutput::init()
{
  if (!settings_.hasSettingGroup(SETTING_OUTPUT, "CONSOLE"))
    ConsoleOutput::defaultSettings();

  ConsoleOutput::hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  SetConsoleActiveScreenBuffer(ConsoleOutput::hScreenBuffer);
  CONSOLE_CURSOR_INFO cursorInfo;
  cursorInfo.dwSize = 100;
  cursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(hScreenBuffer, &cursorInfo);

  ConsoleOutput::srctReadRect.Top     = 0;
  ConsoleOutput::srctReadRect.Left    = 0;
  ConsoleOutput::srctReadRect.Bottom  = 24;
  ConsoleOutput::srctReadRect.Right   = 79;

  ConsoleOutput::coordBufSize.Y       = 25;
  ConsoleOutput::coordBufSize.X       = 80;

  ConsoleOutput::coordBufCoord.X      = 0;
  ConsoleOutput::coordBufCoord.Y      = 0;

  ConsoleOutput::srctWriteRect.Top    = 0;
  ConsoleOutput::srctWriteRect.Left   = 0;
  ConsoleOutput::srctWriteRect.Bottom = 24;
  ConsoleOutput::srctWriteRect.Right  = 79;
}

inline void ConsoleOutput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("CONSOLE");
  sg.setType(SETTING_OUTPUT);
  sg.setFile(userLoc_ + "data/setting/output/console.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

inline bool ConsoleOutput::collectOutput()
{
  if (response_ > ""){
    TextOutput::clearBuffer();
    TextOutput::setBuffer(response_);
  }

  return true;
}

void ConsoleOutput::start()
{
  SetConsoleActiveScreenBuffer(ConsoleOutput::hScreenBuffer);
  Output::start();
}

void ConsoleOutput::pause()
{
  SetConsoleActiveScreenBuffer(hStdout_);
  Output::pause();
}

void ConsoleOutput::display()
{
  ReadConsoleOutput(hStdout_,                      ConsoleOutput::chiBuffer, ConsoleOutput::coordBufSize, ConsoleOutput::coordBufCoord, &srctReadRect);
  WriteConsoleOutput(ConsoleOutput::hScreenBuffer, ConsoleOutput::chiBuffer, ConsoleOutput::coordBufSize, ConsoleOutput::coordBufCoord, &srctWriteRect);
  cls(hStdout_);
}