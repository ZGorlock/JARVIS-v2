////////////////////////////////////////////////////////////////////////////////
//File:   speech_I.h
//Dir:    source/input/
//Date:   2015-01-03
//Author: Zachary Gill
//Interface of SpeechInput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SI_SPEECH_H_
#define _DLA_SI_SPEECH_H_


//constants
#include "const.h"

#define GID_DICTATION 0
#define WM_RECOEVENT  WM_APP


//standard includes
#include "stdafx.h"
#include <string>
#include <vector>
#include "SAPI/sphelper.h"


//includes
#include "input.h"
#include "settings.h"
#include "timer.h"


//class definitions
class SpeechInput : public AudioInput
{
  public:
    SpeechInput                         ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectInput    ();
    virtual void        start           ();
    virtual void        pause           ();
    virtual void        free            ();

  private:
    CComPtr<ISpRecognizer>  recoEngine;
    CComPtr<ISpRecoContext> recoContext;
    CComPtr<ISpRecoGrammar> recoGrammar;  
    CComPtr<ISpAudio>       audio;

    bool  buildSuccess;
    Timer bufferTimer;
} ;


//global variable definitions
extern bool                     terminate_;
extern Settings                 settings_;


#endif