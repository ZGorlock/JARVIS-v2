#include <iostream>
#include <string>
#include <Windows.h>
//#include <msclr/marshal_cppstd.h>

//properties/add reference/assemblies/framework/system.speech
//enable /clr
//diable /z1 /gm /rtc1

#include "testing.h"

#ifdef __SpeechRecognitionNet__

using namespace std;
using namespace System;
using namespace System::Speech::Recognition;

void recoEngine_SpeechRecognized(Object^, SpeechRecognizedEventArgs^);

string speech;

int main()
{
  //create speech recognition engine
  SpeechRecognitionEngine^ recoEngine = gcnew SpeechRecognitionEngine();

  

  //create grammars
  DictationGrammar^ defaultDictationGrammar = gcnew DictationGrammar();
  defaultDictationGrammar->Name = "default dictation";
  defaultDictationGrammar->Enabled = true;
  DictationGrammar^ spellingDictationGrammar = gcnew DictationGrammar("grammar:dictation#spelling");
  spellingDictationGrammar->Name = "spelling dictation";
  spellingDictationGrammar->Enabled = true;
  recoEngine->LoadGrammarAsync(defaultDictationGrammar);
  recoEngine->LoadGrammarAsync(spellingDictationGrammar);

  

  //set event handler
  recoEngine->SpeechRecognized += gcnew EventHandler<SpeechRecognizedEventArgs^>(recoEngine_SpeechRecognized);



  //set audio input
  recoEngine->SetInputToDefaultAudioDevice();



  //start recognizer
  recoEngine->RecognizeAsync(RecognizeMode::Multiple);


  
  while (true) {
    if (speech > "") {
      cout << speech << endl;
      speech = "";
    }
  }


  system("PAUSE");
  return 0;
}

void recoEngine_SpeechRecognized(Object^ sender, SpeechRecognizedEventArgs^ e)
{
  speech = msclr::interop::marshal_as<std::string>(e->Result->Text);
}

#endif