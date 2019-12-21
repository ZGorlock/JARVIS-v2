////////////////////////////////////////////////////////////////////////////////
//File:   speech_O.h
//Dir:    source/output/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of SpeechOutput
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SO_SPEECH_H_
#define _DLA_SO_SPEECH_H_


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <sapi.h>


//includes
#include "output.h"


//class definitions
class SpeechOutput : public AudioOutput
{
  public:
    SpeechOutput                        ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectOutput   ();
    virtual inline void free            ();

  private:
    HRESULT   hr;
    ISpVoice* pVoice;
};


//global variable definitions
extern int                  gender_;
extern bool                 terminate_;
extern std::string          response_;
extern std::string          loc_;
extern int                  oCurrent_;
extern double               oSavePriority_;


//shared function definitions
#include "common.h"


#endif