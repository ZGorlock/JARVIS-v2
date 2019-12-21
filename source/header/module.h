////////////////////////////////////////////////////////////////////////////////
//File:   module.h
//Dir:    source/header/
//Date:   2014-12-31
//Author: Zachary Gill
//Interface of Module
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_MODULE_H_
#define _DLA_SH_MODULE_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <vector>
#include <map>


//includes
#include "command.h"


//typedefs
typedef std::string (*CmdPtr)();


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
    std::string                            getName                   () const;
    std::string                            getAuthor                 () const;
    std::string                            getDescription            () const;
    double                                 getPriority               () const;
    std::vector<std::string>               getKeywords               () const;
    std::vector<std::string>               getDependencies           () const;
    std::vector<std::string>               getVariables              () const;
    std::vector<std::string>               getResourceFiles          () const;
    std::vector<std::string>               getDataFiles              () const;
    std::vector<std::string>               getSolutionFolders        () const;
    std::vector<std::string>               getSolutionFiles          () const;
    std::vector<int>                       getSolutionPropertyCounts () const;
    std::vector<std::vector<std::string> > getSolutionProperties     () const;
    int                                    getCommandCount           () const;
    std::vector<Command>                   getCommands               () const;
    int                                    getHighestBidder          () const;

    //Modifiers
    void setName          (std::string);
    void setAuthor        (std::string);
    void setDescription   (std::string);
    void setPriority      (double);
    void addKeyword       (std::string);
    void removeKeyword    (int);
    void addDependency    (std::string);
    void removeDependency (int);
    void addVariable      (std::string);
    void removeVariable   (int);
    void addSolutionFolder (std::string);
    void removeSolutionFolder(int);
    void addSolutionFile(std::string);
    void removeSolutionFile(int);
    void addSolutionProperty(int, std::string);
    void removeSolutionProperty(int, int);
    void addCommand       (Command);
    void removeCommand    (int);


    //Other Functions
    bool        hasKeyword (std::string) const;
    double      queue      ();
    double      queue      (std::string);
    void        queue_     (std::string, double&);
    std::string execute    () const;
    void        dump       () const;
    void        dump       (std::ostream&) const;
    void        dump       (std::string) const;
    void        dump       (std::ostream&, std::string) const;

    double oPriority;

  private:
    std::string                            name;
    std::string                            author;
    std::string                            description;
    double                                 priority;
    std::vector<std::string>               keywords;
    std::vector<std::string>               dependencies;
    std::vector<std::string>               variables;
    std::vector<std::string>               resourceFiles;
    std::vector<std::string>               dataFiles;
    std::vector<std::string>               solutionFolders;
    std::vector<std::string>               solutionFiles;
    std::vector<int>                       solutionPropertyCounts;
    std::vector<std::vector<std::string> > solutionProperties;
    int                                    commandCount;
    std::vector<Command>                   commands;

    int                                    highestBidder;
} ;


//global variable definitions
extern int                           moduleCount_;
extern std::string                   request_;
extern std::string                   response_;
extern int                           variableCount_;
extern std::vector<Module>           modules_;
extern std::vector<std::string>      variables_;
extern std::vector<std::string>      variableTypes_;
extern std::vector<std::string>      moduleList_;
extern std::vector<std::string>      variableList_;
extern std::map<std::string, CmdPtr> CommandPtrs_;


//resources
#include "modules"


//function definitions
void    loadModules  ();
void    loadModules  (std::string);
bool    hasModule    (std::string);
void    setOPriority (std::string, double);


//shared function definitions
#include "common.h"


#endif