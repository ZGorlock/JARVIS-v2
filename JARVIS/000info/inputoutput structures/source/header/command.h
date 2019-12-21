////////////////////////////////////////////////////////////////////////////////
//File:   command.h
//Dir:    source/header/
//Date:   2014-03-29
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


//class definitions
class Command
{  
  public:
    class Model
    {
      public:
        std::string match;
        int         commandType;
        int         questionType;
        int         question;
    } ;

    //Constructors
    Command ();
    Command (std::string, std::string, std::string, int, std::string, double, std::string, double);
    Command (std::string);
    Command (std::fstream&);

    //Destructors
    ~Command  ();
    void free ();

    //Accessors
    std::string        getName         ();
    std::string        getDescription  ();
    std::string        getDefintion    ();
    int                getReturnType   ();
    std::string        getModule       ();
    double             getPriority     ();
    int                getModelCount   ();
    std::vector<Model> getModels       ();
    std::string        getOutput       ();
    double             getRisk         ();

    //Modifiers
    void setPriority (double);
    void setRisk     (double);

    //Other Functions
    void   addModel (std::string, int, int, int);
    double score    ();
    double score    (std::string);
    void   dump     ();
    void   dump     (std::ostream&);
    void   dump     (std::string);
    void   dump     (std::ostream&, std::string);

  private:
    std::string        name;
    std::string        description;
    std::string        definition;
    int                returnType;
    std::string        module;
    double             priority;
    int                modelCount;
    std::vector<Model> models;
    std::string        output;
    double             risk;
    
    void readCommandFromFileStream(std::fstream&);
} ;


//global variable definitions
extern int         commandCount_;
extern std::string request_;


//shared function definitions
#include "../resource/common.h"


#endif