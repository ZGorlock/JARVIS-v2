////////////////////////////////////////////////////////////////////////////////
//File:   command.h
//Dir:    source/header/
//Date:   2014-03-18
//Author: Zachary Gill
//Interface of Command
////////////////////////////////////////////////////////////////////////////////


#ifndef _JARVIS_SH_COMMAND_H_
#define _JARVIS_SH_COMMAND_H_


//constants
#include "../resource/const.h"


//includes
#include <string>
#include <vector>


//namespaces
using namespace std;


//class definitions
class Command
{  
  public:
    class Model
    {
      public:
        string match;
        int commandType;
        int questionType;
        int question;
    } ;

    //Constructors
    Command ();
    Command (string, string, string, int, string, double, string, double);
    Command (string);
    Command (fstream&);

    //Destructors
    ~Command();

    //Accessors
    string        getName         ();
    string        getDescription  ();
    string        getDefintion    ();
    int           getReturnType   ();
    string        getModule       ();
    double        getPriority     ();
    int           getModelCount   ();
    vector<Model> getModels       ();
    string        getOutput       ();
    double        getRisk         ();

    //Modifiers
    void setPriority (double);
    void setRisk     (double);

    //Other Functions
    void addModel (string, int, int, int);
    void dump        ();
    void dump        (ostream&);
    void dump        (string);
    void dump        (ostream&, string);

  private:
    string        name;
    string        description;
    string        definition;
    int           returnType;
    string        module;
    double        priority;
    int           modelCount;
    vector<Model> models;
    string        output;
    double        risk;
    
    void readCommandFromFileStream(fstream&);
} ;


//global variable definitions
extern int commandCount_;


//global function defintions
extern void getint(fstream&, int&);
extern void getdouble(fstream&, double&);


#endif