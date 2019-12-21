////////////////////////////////////////////////////////////////////////////////
//File:   module.cpp
//Dir:    source/
//Date:   2014-03-19
//Author: Zachary Gill
//Implementation of Module
////////////////////////////////////////////////////////////////////////////////


//constants
#include "resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>


//includes
#include "header/module.h"


//namespaces
using namespace std;


//Constructors

Module::Module()
{
  Module::name = "";
  Module::description = "";
  Module::priority = 0.0;
  Module::commandCount = 0;

  moduleCount_ ++;
}

Module::Module(string name, string description, double priority)
{
  Module::name = name;
  Module::description = description;
  Module::priority = priority;
  Module::commandCount = 0;

  moduleCount_ ++;
}

Module::Module(string modfile)
{
  modfile = "data/module/" + modfile + ".module";
  if (fileExists(modfile)) {
    fstream moduleFile;
    moduleFile.open(modfile.c_str(), fstream::in);

    if (moduleFile.is_open()) {
      string tmp = "";

      getline  (moduleFile, Module::name);
      getline  (moduleFile, Module::description);
      getdouble(moduleFile, Module::priority);

      getline  (moduleFile, tmp);
      stringstream ss(tmp);
      string token;
      while (getline(ss, token, ','))
        Module::dependencies.push_back(token);

      Module::commandCount = 0;
      int nextCommand = 0;
      getint(moduleFile, nextCommand);
      while (nextCommand != 0) {
        Module::commandCount = nextCommand;

        Command tmpCommand (moduleFile);
        Module::commands.push_back(tmpCommand);
        Module::scores.push_back(0);

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
  
  moduleCount_ ++;
}


//Destructors

Module::~Module()
{
  moduleCount_ --;
}


//Accessors

string Module::getName()
{
  return Module::name;
}

string Module::getDescription()
{
  return Module::description;
}

double Module::getPriority()
{
  return Module::priority;
}

vector<string> Module::getDependencies()
{
  return Module::dependencies;
}

int Module::getCommandCount()
{
  return Module::commandCount;
}

vector<Command> Module::getCommands()
{
  return Module::commands;
}

vector<double> Module::getScores()
{
  return Module::scores;
}


//Modifiers

void Module::setPriority(double priority)
{
  Module::priority = priority;
}


//Other Functions

void Module::addCommand(Command command)
{
  Module::commandCount ++;
  Module::commands.push_back(command);
  Module::scores.push_back(0);
}

void Module::queueScores()
{
}

void Module::dump()
{
  Module::dump(cout);
}

void Module::dump(ostream& stream)
{
  Module::dump(stream, "");
}

void Module::dump(string prefix)
{
  Module::dump(cout, prefix);
}

void Module::dump(ostream& stream, string prefix)
{
  stream << prefix << "Name: " << Module::name << endl
         << prefix << "Description: " << Module::description << endl
         << prefix << "Priority: " << Module::priority << endl
         << prefix << "Dependency Count: " << Module::dependencies.size() << endl
         << prefix << "Dependencies: " << endl;
  for (size_t i = 0; i < Module::dependencies.size(); i ++)
    stream << prefix << "    " << Module::dependencies[i] << endl;
  stream << prefix << "Command Count: " << Module::commandCount << endl
         << prefix << "Commands: " << endl;
  for (int i = 0; i < Module::commandCount; i ++) {
    stream << prefix << "    " << commands[i].getName() << endl;
    commands[i].dump(stream, prefix + "    " + "    ");
  }
  stream << prefix << "Scores: " << endl;
  for (int i = 0; i < Module::commandCount; i ++)
    stream << prefix << "    " << scores[i] << endl;
}


//functions

void loadModules()
{
  fstream fs;
  fs.open("data/modules", fstream::in);

  string moduleName = "";
  while (getline(fs, moduleName)) {
    Module tmpModule (moduleName);
    modules_.push_back(tmpModule);
  }

  fs.close();
}