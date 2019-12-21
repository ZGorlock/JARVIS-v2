////////////////////////////////////////////////////////////////////////////////
//File:   module.cpp
//Dir:    source/
//Date:   2015-05-10
//Author: Zachary Gill
//Implementation of Module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include <map>

#include <boost/thread/thread.hpp>


//includes
#include "module.h"
#include "variable.h"


//namespaces
using namespace std;


//Constructors

Module::Module ()
{
  Module::name = "";
  Module::description = "";
  Module::priority = 0.0;
  Module::commandCount = 0;

  moduleCount_ ++;
}
Module::Module (string name, string description, double priority)
{
  Module::name = name;
  Module::description = description;
  Module::priority = priority;
  Module::commandCount = 0;

  Module::priority /= 100;

  moduleCount_ ++;
}
Module::Module (string modfile)
{
  modfile = "data/" + modfile + ".module";
  if (fileExists(modfile)) {
    fstream moduleFile;
    moduleFile.open(modfile.c_str(), fstream::in);

    if (moduleFile.is_open()) {
      string tmp = "";
      
      getline  (moduleFile, Module::name);
      getline  (moduleFile, Module::author);
      getline  (moduleFile, Module::description);
      getdouble(moduleFile, Module::priority);
      
      getline  (moduleFile, tmp);
      stringstream ss0(tmp);
      string token0;
      while (getline(ss0, token0, ','))
        Module::keywords.push_back(uCase(token0));

      getline  (moduleFile, tmp);
      stringstream ss1(tmp);
      string token1;
      while (getline(ss1, token1, ','))
        Module::dependencies.push_back(uCase(token1));
      
      getline  (moduleFile, tmp);
      stringstream ss2(tmp);
      string token2;
      while (getline(ss2, token2, ','))
        Module::variables.push_back(uCase(token2));
      
      getline  (moduleFile, tmp);
      stringstream ss3(tmp);
      string token3;
      while (getline(ss3, token3, ','))
        Module::resourceFiles.push_back(uCase(token3));
      
      getline  (moduleFile, tmp);
      stringstream ss4(tmp);
      string token4;
      while (getline(ss4, token4, ','))
        Module::dataFiles.push_back(uCase(token4));
      
      getline  (moduleFile, tmp);
      stringstream ss5(tmp);
      string token5;
      while (getline(ss5, token5, ','))
        Module::solutionFolders.push_back(uCase(token5));

      getline  (moduleFile, tmp);
      stringstream ss6(tmp);
      string token6;
      while (getline(ss6, token6, ','))
        Module::solutionFiles.push_back(uCase(token6));

      int nextSolutionProperty = 0;
      int tmpSolutionPropertyCount = 0;
      getint(moduleFile, nextSolutionProperty);
      while (nextSolutionProperty != 0) {
        getint(moduleFile, tmpSolutionPropertyCount);
        Module::solutionPropertyCounts.push_back(tmpSolutionPropertyCount);
        Module::solutionProperties.push_back(vector<string>{});
        for (int i = 0; i < tmpSolutionPropertyCount; i++) {
          string tmpSolutionProperty = "";
          getline(moduleFile, tmpSolutionProperty);
          Module::solutionProperties[nextSolutionProperty - 1].push_back(tmpSolutionProperty);
        }
        getint(moduleFile, nextSolutionProperty);
      }

      Module::commandCount = 0;
      int nextCommand = 0;
      getint(moduleFile, nextCommand);
      while (nextCommand != 0) {
        Module::commandCount = nextCommand;

        Command tmpCommand (moduleFile);
        Module::commands.push_back(tmpCommand);

        getint(moduleFile, nextCommand);
      }

      moduleFile.close();
    }
  }
  else {
    Module::name = "";
    Module::description = "";
    Module::priority = 0.0;
    Module::commandCount = 0;
  }

  Module::priority /= 100;

  moduleCount_ ++;
}


//Destructors

Module::~Module ()
{
}

void Module::free ()
{
  for (size_t i = 0; i < Module::commands.size(); i ++)
    Module::commands[i].free();

  Module::~Module();
  moduleCount_ --;
}


//Accessors

string Module::getName () const
{
  return Module::name;
}

string Module::getAuthor() const
{
  return Module::author;
}

string Module::getDescription () const
{
  return Module::description;
}

double Module::getPriority () const
{
  return Module::priority;
}

vector<string> Module::getKeywords () const
{
  return Module::keywords;
}

vector<string> Module::getDependencies () const
{
  return Module::dependencies;
}

vector<string> Module::getVariables() const
{
  return Module::variables;
}

vector<string> Module::getResourceFiles () const
{
  return Module::resourceFiles;
}

vector<string> Module::getDataFiles() const
{
  return Module::dataFiles;
}

