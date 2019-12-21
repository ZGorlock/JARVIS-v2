////////////////////////////////////////////////////////////////////////////////
//File:   core_M.h
//Dir:    source/module/
//Date:   2015-01-23
//Author: Zachary Gill
//Interface of CORE module
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SM_CORE_H_
#define _DLA_SM_CORE_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <vector>


//includes
#include "module.h"
#include "input.h"
#include "output.h"
#include "settings.h"


//namespace definitions
namespace CORE
{
  std::string main           ();

  std::string sayhello       ();
  std::string add            ();

  std::string clear          ();

  std::string enumSettings   ();
  std::string resetSettings  ();
  std::string revertSettings ();

  std::string log            ();
  std::string clearLog       ();

  std::string enumModules    ();
  std::string countModules   ();
  std::string enumCommands   ();
  std::string countCommands  ();
  std::string enumInputs     ();
  std::string countInputs    ();
  std::string enumOutputs    ();
  std::string countOutputs   ();

  std::string hide           ();
  std::string show           ();

  std::string terminate      ();

  namespace PRIVATE
  {
    void defaultSettings     ();
    void loadResources       ();
  } ;
} ;


//global variable definitions
extern bool                     terminate_;
extern bool                     hide_;
extern bool                     hidden_;
extern std::string              log_;
extern std::string              userLoc_;
extern int                      moduleCount_;
extern int                      inputCount_;
extern int                      outputCount_;
extern int                      variableCount_;
extern HANDLE                   hStdout_;
extern HWND                     hWnd_;
extern std::vector<Module>      modules_;
extern std::vector<Input*>      inputs_;
extern std::vector<Output*>     outputs_;
extern std::vector<std::string> variables_;
extern std::vector<std::string> variableTypes_;
extern Settings                 settings_;


#endif