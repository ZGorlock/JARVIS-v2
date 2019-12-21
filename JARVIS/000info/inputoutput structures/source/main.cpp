////////////////////////////////////////////////////////////////////////////////
//File:   main.cpp
//Dir:    source/
//Date:   2014-03-31
//Author: Zachary Gill
//Main Program
////////////////////////////////////////////////////////////////////////////////

#define  NTDDI_VERSION 0x05010000
#define  WINVER        0x0501
#define _WIN32_WINNT   0x0501
#define _WIN32_IE      0x0500


//constants
#include "resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include <conio.h>


//includes
#include "header/main.h"


//namespaces
using namespace std;


//event handlers
BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
    switch(CEvent) {
      case CTRL_C_EVENT :
      case CTRL_BREAK_EVENT :
        saveSettings();
        cleanup();
        return false;
      case CTRL_CLOSE_EVENT :
        saveSettings();
        cleanup();
        return false;
      case CTRL_LOGOFF_EVENT :
        saveSettings();
        cleanup();
        return false;
      case CTRL_SHUTDOWN_EVENT :
        saveSettings();
        cleanup();
        return false;
    }
    return true;
}


//functions

int main()
{
  load();
  init();

  //main loop
  do {
    input();

    //analyze input
    //parse(request);

    respond();

    output();
  } while (terminate_ == false);

  saveSettings();
  cleanup();

  return 0;
}

void load()
{
  loadResources();
  //loadUser();
  loadSettings();
  //loadInputs();
  loadOutputs();
  loadModules();

  //populate CommandPtrs_ map
  //#include "../data/input/ptrs"
  //#include "../data/output/ptrs"
  #include "../data/module/ptrs"
}


void init()
{
  SetConsoleCtrlHandler(ConsoleHandler, true);
  log_.open("data/log.log", fstream::app);
  SetConsoleTextAttribute(h_, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

  //initiate inputs
  initInputs();

  //turn modules

  cls(h_);
  cout << "JARVIS" << endl << endl;
}

void input()
{
  request_ = "";
  cout << "<<";
  getline (cin, request_);
  if (hasModule("TIME"))
    log_ << TIME::timestamp();
  log_ << "<<" << request_ << endl;
}

void respond()
{
  respond(request_);
}
void respond(string request)
{
  double bid           = 0.0;
  int    highestBidder = 0;
  double highestBid    = 0.0;

  for (int i = 0; i < moduleCount_; i ++) {
    bid = modules_[i].queue();
    if (bid > highestBid) {
      highestBidder = i;
      highestBid    = bid;
    }
  }

  if (highestBid > MINIMUM_MATCH_SCORE)
    modules_[highestBidder].execute();
}

void output()
{
  if (response_ > "") {
    cout << ">>" << response_ << endl;
    if (hasModule("TIME"))
      log_ << TIME::timestamp();
    log_ << ">>" << response_ << endl;
    response_ = "";
  }
}

void cleanup()
{
  settings_.~Settings();
  for (size_t i = 0; i < modules_.size(); i ++)
    modules_[i].free();

  log_ << "--------------------------------------------------------------------------------" << endl;
}