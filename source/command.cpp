////////////////////////////////////////////////////////////////////////////////
//File:   command.cpp
//Dir:    source/
//Date:   2015-01-04
//Author: Zachary Gill
//Implementation of Command
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

#include <boost/thread/thread.hpp>

#include "tre/regex.h"


//includes
#include "command.h"
#include "variable.h"


//namespaces
using namespace std;


//Constructors

Command::Command ()
{
  Command::name = "";
  Command::description = "";
  Command::definition = "";
  Command::returnType = RETURN_TYPE_DECLTYPE; 
  Command::module = "";
  Command::modulePriority = 0.0;
  Command::priority = 0.0;
  Command::modelCount = 0;
  Command::output = "";
  Command::risk = 0.0;
  
  commandCount_ ++;
}
Command::Command (string name, string description, string definition, int returnType, string module, double modulePriority, double priority, string output, double risk)
{
  Command::name = name;
  Command::description = description;
  Command::definition = definition;
  Command::returnType = returnType; 
  Command::module = module;
  Command::modulePriority = modulePriority;
  Command::priority = priority;
  Command::modelCount = 0;
  Command::output = output;
  Command::risk = risk;

  Command::priority /= 100;
  Command::modulePriority /= 100;
  Command::risk /= 100;
  for (int i = 0; i < modelCount; i ++)
    Command::models[i].priority /= 100;

  commandCount_ ++;
}
Command::Command (string comfile)
{
  comfile += ".command";
  fstream commandFile;
  commandFile.open(comfile.c_str(), fstream::in);
  readCommandFromFileStream(commandFile);
  commandFile.close();

  Command::priority /= 100;
  Command::modulePriority /= 100;
  Command::risk /= 100;
  for (int i = 0; i < modelCount; i ++)
    Command::models[i].priority /= 100;

  commandCount_ ++;
}
Command::Command (fstream& fs)
{
  readCommandFromFileStream(fs);

  Command::priority /= 100;
  Command::modulePriority /= 100;
  Command::risk /= 100;
  for (int i = 0; i < modelCount; i ++)
    Command::models[i].priority /= 100;

  commandCount_ ++;
}


//Destructors

Command::~Command ()
{
}

void Command::free ()
{
  Command::~Command();
  commandCount_ --;
}


//Accessors

string Command::getName () const
{
  return Command::name;
}

string Command::getDescription () const
{
  return Command::description;
}

string Command::getDefinition () const
{
  return Command::definition;
}

int Command::getReturnType () const
{
  return Command::returnType;
}

string Command::getModule () const
{
  return Command::module;
}

double Command::getModulePriority () const
{
  return Command::modulePriority;
}

double Command::getPriority () const
{
  return Command::priority;
}

int Command::getModelCount () const
{
  return Command::modelCount;
}

vector<Command::Model> Command::getModels () const
{
  return Command::models;
}

string Command::getOutput () const
{
  return Command::output;
}

double Command::getRisk () const
{
  return Command::risk;
}

vector<string> Command::getVars() const
{
  return Command::vars;
}

vector<string> Command::getVarTypes() const
{
  return Command::varTypes;
}


//Modifiers

void Command::setName(string name)
{
  Command::name = name;
}

void Command::setDescription(string description)
{
  Command::description = description;
}

void Command::setDefinition(string definition)
{
  Command::definition = definition;
}

void Command::setReturnType(int returnType)
{
  Command::returnType = returnType;
}

void Command::setModule(string module)
{
  Command::module = module;
}

void Command::setModulePriority (double modulePriority)
{
  Command::modulePriority = modulePriority;
}

void Command::setPriority (double priority)
{
  Command::priority = priority;
}

void Command::addModel(Model model)
{
  Command::models.push_back(model);
  Command::modelCount++;
}

void Command::removeModel(int index)
{
  Command::models.erase(Command::models.begin() + index);
  Command::modelCount--;
}

void Command::setOutput(string output)
{
  Command::output = output;
}

void Command::setRisk (double risk)
{
  Command::risk = risk;
}


//Other Functions

void Command::addModel (string match, double priority, int commandType, int questionType, int question)
{
  if (match == "" || commandType == 0)
    return;

  Command::modelCount ++;

  Model tmpModel;
  models.push_back(tmpModel);
  models[Command::modelCount - 1].match = match;
  models[Command::modelCount - 1].priority = priority;
  models[Command::modelCount - 1].commandType = commandType;
  models[Command::modelCount - 1].questionType = questionType;
  models[Command::modelCount - 1].question = question;
}

void Command::clearVars()
{
  Command::vars.clear();
  Command::varTypes.clear();
}

