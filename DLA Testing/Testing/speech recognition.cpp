#include "stdafx.h"

#include <iostream>
#include <string>

#include "sphelper.h"
#include "speech recognition.h"

using namespace std;

int main()
{
#ifdef _WIN32_WCE
  if (SUCCEEDED(::CoInitializeEx(NULL,COINIT_MULTITHREADED)))
#else
  if (SUCCEEDED(::CoInitialize(NULL)))
#endif
  {
    SR sr;
    if (sr.initSR()) {

      bool exit = false;
      do {
        if (sr.getRecoEvent()) {
          if (sr.speech > "") {
            cout << sr.speech << endl;
            if (sr.speech == "exit" || sr.speech == "Exit")
              exit = true;
            sr.speech = "";
          }
        }
      } while (!exit);

    }

    ::CoUninitialize();
  }

  system("PAUSE");
  return 0;
}

SR::SR()
{
}

SR::~SR()
{
  SR::audio.Release();
  SR::recoGrammar.Release();
  SR::recoContext.Release();
  SR::recoEngine.Release();
}

bool SR::initSR()
{
  HRESULT hr = S_FALSE;

  //create engine
  hr = SR::recoEngine.CoCreateInstance(CLSID_SpInprocRecognizer);

  //create context
  if (SUCCEEDED(hr)) {
    hr = SR::recoEngine->CreateRecoContext(&(SR::recoContext));
    //set context notifier
    if (SUCCEEDED(hr)) {
      //hr = SR::recoContext->SetNotifyCallbackFunction(SR::recoEvent, 0, LPARAM(this));//SetNotifyWindowMessage( SR::hWnd, WM_RECOEVENT, 0, 0 );
      //set notifier interest
      //if (SUCCEEDED(hr)) {
      //  const ULONGLONG interest = SPFEI(SPEI_RECOGNITION);
      //  hr = SR::recoContext->SetInterest(interest, interest);
      //}
    }
  }
  
  //create audio object
  if (SUCCEEDED(hr)) {
    hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &(SR::audio));
    //set audio object as engine input
    if (SUCCEEDED(hr)) {
      hr = SR::recoEngine->SetInput(SR::audio, TRUE);
    }
  }

  //activate engine
  if (SUCCEEDED(hr))
    hr = SR::recoEngine->SetRecoState(SPRST_ACTIVE);

  //create grammar
  if (SUCCEEDED(hr))  {
    hr = SR::recoContext->CreateGrammar(GID_DICTATION, &(SR::recoGrammar));
    //load dictation grammar
    if (SUCCEEDED(hr)) {
      hr = SR::recoGrammar->LoadDictation(NULL, SPLO_STATIC);
      //activate grammar
      if (SUCCEEDED(hr))
        hr = SR::recoGrammar->SetDictationState(SPRS_ACTIVE);
    }
    //if failed, free grammar
    if (FAILED(hr))
      SR::recoGrammar.Release();
  }

  return (hr == S_OK);
}

bool SR::getRecoEvent()
{
  CSpEvent event;
  while (event.GetFrom(SR::recoContext) == S_OK)
  {
    if (event.eEventId == SPEI_RECOGNITION) {
      CSpDynamicString dstrText;
      if (FAILED(event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL)))
        dstrText = "";
      SR::speech = dstrText.CopyToChar();
      return true;
    }
  }
  return false;
}
