#include "stdafx.h"
#include "sphelper.h"
#include <string>

#define GID_DICTATION 0
#define WM_RECOEVENT  WM_APP

class SR
{
public:
  SR();
  ~SR();

  bool initSR();
  bool getRecoEvent();
  
  CComPtr<ISpRecognizer>  recoEngine;
  CComPtr<ISpRecoContext> recoContext;
  CComPtr<ISpRecoGrammar> recoGrammar;  
  CComPtr<ISpAudio>       audio;

  std::string             speech;
};
