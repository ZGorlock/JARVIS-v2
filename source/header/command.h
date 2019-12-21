////////////////////////////////////////////////////////////////////////////////
//File:   command.h
//Dir:    source/header/
//Date:   201-01-04
//Author: Zachary Gill
//Interface of Command
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_COMMAND_H_
#define _DLA_SH_COMMAND_H_


//constants
#include "const.h"


//standard includes
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "tre/regex.h"


//includes
#include "settings.h"


//typedefs
typedef double (*VarPtr)(std::string);


//class definitions
class Command
{  
  public:
    class Model
    {
      public:
        std::string              match;
        std::string              regex;
        double                   priority;
        int                      commandType;
        int                      questionType;
        int                      question;
        std::vector<std::string> vars;
        std::vector<std::string> varTypes;
        regex_t                  reg;
        int                      regHr;

        void dump () const;
        void dump (std::ostream&) const;
        void dump (std::string) const;
        void dump (std::ostream&, std::string) const;
    } ;

    //Constructors
    Command ();
    Command (std::string, std::string, std::string, int, std::string, double, double, std::string, double);
    Command (std::string);
    Command (std::fstream&);

    //Destructors
    ~Command  ();
    void free ();

    //Accessors
    std::string              getName           () const;
    std::string              getDescription    () const;
    std::string              getDefinition     () const;
    int                      getReturnType     () const;
    std::string              getModule         () const;
    double                   getModulePriority () const;
    double                   getPriority       () const;
    int                      getModelCount     () const;
    std::vector<Model>       getModels         () const;
    std::string              getOutput         () const;
    double                   getRisk           () const;

    std::vector<std::string> getVars           () const;
    std::vector<std::string> getVarTypes       () const;

    //Modifiers
    void setName           (std::string);
    void setDescription    (std::string);
    void setDefinition     (std::string);
    void setReturnType     (int);
    void setModule         (std::string);
    void setModulePriority (double);
    void setPriority       (double);
    void addModel          (Model);
    void removeModel       (int);
    void setOutput         (std::string);
    void setRisk           (double);

    //Other Functions
    void   addModel       (std::string, double, int, int, int);
    void   clearVars      ();
    void   clearModelVars (int);
    double score          ();
    double score          (std::string);
    double scoreModel     (std::string, std::string, int);
    void   score_         (std::string, double&);
    void   scoreModel_    (std::string, std::string, int, double&);
    void   dump           () const;
    void   dump           (std::ostream&) const;
    void   dump           (std::string) const;
    void   dump           (std::ostream&, std::string) const;

  private:
    std::string              name;
    std::string              description;
    std::string              definition;
    int                      returnType;
    std::string              module;
    double                   modulePriority;
    double                   priority;
    int                      modelCount;
    std::vector<Model>       models;
    std::string              output;
    double                   risk;
    std::vector<std::string> vars;
    std::vector<std::string> varTypes;
    
    void        readCommandFromFileStream (std::fstream&);
    std::string buildRegex(std::string);
} ;


//global variable definitions
extern std::string                   request_;
extern int                           commandCount_;
extern std::vector<std::string>      variables_;
extern regaparams_t                  reg_params_;
extern regmatch_t                    reg_pmatch_[REG_PMATCH_SIZE];
extern regamatch_t                   reg_match_;
extern std::map<std::string, VarPtr> VariablePtrs_;
extern Settings                      settings_;


//shared function definitions
#include "common.h"


#endif