void Command::clearModelVars(int model)
{
  if (!inRange(model, 0, Command::modelCount))
    return;
  Command::models[model].vars.clear();
  Command::models[model].varTypes.clear();
}

double Command::score ()
{
  return Command::score(request_);
}
double Command::score (string query)
{
  double maxScore = 115.0;
  double commandScore = 0.0;
  string match = "";
  int bestModel = 0;
  
  if (query.length() == 0)
    return 0.0;

  query = uCase(query);
    
  Command::clearVars();

  vector<double> modelScores;
  for (int i = 0; i < Command::modelCount; i++)
    modelScores.push_back(0.0);

  if (settings_.getSettingB(SETTING_METRIC, "METRICS", "MODEL_SCORE_MT")) {
    boost::thread_group scoreThreads;
    for (int i = 0; i < Command::modelCount; i++) {
      match = Command::models[i].match;
      boost::thread *t = new boost::thread(&Command::scoreModel_, this, match, query, i, boost::ref(modelScores[i]));
      scoreThreads.add_thread(t);
    }
    scoreThreads.join_all();
  }
  else {
    for (int i = 0; i < Command::modelCount; i++)
      modelScores[i] = scoreModel(Command::models[i].match, query, i);
  }

  for (int i = 0; i < Command::modelCount; i++) {
    if (modelScores[i] > commandScore) {
      commandScore = modelScores[i];
      bestModel = i;
    }
  }

  for (size_t i = 0; i < Command::models[bestModel].vars.size(); i++) {
    Command::vars.push_back(Command::models[bestModel].vars[i]);
    Command::varTypes.push_back(Command::models[bestModel].varTypes[i]);
  }

  if (Command::vars.size() > 0)
    maxScore += 40.0;

  commandScore += 5.0 * Command::priority;
  commandScore += 5.0 * Command::modulePriority;
  
  commandScore -= 10.0 * Command::risk;

  return commandScore / maxScore;
}

double Command::scoreModel(string match, string query, int i)
{
  double modelScore = 0.0;
  
  vector<string> matchTokens;
  vector<string> variableTokens;
  matchTokens = tokenize(match, true);
  removePunctuationSoft(matchTokens);
  
  Command::clearModelVars(i);

  bool hasVariables = false;
  for (size_t j = 0; j < matchTokens.size(); j++) {
    if (isVariable(matchTokens[j]))
      hasVariables = true;
  }  

  if (hasVariables) {
    int variableCount = 0;
    string matchSum = "";

    for (size_t j = 0; j < matchTokens.size(); j++) {
      if (NOT isVariable(matchTokens[j])) {
        if (matchSum.length() > 0)
          matchSum += " ";
        matchSum += matchTokens[j];
      }
      else {
        variableCount++;
        variableTokens.push_back(matchTokens[j]);
      }
    }

    reg_match_.nmatch = variableCount + 1;
    if (reg_match_.nmatch > REG_PMATCH_SIZE)
      reg_match_.nmatch = REG_PMATCH_SIZE;
    
    if (regaexec(&Command::models[i].reg, query.c_str(), &reg_match_, reg_params_, NULL) == 0) {
      string querySum;

      for (int j = 1; j <= variableCount; j++) {
        if (reg_match_.pmatch[j].rm_eo > -1 && reg_match_.pmatch[j].rm_so > -1) {

          Command::models[i].vars.push_back(query.substr(reg_match_.pmatch[j].rm_so, (reg_match_.pmatch[j].rm_eo - reg_match_.pmatch[j].rm_so)));
          Command::models[i].varTypes.push_back(variableTokens[j - 1]);

          for (int k = reg_match_.pmatch[j].rm_so; k <= reg_match_.pmatch[j].rm_eo; k++)
            query[k] = '\0';
          if (reg_match_.pmatch[j].rm_eo == query.length()) {
            if (reg_match_.pmatch[j].rm_so > 0)
              query[reg_match_.pmatch[j].rm_so - 1] = '\0';
          }
        }
      }

      for (int j = 0; j < (int)query.length(); j++) {
        if (query[j] != '\0')
          querySum += query[j];
      }

      modelScore += 40.0 * compareString(querySum, matchSum);
      modelScore += 60.0 * compareStringPhonetic(querySum, matchSum);

      double minScore = 40.0;
      for (size_t j = 0; j < Command::models[i].vars.size(); j++) {
        double tmpScore = 40 * VariablePtrs_[Command::models[i].varTypes[j]](Command::models[i].vars[j]);
        if (tmpScore < minScore)
          minScore = tmpScore;
      }
      modelScore += minScore;
    }
  }
  else {
      modelScore += 40.0 * compareString(query, match);
      modelScore += 60.0 * compareStringPhonetic(query, match);
  }

  modelScore += 5.0 * Command::models[i].priority;

  return modelScore;
}

