////////////////////////////////////////////////////////////////////////////////
//File:   main.cpp
//Dir:    source/
//Date:   2015-05-10
//Author: Zachary Gill
//Main Program
////////////////////////////////////////////////////////////////////////////////

#define  NTDDI_VERSION NTDDI_WIN7
#define  WINVER        _WIN32_WINNT_WIN7
#define _WIN32_WINNT   _WIN32_WINNT_WIN7
#define _WIN32_IE      _WIN32_IE_IE50

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <conio.h>

#include <boost/thread/thread.hpp>

#include "tre/regex.h"


//includes
#include "main.h"
#include "module.h"
#include "command.h"
#include "input.h"
#include "output.h"
#include "variable.h"
#include "settings.h"
#include "contact.h"
#include "user.h"
#include "testing.h"


//namespaces
using namespace std;


//event handlers
BOOL WINAPI ConsoleHandler (DWORD CEvent)
{
    switch(CEvent) {
      case CTRL_BREAK_EVENT:
        cleanup();
        return false;
      case CTRL_CLOSE_EVENT:
        cleanup();
        return false;
      case CTRL_LOGOFF_EVENT:
        cleanup();
        return false;
      case CTRL_SHUTDOWN_EVENT:
        cleanup();
        return false;
    }
    return true;
}


//functions

int main ()
{
  if (testing())
    return EXIT_FAILURE;

  setup();

  //main loop
  do {
    DELAY_LOOP;
    input();
    respond();
    output();
  } while (!terminate_);

  cleanup();

  return EXIT_SUCCESS;
}


void setup()
{
  setConsole();
  load();
  init();
  startup();
}

void setConsole()
{
  //turn on
  fstream turnOn;
  turnOn.open(loc_ + "bin/inuse", fstream::out);
  while (!turnOn) {
    delay(200 + (rand() % 50));
    turnOn.open(loc_ + "bin/inuse", fstream::out);
  }
  turnOn << "1";
  turnOn.close();

  //load shared settings
  loadSharedSettings();

  //console settings
  SetConsoleTextAttribute(hStdout_, FGColor_ | FGIntensity_ | BGColor_ | BGIntensity_);
  CONSOLE_CURSOR_INFO cursorInfo;
  cursorInfo.dwSize = sizeof(CONSOLE_CURSOR_INFO);
  cursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(hStdout_, &cursorInfo);

  //get system information
  SYSTEM_INFO sinfo;
  GetNativeSystemInfo(&sinfo);
  switch (sinfo.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_IA64:
    case PROCESSOR_ARCHITECTURE_AMD64:
      architecture_ = "X64";
    case PROCESSOR_ARCHITECTURE_INTEL:
      architecture_ = "X86";
    default:
      architecture_ = "Unkown";
  }
  #ifdef _WIN32
    os_ = "Windows";
  #elif _WIN64
    os_ = "Windows";
  #elif __unix
    os_ = "Unix";
  #elif __unix__
    os_ = "Unix";
  #elif __APPLE__
    os_ = "Mac OSX";
  #elif __MACH__
    os_ = "Mac OSX";
  #elif __linux__
    os_ = "Linux";
  #elif __FreeBSD__
    os_ = "FreeBSD";
  #else
    os_ = "Unkown";
  #endif

  //update loc
  TCHAR n [FILENAME_MAX];
  GetModuleFileName(NULL, n, FILENAME_MAX);
  loc_ = n;
  loc_ = loc_.substr(0, loc_.length() - 7);
  ReplaceStringInPlace(loc_, "\\", "/");
  fstream fs;
  char lpName[16] = "ProgramData";
  char lpBuffer[128];
  GetEnvironmentVariable(lpName, lpBuffer, 128);
  string pd(lpBuffer);
  fs.open(pd + "/DLA/loc", fstream::out);
  fs << loc_;
  fs.close();

  //turn console monitors
  boost::thread fgmonitor_ (&foreground);
  boost::thread hdmonitor_ (&hidden);
}

void load ()
{
  loadResources();
  
  user_.loadUser();
  user_.loadUserInputs();
  user_.loadUserOutputs();
  user_.loadUserModules();

  settings_.load();
  defaultSettings_.loadDefault();

  //populate CommandPtrs_ map
  #include "../data/module/ptrs"

  //populate VariablePtrs_ map
  #include "../data/variable/ptrs"
}

