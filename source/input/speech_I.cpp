////////////////////////////////////////////////////////////////////////////////
//File:   speech_I.cpp
//Dir:    source/input/
//Date:   2015-01-03
//Author: Zachary Gill
//Implementation of SpeechInput
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>
#include "SAPI/sphelper.h"


//includes
#include "speech_I.h"
#include "settings.h"
#include "timer.h"


//namespaces
using namespace std;


//Constructors

SpeechInput::SpeechInput ()
{
  SpeechInput::name = "SPEECH";
  SpeechInput::console = true;
}


//Destructors

void SpeechInput::free ()
{
  SpeechInput::bufferTimer.~Timer();
  SpeechInput::audio.Release();
  SpeechInput::recoGrammar.Release();
  SpeechInput::recoContext.Release();
  SpeechInput::recoEngine.Release();
  Input::~Input();
}


//Other Functions

void SpeechInput::main ()
{
#ifdef _WIN32_WCE
  if (SUCCEEDED(::CoInitializeEx(NULL,COINIT_MULTITHREADED)))
#else
  if (SUCCEEDED(::CoInitialize(NULL)))
#endif
  {
    SpeechInput::init();
    if (SpeechInput::buildSuccess) {

      do {
        DELAY_LOOP;
        if (SpeechInput::bufferTimer.time() >= 2.0)
          SpeechInput::clearBuffer();
        if (SpeechInput::active) {
          if (SpeechInput::collectInput()) {
            SpeechInput::done = false;
            addInput(SpeechInput::buffer);
            SpeechInput::done = true;
          }
        }
      } while (!terminate_);

    }
    ::CoUninitialize();
    SpeechInput::free();
  }
}

void SpeechInput::init ()
{
  if (!settings_.hasSettingGroup(SETTING_INPUT, "SPEECH"))
    SpeechInput::defaultSettings();

  HRESULT hr = S_FALSE;

  //create engine
  hr = SpeechInput::recoEngine.CoCreateInstance(CLSID_SpInprocRecognizer);

  //create context
  if (SUCCEEDED(hr)) {
    hr = SpeechInput::recoEngine->CreateRecoContext(&(SpeechInput::recoContext));
    //set context notifier
    if (SUCCEEDED(hr)) {
      //hr = SpeechInput::recoContext->SetNotifyCallbackFunction(SpeechInput::recoEvent, 0, LPARAM(this));//SetNotifyWindowMessage( SpeechInput::hWnd, WM_RECOEVENT, 0, 0 );
      //set notifier interest
      //if (SUCCEEDED(hr)) {
      //  const ULONGLONG interest = SPFEI(SPEI_RECOGNITION);
      //  hr = SpeechInput::recoContext->SetInterest(interest, interest);
      //}
    }
  }
  
  //create audio object
  if (SUCCEEDED(hr)) {
    hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &(SpeechInput::audio));
    //set audio object as engine input
    if (SUCCEEDED(hr)) {
      hr = SpeechInput::recoEngine->SetInput(SpeechInput::audio, TRUE);
    }
  }
  
  //activate engine
  if (SUCCEEDED(hr))
    hr = SpeechInput::recoEngine->SetRecoState(SPRST_ACTIVE);

  //create grammar
  if (SUCCEEDED(hr))  {
    hr = SpeechInput::recoContext->CreateGrammar(GID_DICTATION, &(SpeechInput::recoGrammar));
    //load dictation grammar
    if (SUCCEEDED(hr)) {
      hr = SpeechInput::recoGrammar->LoadDictation(NULL, SPLO_STATIC);
      //activate grammar
      if (SUCCEEDED(hr))
        hr = SpeechInput::recoGrammar->SetDictationState(SPRS_ACTIVE);
    }
    //if failed, free grammar
    if (FAILED(hr))
      SpeechInput::recoGrammar.Release();
  }

  SpeechInput::buildSuccess = (hr == S_OK);
  return;
}

void SpeechInput::defaultSettings()
{
  SettingGroup sg;
  sg.setName("SPEECH");
  sg.setType(SETTING_INPUT);
  sg.setFile(userLoc_ + "data/setting/input/speech.settings");
  sg.addSetting(Setting("ENABLED", true));
  settings_.addSettingGroup(sg);
}

bool SpeechInput::collectInput ()
{
  CSpEvent event;
  bool initialGet = true;
  while (event.GetFrom(SpeechInput::recoContext) == S_OK) {
    if (event.eEventId == SPEI_RECOGNITION) {
      if (initialGet) {
        SpeechInput::clearBuffer();
        initialGet = false;
      }
      CSpDynamicString dstrText;
      if (FAILED(event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL)))
        dstrText = "";
      if (SpeechInput::buffer > "")
        SpeechInput::buffer += " ";
      SpeechInput::buffer += dstrText.CopyToChar();
      SpeechInput::bufferTimer.start();
      return true;
    }
  }
  return false;
}

void SpeechInput::start()
{
  SpeechInput::recoGrammar->SetDictationState(SPRS_ACTIVE);
}

void SpeechInput::pause()
{
  SpeechInput::recoGrammar->SetDictationState(SPRS_INACTIVE);
}