vector<string> Module::getSolutionFolders () const
{
  return Module::solutionFolders;
}

vector<string> Module::getSolutionFiles() const
{
  return Module::solutionFiles;
}

vector<int> Module::getSolutionPropertyCounts () const
{
  return Module::solutionPropertyCounts;
}

vector<vector<string> > Module::getSolutionProperties() const
{
  return Module::solutionProperties;
}

int Module::getCommandCount () const
{
  return Module::commandCount;
}

vector<Command> Module::getCommands () const
{
  return Module::commands;
}

int Module::getHighestBidder () const
{
  return Module::highestBidder;
}


//Modifiers

void Module::setName(string name)
{
  Module::name = name;
  for (int i = 0; i < Module::commandCount; i++)
    Module::commands[i].setModule(name);
}

void Module::setAuthor(string author)
{
  Module::author = author;
}

void Module::setDescription(string description)
{
  Module::description = description;
}

void Module::setPriority (double priority)
{
  Module::priority = priority;
  for (int i = 0; i < Module::commandCount; i++)
    Module::commands[i].setModulePriority(priority);
}

void Module::addKeyword(string keyword)
{
  Module::keywords.push_back(keyword);
}

void Module::removeKeyword(int index)
{
  Module::keywords.erase(Module::keywords.begin() + index);
}

void Module::addDependency(string dependency)
{
  Module::dependencies.push_back(dependency);
}

void Module::removeDependency(int index)
{
  Module::dependencies.erase(Module::dependencies.begin() + index);
}

void Module::addVariable(string variable)
{
  Module::variables.push_back(variable);
}

void Module::removeVariable(int index)
{
  Module::variables.erase(Module::variables.begin() + index);
}

void Module::addSolutionFolder(string folder)
{
  Module::solutionFolders.push_back(folder);
}

void Module::removeSolutionFolder(int index)
{
  Module::solutionFolders.erase(Module::solutionFolders.begin() + index);
}

void Module::addSolutionFile(string file)
{
  Module::solutionFiles.push_back(file);
  vector<string> tmps;
  Module::solutionProperties.push_back(tmps);
  Module::solutionPropertyCounts.push_back(0);
}

void Module::removeSolutionFile(int index)
{
  Module::solutionFiles.erase(Module::solutionFiles.begin() + index);
  Module::solutionProperties.erase(Module::solutionProperties.begin() + index + 2);
  Module::solutionPropertyCounts.erase(Module::solutionPropertyCounts.begin() + index + 2);
}

void Module::addSolutionProperty(int index, string prop)
{
  Module::solutionProperties[index].push_back(prop);
  Module::solutionPropertyCounts[index]++;
}

void Module::removeSolutionProperty(int index, int index2)
{
  Module::solutionProperties[index].erase(Module::solutionProperties[index].begin() + index2);
  Module::solutionPropertyCounts[index]--;
}

void Module::addCommand (Command command)
{
  Module::commandCount ++;
  Module::commands.push_back(command);
}

void Module::removeCommand(int index)
{
  Module::commandCount --;
  Module::commands.erase(Module::commands.begin() + index);
}



//Other Functions

bool Module::hasKeyword(string keyword) const
{
  keyword = uCase(keyword);
  for (size_t i = 0; i < Module::keywords.size(); i++) {
    if (Module::keywords[i] == keyword)
      return true;
  }
  return false;
}

double Module::queue ()
{
  return Module::queue(request_);
}
double Module::queue (string query)
{
  Module::highestBidder = 0;
  double bid            = 0.0;
  double highestBid     = 0.0;

  vector<double> bids(Module::commandCount);

  fstream fs;
  bool output_command_score = settings_.getSettingB(SETTING_METRIC, "METRICS", "OUTPUT_COMMAND_SCORE");
  if (output_command_score)
    fs.open("temp/commandScore.txt", fstream::app);
  
  if (settings_.getSettingB(SETTING_METRIC, "METRICS", "COMMAND_SCORE_MT")) {
    boost::thread_group scoreThreads;
    for (int i = 0; i < Module::commandCount; i++) {
      boost::thread *t = new boost::thread(&Command::score_, &Module::commands[i], query, boost::ref(bids[i]));
      scoreThreads.add_thread(t);
    }
    scoreThreads.join_all();

    for (int i = 0; i < Module::commandCount; i++) {
      if (output_command_score)
        fs << Module::name << ":" << Module::commands[i].getName() << " - " << bids[i] << endl;

      if (bids[i] > highestBid) {
        Module::highestBidder = i;
        highestBid = bids[i];
      }
    }
  }
  else {
    for (int i = 0; i < Module::commandCount; i++) {
      bids[i] = Module::commands[i].score(query);

      if (output_command_score)
        fs << Module::name << ":" << Module::commands[i].getName() << " - " << bids[i] << endl;

      if (bids[i] > highestBid) {
        Module::highestBidder = i;
        highestBid = bids[i];
      }
    }
  }
  
  if (output_command_score)
    fs.close();
  
  return highestBid;
}
void Module::queue_(string query, double& out)
{
  out = Module::queue(query);
  return;
}