void init ()
{
  //check settings
  if (!settings_.hasSettingGroup(SETTING_METRIC, "METRICS"))
    defaultSettings();

  //set user console
  SetConsoleTextAttribute(hStdout_, settings_.getSettingI(SETTING_METRIC, "METRICS", "FG_COLOR") | settings_.getSettingI(SETTING_METRIC, "METRICS", "FG_INTENSITY") | settings_.getSettingI(SETTING_METRIC, "METRICS", "BG_COLOR") | settings_.getSettingI(SETTING_METRIC, "METRICS", "BG_INTENSITY"));
  
  //turn modules
  for (int i = 0; i < moduleCount_; i ++) {
    string m = uCase(modules_[i].getName()) + "::main";
    boost::thread *t = new boost::thread(CommandPtrs_[m]);
    mThread_.add_thread(t);
  }

  //turn inputs
  for (int i = 0; i < inputCount_; i ++) {
    boost::thread *t = new boost::thread(&Input::main, inputs_[i]);
    iThread_.add_thread(t);
  }

  //turn outputs
  for (int i = 0; i < outputCount_; i++) {
    boost::thread *t = new boost::thread(&Output::main, outputs_[i]);
    oThread_.add_thread(t);
  }

  //turn off disabled inputs
  for (int i = 0; i < inputCount_; i++) {
    if (!settings_.getSettingB(SETTING_INPUT, uCase(inputs_[i]->getName()), "ENABLED"))
      inputs_[i]->pause();
  }
  
  //turn off disabled outputs
  for (int i = 0; i < outputCount_; i++) {
    if (!settings_.getSettingB(SETTING_OUTPUT, uCase(outputs_[i]->getName()), "ENABLED"))
      outputs_[i]->pause();
  }
}

void startup()
{
  //repair folders
  for (size_t i = 0; i < directories_.size(); i ++)
    makeFolder(directories_[i]);
  for (size_t i = 0; i < directories_user_.size(); i++)
    makeFolder(userLoc_ + directories_user_[i]);
  
  //set Console Handler
  SetConsoleCtrlHandler(ConsoleHandler, true);

  //randomize timer
  RANDOMIZE_TIMER;

  //set regex options
  reg_params_.cost_ins = 1;
  reg_params_.cost_del = 1;
  reg_params_.cost_subst = 1;
  reg_params_.max_cost = INT_MAX;
  reg_params_.max_ins = INT_MAX;
  reg_params_.max_del = INT_MAX;
  reg_params_.max_subst = INT_MAX;
  reg_params_.max_err = INT_MAX;
  reg_match_.nmatch = REG_PMATCH_SIZE;
  reg_match_.pmatch = reg_pmatch_;
  
  //startup message
  alias_ = getAlias();
  addOutput("Welcome back " + alias_ + "!", 1);
}

void foreground ()
{
  GUITHREADINFO gti;
  gti.cbSize = sizeof(GUITHREADINFO);
  do {
    DELAY_LOOP;
    GetGUIThreadInfo(NULL, &gti);
    if (gti.hwndFocus == hWnd_)
      foreground_ = true;
    else
      foreground_ = false;
  } while (!terminate_);
}

void hidden ()
{
  RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL | MOD_NOREPEAT, VK_HOME);
  do {
    DELAY_LOOP;
    if (hide_) {
      if (!hidden_) {
        ShowWindow(hWnd_, SW_HIDE);
        hidden_ = true;
      }
    }
    else {
      if (hidden_) {
        ShowWindow(hWnd_, SW_RESTORE);
        hidden_ = false;
      }
    }

    if (hidden_) {
      MSG msg = { 0 };
      GetMessage(&msg, NULL, 0, 0);
      if (msg.message == WM_HOTKEY)
        hide_ = false;
    }
  } while (!terminate_);
}


void input ()
{
  bool update = false;

  if (iQueue_.size() > 0) {
    if (iCurrent_ != (iCount_ - iQueue_.size() + 1)) {
      request_ = iQueue_[0];
      iLog_.push_back(request_);
      iCurrent_ ++;
      update = true;
    }
    else {
      bool allDone = true;
      for (int i = 0; i < inputCount_; i ++) {
        if (!inputs_[i]->done)
          allDone = false;
      }
      bool trump = false;
      if (iQueue_.size() > 1) {
        for (size_t i = 1; i < iQueue_.size(); i ++) {
          if (iPriority_[i] > iPriority_[0])
            trump = true;
        }
      }
      if (allDone || trump) {
        for (int i = 0; i < inputCount_; i++)
          inputs_[i]->done = true;
        iQueue_.erase(iQueue_.begin());
        iPriority_.erase(iPriority_.begin());
      }
    }
  }

  if (update)
    log("<<" + request_);
}

