////////////////////////////////////////////////////////////////////////////////
//File:   module.h
//Dir:    source/header/
//Date:   2014-03-29
//Author: Zachary Gill
//Interface of Module
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_MODULE_H_
#define _JARVIS_SH_MODULE_H_


//constants
#include "../resource/const.h"


//standard includes
#include <string>
#include <vector>
#include <map>


//includes
#include "command.h"


//typedefs
typedef std::string (*ComPtr)();


//class definitions
class Module
{
  public:
    //Constructors
    Module ();
    Module (std::string, std::string, double);
    Module (std::string);

    //Destructors
    ~Module();
    void free();

    //Accessors
    std::string              getName          ();
    std::string              getDescription   ();
    double                   getPriority      ();
    std::vector<std::string> getDependencies  ();
    int                      getCommandCount  ();
    std::vector<Command>     getCommands      ();
    std::vector<double>      getScores        ();
    int                      getHighestBidder ();

    //Modifiers
    void setPriority (double);

    //Other Functions
    void   addCommand (Command);
    double queue      ();
    double queue      (std::string);
    void   execute    ();
    void   dump       ();
    void   dump       (std::ostream&);
    void   dump       (std::string);
    void   dump       (std::ostream&, std::string);

  private:
    std::string              name;
    std::string              description;
    double                   priority;
    std::vector<std::string> dependencies;
    int                      commandCount;
    std::vector<Command>     commands;
    std::vector<double>      scores;
    int                      highestBidder;
} ;


//global variable definitions
extern int                           moduleCount_;
extern std::string                   request_;
extern std::string                   response_;
extern std::vector<Module>           modules_;
extern std::map<std::string, ComPtr> CommandPtrs_;


//function definitions
void loadModules ();
bool hasModule   (std::string);


//shared function definitions
#include "../resource/common.h"


#endif