void Command::score_(string query, double& out)
{
  out = Command::score(query);
  return;
}

void Command::scoreModel_(string match, string query, int i, double& out)
{
  out = Command::scoreModel(match, query, i);
  return;
}

void Command::Model::dump() const
{
  Command::Model::dump(cout);
}
void Command::Model::dump(ostream& stream) const
{
  Command::Model::dump(stream, "");
}
void Command::Model::dump(string prefix) const
{
  Command::Model::dump(cout, prefix);
}
void Command::Model::dump(ostream& stream, string prefix) const
{
  stream << "Match: "         << Command::Model::match        << endl
         << "Priority: "      << Command::Model::priority     << endl
         << "Command Type: "  << Command::Model::commandType  << endl
         << "Question Type: " << Command::Model::questionType << endl
         << "Question: "      << Command::Model::question     << endl;
  stream << endl;
}

void Command::dump () const
{
  Command::dump(cout);
}
void Command::dump (ostream& stream) const
{
  Command::dump(stream, "");
}
void Command::dump (string prefix) const
{
  Command::dump(cout, prefix);
}
void Command::dump (ostream& stream, string prefix) const
{
  stream << prefix << "Name: "            << Command::name        << endl
         << prefix << "Description: "     << Command::description << endl
         << prefix << "Definition: "      << Command::definition  << endl
         << prefix << "Return Type: "     << Command::returnType  << endl
         << prefix << "Module: "          << Command::module      << endl
         << prefix << "Module Priority: " << Command::priority    << endl
         << prefix << "Priority: "        << Command::priority    << endl
         << prefix << "Model Count: "     << Command::modelCount  << endl
         << prefix << "Models: "                                  << endl;

  for (int i = 0; i < Command::modelCount; i ++)
    stream << prefix << "    " << "#" << i + 1 << endl
           << prefix << "    " << "    " << "Match: "         << Command::models[i].match        << endl
           << prefix << "    " << "    " << "Regex: "         << Command::models[i].regex        << endl
           << prefix << "    " << "    " << "Priority: "      << Command::models[i].priority     << endl
           << prefix << "    " << "    " << "Command Type: "  << Command::models[i].commandType  << endl
           << prefix << "    " << "    " << "Question Type: " << Command::models[i].questionType << endl
           << prefix << "    " << "    " << "Question: "      << Command::models[i].question     << endl;

  stream << prefix << "Outputs: "         << Command::output      << endl
         << prefix << "Risk: "            << Command::risk        << endl;
}


//Private Functions

void Command::readCommandFromFileStream (fstream& fs)
{
  if (fs.is_open()) {
    string tmp = "";

    getline  (fs, Command::name);
    getline  (fs, Command::description);
    getline  (fs, Command::definition);
    getint   (fs, Command::returnType);
    getline  (fs, Command::module);
    getdouble(fs, Command::modulePriority);
    getdouble(fs, Command::priority);

    Command::modelCount = 0;
    int nextModel = 0;
    getint(fs, nextModel);
    while (nextModel != 0) {
      Command::modelCount = nextModel;
      Model tmpModel;
      models.push_back(tmpModel);

      getline  (fs, models[Command::modelCount - 1].match);
      getdouble(fs, models[Command::modelCount - 1].priority);
      getint   (fs, models[Command::modelCount - 1].commandType);
      getint   (fs, models[Command::modelCount - 1].questionType);
      getint   (fs, models[Command::modelCount - 1].question);

      models[Command::modelCount - 1].regex = buildRegex(models[Command::modelCount - 1].match);
      models[Command::modelCount - 1].regHr = regcomp(&models[Command::modelCount - 1].reg, models[Command::modelCount - 1].regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NEWLINE | REG_UNGREEDY);

      getint(fs, nextModel);
    }

    getline  (fs, Command::output);
    getdouble(fs, Command::risk);
  }
}

string Command::buildRegex(string model)
{
  string regex;
  vector<string> tokens;

  regex = "^";
  tokens = tokenize(model, true);

  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i].substr(0, 1) == "%" && tokens[i].substr(tokens[i].size() - 1) == "%")
      regex += "([[:print:]]*)";
    else
      regex += lCase(tokens[i]);

    if (i < tokens.size() - 1)
      regex += "\\s";
  }

  regex += "$";
  return regex;
}