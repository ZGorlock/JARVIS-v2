////////////////////////////////////////////////////////////////////////////////
//File:   speech_O.cpp
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of SpeechOutput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <sapi.h>
#include <SAPI/sphelper.h>


//includes
#include "speech_O.h"
#include "settings.h"


//namespaces
using namespace std;


//Constructors

SpeechOutput::SpeechOutput()
{
  SpeechOutput::name = "SPEECH";
}


//Destructors

inline void SpeechOutput::free()
{
  SpeechOutput::pVoice->Pause();
  SpeechOutput::pVoice->Release();
  SpeechOutput::pVoice = NULL;
  ::CoUninitialize();
  Output::~Output();
}


//Other Functions

void SpeechOutput::main()
{
  SpeechOutput::init();

  if (FAILED(SpeechOutput::hr))
    return;
  SPVOICESTATUS pStatus;

  do {
    DELAY_LOOP;
    if (SpeechOutput::active) {

      if (SpeechOutput::collectOutput()) {
        SpeechOutput::hr = SpeechOutput::pVoice->Speak(stringToWideString(TextOutput::getBuffer()).c_str(), SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
        TextOutput::clearBuffer();
        if (FAILED(SpeechOutput::hr)) {
          Output::done = true;
          AudioOutput::speaking = false;
        }
      }

      pVoice->GetStatus(&pStatus, NULL);
      if (pStatus.dwRunningState == SPRS_IS_SPEAKING) {
        AudioOutput::speaking = true;
        Output::done = false;
      }
      else {
        AudioOutput::speaking = false;
        Output::done = true;
      }

    }
  } while (!terminate_);
  SpeechOutput::free();
}

inline void SpeechOutput::init()
{
  if (!settings_.hasSettingGroup(SETTING_OUTPUT, "SPEECH"))
    SpeechOutput::defaultSettings();

  SpeechOutput::pVoice = NULL;
  SpeechOutput::hr = ::CoInitialize(NULL);
  if (FAILED(SpeechOutput::hr))
    return;

  SpeechOutput::hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
  if (FAILED(SpeechOutput::hr))
    return;

  if (gender_ > GENDER_DEFAULT) {
    WCHAR* szRequiredAttribs = L"";
    if (gender_ == GENDER_FEMALE)
      szRequiredAttribs = L"gender=female";
    else
      szRequiredAttribs = L"gender=male";
    WCHAR* szOptAttribs = L"";
    ISpObjectToken* cpToken(NULL);
    SpFindBestToken(SPCAT_VOICES, szRequiredAttribs, szOptAttribs, &cpToken);
    SpeechOutput::pVoice->SetVoice(cpToken);
  }

  SpeechOutput::pVoice->SetVolume(100);

  AudioOutput::currentOutput = oCurrent_;
}

inline void SpeechOutput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("SPEECH");
  sg.setType(SETTING_OUTPUT);
  sg.setFile(userLoc_ + "data/setting/output/speech.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

inline bool SpeechOutput::collectOutput()
{
  if (response_ > "" && AudioOutput::currentOutput != oCurrent_) {
    TextOutput::clearBuffer();
    TextOutput::setBuffer(response_);
    AudioOutput::currentOutput ++;
    Output::done = false;
    if (oSavePriority_ < .2) {
      TextOutput::clearBuffer();
      Output::done = true;
    }
    return true;
  }
  return false;
}