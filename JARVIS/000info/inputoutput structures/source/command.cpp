////////////////////////////////////////////////////////////////////////////////
//File:   command.cpp
//Dir:    source/
//Date:   2014-03-29
//Author: Zachary Gill
//Implementation of Command
////////////////////////////////////////////////////////////////////////////////


//constants
#include "resource/const.h"


//standard includes
#include <ostream>
#include <fstream>
#include <iostream>
#include <string>


//includes
#include "header/command.h"


//namespaces
using namespace std;


//Constructors

Command::Command()
{
  Command::name = "";
  Command::description = "";
  Command::definition = "";
  Command::returnType = RETURN_TYPE_DECLTYPE; 
  Command::module = "";
  Command::priority = 0.0;
  Command::modelCount = 0;
  Command::output = "";
  Command::risk = 0.0;
  
  commandCount_ ++;
}
Command::Command(string name, string description, string definition, int returnType, string module, double priority, string output, double risk)
{
  Command::name = name;
  Command::description = description;
  Command::definition = definition;
  Command::returnType = returnType; 
  Command::module = module;
  Command::priority = priority;
  Command::modelCount = 0;
  Command::output = output;
  Command::risk = risk;

  commandCount_ ++;
}
Command::Command(string comfile)
{
  comfile += ".command";
  fstream commandFile;
  commandFile.open(comfile.c_str(), fstream::in);
  readCommandFromFileStream(commandFile);
  commandFile.close();

  commandCount_ ++;
}
Command::Command(fstream& fs)
{
  readCommandFromFileStream(fs);

  commandCount_ ++;
}


//Destructors

Command::~Command()
{
}

void Command::free()
{
  Command::~Command();
  commandCount_ --;
}


//Accessors

string Command::getName()
{
  return Command::name;
}

string Command::getDescription()
{
  return Command::description;
}

string Command::getDefintion()
{
  return Command::definition;
}

int Command::getReturnType()
{
  return Command::returnType;
}

string Command::getModule()
{
  return Command::module;
}

double Command::getPriority()
{
  return Command::priority;
}

int Command::getModelCount()
{
  return Command::modelCount;
}

vector<Command::Model> Command::getModels()
{
  return Command::models;
}

string Command::getOutput()
{
  return Command::output;
}

double Command::getRisk()
{
  return Command::risk;
}


//Modifiers

void Command::setPriority(double priority)
{
  Command::priority = priority;
}

void Command::setRisk(double risk)
{
  Command::risk = risk;
}


//Other Functions

void Command::addModel(string match, int commandType, int questionType, int question)
{
  if (match == "" || commandType == 0)
    return;

  Command::modelCount ++;

  Model tmpModel;
  models.push_back(tmpModel);
  models[Command::modelCount - 1].match = match;
  models[Command::modelCount - 1].commandType = commandType;
  models[Command::modelCount - 1].questionType = questionType;
  models[Command::modelCount - 1].question = question;
}

double Command::score()
{
  return Command::score(request_);
}
double Command::score(string query)
{
  query = uCase(query);
  double score = 0.0;

  for (int i = 0; i < Command::modelCount; i ++)
    if (query == Command::models[i].match)
      score = 100.0;

  return score;
}

void Command::dump()
{
  Command::dump(cout);
}
void Command::dump(ostream& stream)
{
  Command::dump(stream, "");
}
void Command::dump(string prefix)
{
  Command::dump(cout, prefix);
}
void Command::dump(ostream& stream, string prefix)
{
  stream << prefix << "Name: "        << Command::name        << endl
         << prefix << "Description: " << Command::description << endl
         << prefix << "Definition: "  << Command::definition  << endl
         << prefix << "Return Type: " << Command::returnType  << endl
         << prefix << "Module: "      << Command::module      << endl
         << prefix << "Priority: "    << Command::priority    << endl
         << prefix << "Model Count: " << Command::modelCount  << endl
         << prefix << "Models: " << endl;

  for (int i = 0; i < Command::modelCount; i ++)
    stream << prefix << "    " << "#" << i + 1 << endl
           << prefix << "    " << "    " << "Match: "         << Command::models[i].match        << endl
           << prefix << "    " << "    " << "Command Type: "  << Command::models[i].commandType  << endl
           << prefix << "    " << "    " << "Question Type: " << Command::models[i].questionType << endl
           << prefix << "    " << "    " << "Question: "      << Command::models[i].question     << endl;

  stream << prefix << "Outputs: " << Command::output          << endl
         << prefix << "Risk: "    << Command::risk            << endl;
}


//Private Functions

void Command::readCommandFromFileStream(fstream& fs)
{
  if (fs.is_open()) {
    string tmp = "";

    getline  (fs, Command::name);
    getline  (fs, Command::description);
    getline  (fs, Command::definition);
    getint   (fs, Command::returnType);
    getline  (fs, Command::module);
    getdouble(fs, Command::priority);

    Command::modelCount = 0;
    int nextModel = 0;
    getint(fs, nextModel);
    while (nextModel != 0) {
      Command::modelCount = nextModel;
      Model tmpModel;
      models.push_back(tmpModel);

      getline(fs, models[Command::modelCount - 1].match);
      getint (fs, models[Command::modelCount - 1].commandType);
      getint (fs, models[Command::modelCount - 1].questionType);
      getint (fs, models[Command::modelCount - 1].question);

      getint(fs, nextModel);
    }

    getline  (fs, Command::output);
    getdouble(fs, Command::risk);
  }
}