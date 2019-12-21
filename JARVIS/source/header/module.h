////////////////////////////////////////////////////////////////////////////////
//File:   module.h
//Dir:    source/header/
//Date:   2014-03-19
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


//includes
#include "command.h"


//namespaces
using namespace std;


//class definitions
class Module
{
  public:
    //Constructors
    Module ();
    Module (string, string, double);
    Module (string);

    //Destructors
    ~Module();

    //Accessors
    string          getName         ();
    string          getDescription  ();
    double          getPriority     ();
    vector<string>  getDependencies ();
    int             getCommandCount ();
    vector<Command> getCommands     ();
    vector<double>  getScores       ();

    //Modifiers
    void setPriority (double);

    //Other Functions
    void addCommand  (Command);
    void queueScores ();
    void dump        ();
    void dump        (ostream&);
    void dump        (string);
    void dump        (ostream&, string);

  private:
    string          name;
    string          description;
    double          priority;
    vector<string>  dependencies;
    int             commandCount;
    vector<Command> commands;
    vector<double>  scores;
} ;


//global variable definitions
extern int moduleCount_;
extern vector<Module> modules_;


//function definitions
void loadModules();


//global function defintions
extern bool fileExists(string name);
extern void getint(fstream&, int&);
extern void getdouble(fstream&, double&);


#endif