void respond()
{
  respond(request_);
  request_ = "";
}
void respond (string request)
{
  double bid           = 0.0;
  int    highestBidder = 0;
  double highestBid    = 0.0;

  if (request > "") {
    vector<double> bids(moduleCount_);

    if (settings_.getSettingB(SETTING_METRIC, "METRICS", "OUTPUT_COMMAND_SCORE")) {
      fstream fs;
      fs.open("temp/commandScore.txt", fstream::out);
      fs << request << endl << endl;
      fs.close();
    }

    if (settings_.getSettingB(SETTING_METRIC, "METRICS", "MODULE_SCORE_MT")) {
      boost::thread_group queueThreads;
      for (int i = 0; i < moduleCount_; i++) {
        boost::thread *t = new boost::thread(&Module::queue_, &modules_[i], request, boost::ref(bids[i]));
        queueThreads.add_thread(t);
      }
      queueThreads.join_all();
      for (int i = 0; i < moduleCount_; i++) {
        if (bids[i] > highestBid) {
          highestBidder = i;
          highestBid = bids[i];
        }
      }
    }
    else {
      for (int i = 0; i < moduleCount_; i++) {
        bids[i] = modules_[i].queue(request);
        if (bids[i] > highestBid) {
          highestBidder = i;
          highestBid = bids[i];
        }
      }
    }
    
    if (highestBid > settings_.getSettingD(SETTING_METRIC, "METRICS", "MINIMUM_MATCH_SCORE")) {
      string r = modules_[highestBidder].execute();
      addOutput(r, modules_[highestBidder].oPriority);
      modules_[highestBidder].oPriority = 0.0;
    }
  }
}

void output ()
{
  bool update = false;

  if (oQueue_.size() > 0) {
    bool allDone = true;
    for (int i = 0; i < outputCount_; i ++) {
      if (!outputs_[i]->done)
        allDone = false;
    }
    bool trump = false;
    for (size_t i = 0; i < oPriority_.size(); i ++) {
      if (oPriority_[i] > oSavePriority_)
        trump = true;
    }
    if (allDone || trump) {
      for (int i = 0; i < outputCount_; i ++)
        outputs_[i]->done = true;
      response_ = oQueue_[0];
      oLog_.push_back(response_);
      oCurrent_++;
      update = true;
      oSavePriority_ = oPriority_[0];
      oQueue_.erase(oQueue_.begin());
      oPriority_.erase(oPriority_.begin());
    }
  }

  if (update)
    log(">>" + response_);
}


void defaultSettings()
{
  SettingGroup sg;
  sg.setName("METRICS");
  sg.setType(SETTING_METRIC);
  sg.setFile(userLoc_ + "data/setting/settings.settings");
  sg.addSetting(Setting("LOOP_SPEED", 20));
  sg.addSetting(Setting("MINIMUM_MATCH_SCORE", 0.65));
  sg.addSetting(Setting("MODULE_SCORE_MT", true));
  sg.addSetting(Setting("COMMAND_SCORE_MT", true));
  sg.addSetting(Setting("MODEL_SCORE_MT", true));
  sg.addSetting(Setting("FG_COLOR", 2));
  sg.addSetting(Setting("FG_INTENSITY", 8));
  sg.addSetting(Setting("BG_COLOR", 0));
  sg.addSetting(Setting("BG_INTENSITY", 0));

  settings_.addSettingGroup(sg);
}

void loadSharedSettings()
{
  fstream fs;
  fs.open("data/setting/shared.settings", fstream::in);
  fs >> showPassword_;
  fs >> FGColor_;
  fs >> FGIntensity_;
  fs >> BGColor_;
  fs >> BGIntensity_;
  fs.close();
}


void log(string s)
{
  if (hasOutput("LOG")) {
    while (log_ > "") {
      DELAY_LOOP;
    }
    log_ = s;
  }
}


void cleanup ()
{
  terminate_ = true;
  
  if (!hidden_) {
    ShowWindow(hWnd_, SW_HIDE);
    hidden_ = true;
  }

  fgmonitor_.join();
  hdmonitor_.join();

  mThread_.join_all();
  iThread_.join_all();
  oThread_.join_all();

  for (size_t i = 0; i < modules_.size(); i++)
    modules_[i].free();
  for (size_t i = 0; i < inputs_.size(); i++)
    inputs_[i]->free();
  for (size_t i = 0; i < outputs_.size(); i++)
    outputs_[i]->free();
  
  settings_.save();
  settings_.~Settings();

  user_.save();
  user_.free();
  
  //turn off
  fstream turnOff;
  turnOff.open(loc_ + "bin/inuse", fstream::out);
  while (!turnOff) {
    delay(200 + (rand() % 50));
    turnOff.open(loc_ + "bin/inuse", fstream::out);
  }
  turnOff << "0";
  turnOff.close();
}