string Module::execute () const
{
  variables_.clear();
  variableTypes_.clear();
  for (size_t i = 0; i < Module::commands[Module::highestBidder].getVars().size(); i++) {
    variables_.push_back(Module::commands[Module::highestBidder].getVars()[i]);
    variableTypes_.push_back(Module::commands[Module::highestBidder].getVarTypes()[i]);
  }
  variableCount_ = variables_.size();
  return CommandPtrs_[Module::name + "::" + Module::commands[Module::highestBidder].getName()]();
}

void Module::dump () const
{
  Module::dump(cout);
}
void Module::dump (ostream& stream) const
{
  Module::dump(stream, "");
}
void Module::dump (string prefix) const
{
  Module::dump(cout, prefix);
}
void Module::dump (ostream& stream, string prefix) const
{
  stream << prefix << "Name: "             << Module::name                << endl
         << prefix << "Author:"            << Module::author              << endl
         << prefix << "Description: "      << Module::description         << endl
         << prefix << "Priority: "         << Module::priority            << endl;

  stream << prefix << "Dependency Count: " << Module::dependencies.size() << endl
         << prefix << "Dependencies: "                                    << endl;
  for (size_t i = 0; i < Module::dependencies.size(); i ++)
    stream << prefix << "    " << Module::dependencies[i] << endl;
  
  stream << prefix << "Variable Count: " << Module::variables.size() << endl
         << prefix << "Variables: "                                  << endl;
  for (size_t i = 0; i < Module::variables.size(); i ++)
    stream << prefix << "    " << Module::variables[i] << endl;

  stream << prefix << "Command Count: "    << Module::commandCount        << endl
         << prefix << "Commands: "                                        << endl;

  for (int i = 0; i < Module::commandCount; i ++) {
    stream << prefix << "    " << commands[i].getName() << endl;
    commands[i].dump(stream, prefix + "    " + "    ");
  }
}


//functions
void loadModules ()
{
  loadModules("");
}
void loadModules(string username)
{
  fstream fs;
  vector<string> modules;

  if (username > "") {
    loadFileS("data/user/" + username + "/data/modules", moduleList_);
    loadFileS("data/user/" + username + "/data/modules", modules);
    fs.open("data/user/" + username + "/data/modules", fstream::in);
  }
  else {
    loadFileS("data/modules", moduleList_);
    loadFileS("data/modules", modules);
    fs.open("data/modules", fstream::in);
  }

  //check for user defined modules
  string moduleName = "";
  while (getline(fs, moduleName)) {
    string testStr;
    if (username > "")
      testStr = "data/user/" + username + "/data/module/" + moduleName + ".module";
    else
      testStr = "data/module/" + moduleName + ".module";

    if (fileExists(testStr)) { //if the user has a user defined module of that name
      if (username > "")
        testStr = "user/" + username + "/data/module/" + moduleName;
      else
        testStr = "module/" + moduleName;
      Module tmpModule(testStr);
      modules_.push_back(tmpModule);

      for (size_t i = 0; i < modules_[modules_.size() - 1].getVariables().size(); i++) {
        if (!hasVariable(modules_[modules_.size() - 1].getVariables()[i])) {
          variableList_.push_back(modules_[modules_.size() - 1].getVariables()[i]);
        }
      }
      vector<string> moduleCopy = modules;
      for (size_t i = 0; i < modules.size(); i++) {
        if (modules[i] != moduleName) //remove module from list so it isnt added again later
          moduleCopy.push_back(modules[i]);
      }
      modules = moduleCopy;
    }
  }
  fs.close();

  //add common version of modules
  for (size_t i = 0; i < modules.size(); i++) {
    if (fileExists("data/module/" + modules[i] + ".module")) {
      Module tmpModule("module/" + modules[i]);
      modules_.push_back(tmpModule);

      for (size_t i = 0; i < modules_[modules_.size() - 1].getVariables().size(); i++) {
        if (!hasVariable(modules_[modules_.size() - 1].getVariables()[i])) {
          variableList_.push_back(modules_[modules_.size() - 1].getVariables()[i]);
        }
      }
    }
  }
}

bool hasModule (string name)
{
  for (int i = 0; i < moduleCount_; i ++)
    if (modules_[i].getName() == name)
      return true;
  return false;
}

void setOPriority(string name, double oPriority)
{
  for (int i = 0; i < moduleCount_; i++)
    if (modules_[i].getName() == name)
      modules_[i].oPriority = oPriority;
}