////////////////////////////////////////////////////////////////////////////////
//File:   module_editor.cpp
//Dir:    *
//Date:   2015-02-16
//Author: Zachary Gill
//Implementation of Module Editor
////////////////////////////////////////////////////////////////////////////////


#define  NTDDI_VERSION NTDDI_WIN7
#define  WINVER        _WIN32_WINNT_WIN7
#define _WIN32_WINNT   _WIN32_WINNT_WIN7
#define _WIN32_IE      _WIN32_IE_IE50

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


//standard includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <io.h>
#include <string>
#include <vector>
#include <map>
#include <rpc.h>
#include <ctime>
#include <time.h>
#include <cctype>
#include <cstdio>
#include <stdlib.h>
#include <algorithm>
#include <Windows.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/thread/thread.hpp"

#include "CryptoPP/config.h"
#include "CryptoPP/default.h"
#include "CryptoPP/sha.h"
#include "CryptoPP/aes.h"
#include "CryptoPP/files.h"
#include "CryptoPP/modes.h"
#include "CryptoPP/filters.h"
#include "CryptoPP/hex.h"
#include "CryptoPP/pwdbased.h"


//includes
#include "module_editor.h"


//libraries
#pragma comment(lib, "Rpcrt4.lib")


//namespaces
using namespace std;


//functions

int main ()
{
  //declare variables
  int a00 = 0;
  int a0 = 0;
  int a1 = 0;
  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
  int e = 0;
  int f = 0;
  int g = 0;
  int h = 0;
  int z = 0;
  int tmpI = 0;
  double tmpD = 0;
  string modfile = "";
  string modloc = "";
  string tmp = "";
  string newTmp = "";
  string loc = "";
  bool ready, has;
  vector<string> arr;

  //load location of DLA
  if (!fileExists("C:/ProgramData/DLA/loc")) //if loc file doesnt exist
    return 1;

  fstream fs;
  fs.open("C:/ProgramData/DLA/loc", fstream::in);
  std::getline(fs, loc_);
  fs.close();


  //main loop
  do {
    cls(h_);
    cout << "What do you want to do?" << endl
      << "0 - Exit" << endl
      << "1 - Log-in" << endl << endl;
    cout << ": ";
    cin >> a00;
    switch (a00) {
      case 0:
        break;
      case 1:
        User user;
        if (simpleLogin(user)) { //log in

          do {
            //declare lists
            vector<string> modules; //stores a list of all modules
            vector<string> moduleUser; //stores a list of the users who own each module in modules
            vector<string> users; //stores a list of users
            loadLists(loc_, modules, moduleUser, users); //load lists

            cls(h_);
            cout << "Module Count: " << modules.size() << endl
                 << "Modules" << endl;
            for (size_t i = 0; i < modules.size(); i ++)
              cout << "    " << modules[i] << endl; //print list of modules to screen
            cout << endl
                 << "0 - Done (log-out)" << endl
                 << "1 - Edit Module" << endl
                 << "2 - Add Module" << endl
                 << "3 - Remove Module" << endl << endl
                 << ": ";
            cin >> a0; //get user input
            switch (a0)
            {
              case 0 : //Done (log-out)
                break;
              case 1 : //Edit Module
                do {
                  loadLists(loc_, modules, moduleUser, users); //reload lists
                  cls(h_);
                  cout << "Module Count: " << modules.size() << endl
                       << "Modules" << endl;
                  for (size_t i = 0; i < modules.size(); i ++)
                    cout << "   [" << i + 1 << "] " << modules[i] << endl; //enumerate modules on screen
                  cout << endl
                       << "0 - Done Editing Modules" << endl
                       << "X - Edit Module [X]" << endl << endl
                       << ": ";
                  cin >> a1; //get user input
                  std::getline(cin, tmp);
                  if (a1 > 0 && a1 <= (int) modules.size()) { //if user input is a valid module
                    
                    if (!(user.getAdmin() || uCase(user.getUsername()) == uCase(moduleUser[a1 - 1]))) {
                      cls(h_);
                      cout << "You need administrator priviledges to edit this module." << endl;
                      cout << "Would you like to create a user-specific copy of this module?" << endl
                        << "0 - No" << endl
                        << "1 - Yes" << endl;
                      cin >> a;
                      switch (a)
                      {
                        case 0:
                          break;
                        case 1:
                          string modfile = loc_ + "data/module/" + modules[a1 - 1] + ".module"; //file of module
                          Module m (modfile); //load the module
                          
                          if (!fileExists(loc_ + "data/user/" + user.getUsername() + "/data/module/" + lCase(m.name) + ".module")) { //if the user doesnt already have this file
                            arr.clear();
                            loadFileS(loc_ + "data/user/" + user.getUsername() + "/data/modules", arr);
                            has = false;
                            for (size_t mods = 0; mods < arr.size(); mods++) {
                              if (uCase(arr[mods]) == uCase(m.name))
                                has = true;
                            }
                            if (has) { //it has module installed
                              copyFile(loc_ + "data/module/" + lCase(m.name) + ".module", loc_ + "data/user/" + user.getUsername() + "/data/module/" + lCase(m.name) + ".module"); //copy it
                              addToUser(m, loc_, user.getUsername()); //add to user
                              addUserModule(m, loc_, user.getUsername()); //add user module
                              cls(h_);
                              cout << "A user-specific instance of this module has been created." << endl;
                              system("PAUSE");
                            }
                            else {
                              cls(h_);
                              cout << "This module must first be added to this user." << endl;
                              cout << "Contact an admin." << endl;
                              system("PAUSE");
                            }
                          }
                          else { //user already has a copy of this module
                            cls(h_);
                            cout << "You already have an instance of this module." << endl;
                            system("PAUSE");
                          }
                          break;
                      }
                    }
                    else {
                      string modfile = loc_ + "data/module/" + modules[a1 - 1] + ".module"; //file of module
                      Module m (modfile); //load the module
                      vector<string> settingNames; //stores the names of the settings of the module
                      vector<string> settingValues; //stores the values of the settings of the module
                      string setfile;
                      if (moduleUser[a1 - 1] == "") //module is not user speccific
                        setfile = loc_ + "data/setting/module/" + modules[a1 - 1] + ".settings"; //settings file of module
                      else //module is user specific
                        setfile = loc_ + "data/user/" + moduleUser[a1 - 1] + "/data/setting/module/" + modules[a1 - 1] + ".settings"; //settings file of user specific module
                      loadSettings(loc_, settingNames, settingValues, setfile); //load module settings
                      do { //Edit Module
                        cls(h_);
                        m.dump(); //print data about the module
                        cout << "0  - Done Editing Module (Save)" << endl
                             << "1  - Change Name" << endl
                             << "2  - Edit Description" << endl
                             << "3  - Change Priority" << endl
                             << "4  - Edit Keywords" << endl
                             << "5  - Edit Dependencies" << endl
                             << "6  - Edit Variables" << endl
                             << "7  - Edit Resource Files" << endl
                             << "8  - Edit Data Files" << endl
                             << "9  - Edit Solution Settings" << endl
                             << "10 - Edit Commands" << endl
                             << "11 - Edit Settings" << endl
                             << "12 - Add to DLA" << endl
                             << "13 - Remove from DLA" << endl
                             << "14 - Add to User" << endl
                             << "15 - Remove from User" << endl
                             << "16 - Create User Copy" << endl
                             << "17 - Compile for Release" << endl << endl
                             << ": ";
                        cin >> a; //get user input
                        std::getline(cin, tmp);
                        switch (a) {
                          case 0 : //Dont Editing Module (Save)
                            //organize alphabetically
                            alphabetize(m.keywords);
                            alphabetize(m.dependencies);
                            alphabetize(m.variables);
                            //organize commands by match element alphabetically
                            for (int i = 0; i < m.commandCount; i ++) {
                              Command& n = m.commands[i];
                              for (int j = 0; j < n.modelCount; j++) {
                                for (int k = 0; k < n.modelCount; k++) {
                                  if (n.models[k].match > n.models[j].match)
                                    swap(n.models[k], n.models[j]);
                                }
                              }
                            }
                            //organize settings by name alphabetically
                            for (size_t i = 0; i < settingNames.size(); i++) {
                              for (size_t j = 0; j < settingNames.size(); j++) {
                                if (settingNames[j] > settingNames[i]) {
                                  swap(settingNames[j], settingNames[i]);
                                  swap(settingValues[j], settingValues[i]);
                                }
                              }
                            }

                            //save the module
                            modloc = loc_ + "data/module/" + modules[a1 - 1]; //module file
                            saveModule(m, loc_, moduleUser[a1 - 1], modloc); //save the module
                            //modloc = loc_ + "data/setting/module/" + modules[a1 - 1] + ".settings"; //module settings file
                            saveSettings(m, loc_, settingNames, settingValues, setfile); //save the module settings
                            break;
                          case 1 : //Change Name
                            cls(h_);
                            cout << "Old Name: " << m.name << endl
                                 << "New Name: ";
                            std::getline(cin, tmp); //get user input for new name
                            if (tmp > "") //if the name is not empty
                              m.name = uCase(tmp); //capitalize it
                            break;
                          case 2 : //Edit Description
                            cls(h_);
                            cout << "Old Description: " << m.description << endl
                                 << "New Description: ";
                            std::getline(cin, tmp); //get user input of new description
                            if (tmp > "") //if description is not empty
                              m.description = tmp; //set new description
                            break;
                          case 3 : //Change Priority
                            cls(h_);
                            cout << "Priority is a double between 0 and 100 inclusive" << endl << endl; //print range for priority
                            cout << "Old Priority: " << m.priority << endl
                                 << "New Priority: ";
                            cin >> tmpD; //get user input of new priority
                            std::getline(cin, tmp);
                            if (tmpD >= 0 && tmpD <= 100) //if user input is appropriate
                              m.priority = tmpD; //change priority
                            break;
                          case 4 : //Edit Keywords
                            do {
                              cls(h_);
                              cout << "Keyword Count: " << m.keywords.size() << endl
                                   << "Keywords: "                              << endl;
                              for (size_t i = 0; i < m.keywords.size(); i ++)
                                cout << "    " << m.keywords[i] << endl; //print all current keywords to the screen
                              cout << endl
                                   << "0 - Done Editing Keywords" << endl
                                   << "1 - Add Keyword" << endl
                                   << "2 - Remove Keyword" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0 : //Done Editing Keywords
                                  alphabetize(m.keywords); //alphabetize keywords
                                  break;
                                case 1 : //Add Keyword
                                  do {
                                    cls(h_);
                                    cout << "Keyword Count: " << m.keywords.size() << endl
                                         << "Keywords: "                              << endl;
                                    for (size_t i = 0; i < m.keywords.size(); i ++)
                                      cout << "    " << m.keywords[i] << endl; //print all keywords to the screen
                                    cout << endl
                                         << "0 - Done Adding Keywords" << endl
                                         << "1 - Add a Keyword" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Adding Keywords
                                        break;
                                      case 1 : //Add a Keyword
                                        cls(h_);
                                        cout << "Name of New Keyword: ";
                                        std::getline(cin, tmp); //get user input of name of new keyword
                                        if (tmp > "") { //if the new name is not empty
                                          bool isnew = true; //initialize bool test
                                          for (size_t i = 0; i < m.keywords.size(); i ++) //for each keyword
                                            if (uCase(m.keywords[i]) == uCase(tmp)) //if it matches the new name
                                              isnew = false; //it is not new
                                          if (isnew) //if it is new
                                            m.keywords.push_back(uCase(tmp)); //add keyword to list
                                        }
                                        break;
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 2 : //Remove Keyword
                                  do {
                                    cls(h_);
                                    cout << "Keyword Count: " << m.keywords.size() << endl
                                         << "Keywords: "                              << endl;
                                    for (size_t i = 0; i < m.keywords.size(); i ++)
                                      cout << "    [" << i + 1 << "] " << m.keywords[i] << endl; //enumerate keywords on screen
                                    cout << endl
                                         << "0 - Done Removing Keywords" << endl
                                         << "X - Remove Keyword [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int) m.keywords.size()) //if user input is a valid keyword
                                      m.keywords.erase(m.keywords.begin() + c - 1); //remove it
                                  } while (c > 0); //loop until 0
                                  break;
                              }
                            } while (b > 0); //loop until 0
                            break;
                          case 5 : //Edit Dependencies
                            do {
                              cls(h_);
                              cout << "Dependency Count: " << m.dependencies.size() << endl
                                   << "Dependencies: "                              << endl;
                              for (size_t i = 0; i < m.dependencies.size(); i ++)
                                cout << "    " << m.dependencies[i] << endl; //print dependencies to screen
                              cout << endl
                                   << "0 - Done Editing Dependencies" << endl
                                   << "1 - Add Dependency" << endl
                                   << "2 - Remove Dependency" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0 : //Done Editing Dependencies
                                  alphabetize(m.dependencies); //alphabetize dependencies
                                  break;
                                case 1 : //Add Dependency
                                  do {
                                    cls(h_);
                                    cout << "Dependency Count: " << m.dependencies.size() << endl
                                         << "Dependencies: "                              << endl;
                                    for (size_t i = 0; i < m.dependencies.size(); i ++)
                                      cout << "    " << m.dependencies[i] << endl; //print list of current dependencies
                                    cout << endl
                                         << "0 - Done Adding Dependencies" << endl
                                         << "1 - Add a Dependency" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Adding Dependencies
                                        break;
                                      case 1 : //Add a Dependency
                                        cls(h_);
                                        cout << "Name of New Dependency: ";
                                        std::getline(cin, tmp); //get user input of name of new dependency
                                        if (tmp > "") { //if new name is not empty
                                          bool isnew = true; //initialize bool test
                                          for (size_t i = 0; i < m.dependencies.size(); i ++) //for each dependency
                                            if (uCase(m.dependencies[i]) == uCase(tmp)) //if name matches existing dependency
                                              isnew = false; //it is not new
                                          if (isnew) //if it is new
                                            m.dependencies.push_back(uCase(tmp)); //add dependency to list
                                        }
                                        break;
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 2 : //Remove Dependency
                                  do {
                                    cls(h_);
                                    cout << "Dependency Count: " << m.dependencies.size() << endl
                                         << "Dependencies: "                              << endl;
                                    for (size_t i = 0; i < m.dependencies.size(); i ++)
                                      cout << "    [" << i + 1 << "] " << m.dependencies[i] << endl; //enumerate dependencies on screen
                                    cout << endl
                                         << "0 - Done Removing Dependencies" << endl
                                         << "X - Remove Dependency [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int) m.dependencies.size()) //if user input is a valid dependency
                                      m.dependencies.erase(m.dependencies.begin() + c - 1); //remove it
                                  } while (c > 0); //loop until 0
                                  break;
                              }
                            } while (b > 0); //loop until 0
                            break;
                          case 6 : //Edit Variables
                            do {
                              cls(h_);
                              cout << "Variable Count: " << m.variables.size() << endl
                                   << "Variables: "                            << endl;
                              for (size_t i = 0; i < m.variables.size(); i ++)
                                cout << "    " << m.variables[i] << endl; //print variable list to screen
                              cout << endl
                                   << "0 - Done Editing Variables" << endl
                                   << "1 - Add Variable" << endl
                                   << "2 - Remove Variable" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0 : //Done Editing Variables
                                  alphabetize(m.variables); //alphabetize variable list
                                  makeVariables(m, loc_); //make files for new variables
                                  break;
                                case 1 : //Add Variable
                                  do {
                                    cls(h_);
                                    cout << "Variable Count: " << m.variables.size() << endl
                                         << "Variables: "                            << endl;
                                    for (size_t i = 0; i < m.variables.size(); i ++)
                                      cout << "    " << m.variables[i] << endl; //print variable list to screen
                                    cout << endl
                                         << "0 - Done Adding Variables" << endl
                                         << "1 - Add a Variable" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Adding Variables
                                        break;
                                      case 1 : //Add a Variable
                                        cls(h_);
                                        cout << "Name of New Variable (ex: 'CHR'): ";
                                        std::getline(cin, tmp); //get user input of the new variable name
                                        if (tmp > "") { //if the 
                                          string tmp2 = tmp; //clone tmp to tmp2
                                          tmp = ""; //reset tmp
                                          for (size_t i = 0; i < tmp2.length(); i++) { //go through tmp2 char by char
                                            if (tmp2[i] != '%') //remove any '%'
                                              tmp += tmp2[i]; //store back in tmp
                                          }
                                          bool isnew = true; //initialize bool test
                                          for (size_t i = 0; i < m.variables.size(); i ++) //for each variable
                                            if (uCase(m.variables[i]) == uCase(tmp)) //if the name matches the new variable name
                                              isnew = false; //it is not new
                                          if (isnew) //if it is new
                                            m.variables.push_back(uCase(tmp)); //add variable to list
                                        }
                                        break;
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 2 : //Remove Variable
                                  do {
                                    cls(h_);
                                    cout << "Variable Count: " << m.variables.size() << endl
                                         << "Variables: " << endl;
                                    for (size_t i = 0; i < m.variables.size(); i++)
                                      cout << "    [" << i + 1 << "] " << m.variables[i] << endl; //enumerate variables on screen
                                    cout << endl
                                      << "0 - Done Removing Variables" << endl
                                      << "X - Remove Variable [X]" << endl << endl
                                      << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int)m.variables.size()) { //if user input is valid variable
                                      string variableErase = m.variables[c - 1]; //save variable name to string
                                      m.variables.erase(m.variables.begin() + c - 1); //remove variable from list

                                      //remove variable
                                      vector<string> tmpModules; //stores a list of all modules
                                      vector<string> tmpModuleUser; //stores a list of the users who own each module in tmpModules
                                      vector<string> tmpUser; //stores a list of users
                                      loadLists(loc_, tmpModules, tmpModuleUser, tmpUser); //load lists
                                      vector<Module> mods; //stores temporarily loaded Modules
                                      for (size_t i = 0; i < tmpModules.size(); i++) { //for each module
                                        string mfile = loc_ + "data/module/" + tmpModules[i] + ".module"; //get the module file
                                        Module tmpMod(mfile); //load the Module into tmpMod
                                        if (uCase(tmpMod.name) != uCase(m.name)) //if it is not the current module
                                          mods.push_back(tmpMod); //add it to mods
                                        tmpMod.~Module(); //free the Module
                                      }

                                      bool stillNeeds = false; //initialize bool test
                                      for (size_t i = 0; i < mods.size(); i++) { //for each Module in mods
                                        for (size_t j = 0; j < mods[i].variables.size(); j++) { //for each variable in that mods[i]
                                          if (uCase(mods[i].variables[j]) == uCase(variableErase)) //if it is the variable you are trying to remove
                                            stillNeeds = true; //another module still needs it
                                        }
                                      }
                                      if (stillNeeds == false) { //if no other module still needs it
                                        //delete .cpp file
                                        modfile = loc_ + "source/variable/" + lCase(variableErase) + "_V.cpp";
                                        deleteFile(modfile);

                                        //delete .h file
                                        modfile = loc_ + "source/variable/" + lCase(variableErase) + "_V.h";
                                        deleteFile(modfile);

                                        //delete .variable file
                                        modfile = loc_ + "data/variable/" + lCase(variableErase) + ".variable";
                                        deleteFile(modfile);

                                        //remove from data/variable/ptrs file
                                        vector<string> varptriables; //stores lines of variable/ptrs file
                                        string varptrnamefile = loc_ + "data/variable/ptrs"; //the variable/ptrs file
                                        fstream dvptrs; //open file stream
                                        loadFileS(varptrnamefile, varptriables); //load variable/ptrs file into varptriables
                                        dvptrs.open(varptrnamefile, fstream::out); //open the same file for output
                                        for (size_t i = 0; i < varptriables.size(); i++) { //for each string in varptriables
                                          if (varptriables[i] != "#include \"" + lCase(variableErase) + ".variable\"") //if it is not the line for the variable we are trying to remove
                                            dvptrs << varptriables[i] << endl; //print it back to the file
                                        }
                                        dvptrs.close(); //close the file

                                        //remove from source/resource/variables file
                                        vector<string> variables; //will store lines from source/resource/variables file
                                        string varnamefile = loc_ + "source/resource/variables"; //the source/resource/variables file
                                        fstream srvars; //open a file stream
                                        loadFileS(varnamefile, variables); //load the lines of source/resource/variables into variables vector
                                        srvars.open(varnamefile, fstream::out); //open same file for output
                                        for (size_t i = 0; i < variables.size(); i++) { //for each string
                                          if (variables[i] != "#include \"../variable/" + lCase(variableErase) + "_V.h\"") //if it is not the line for the variable we are trying to remove
                                            srvars << variables[i] << endl; //print it back to the file
                                        }
                                        srvars.close(); //close the file
                                      }

                                      for (size_t m = 0; m < mods.size(); m++)
                                        mods[m].~Module();
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                              }
                            } while (b > 0); //loop until 0
                            break;
                          case 7 : //Edit Resource Files
                            do {
                              cls(h_);
                              cout << "Resource File Count: " << m.resourceFiles.size() << endl
                                   << "Resource Files: " << endl;
                              for (size_t i = 0; i < m.resourceFiles.size(); i++)
                                cout << "    " << m.resourceFiles[i] << endl; //list resource files to screen
                              cout << endl
                                   << "0 - Done Editing Resource Files" << endl
                                   << "1 - Add Resource File" << endl
                                   << "2 - Remove Resource File" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0: //Done Editing Resource Files
                                  alphabetize(m.resourceFiles); //alphabetize
                                  break;
                                case 1: //Add Resource File
                                  do {
                                    cls(h_);
                                    cout << "Resource File Count: " << m.resourceFiles.size() << endl
                                         << "Resource Files: " << endl;
                                    for (size_t i = 0; i < m.resourceFiles.size(); i++)
                                      cout << "    " << m.resourceFiles[i] << endl; //list resource files to screen
                                    cout << endl
                                         << "0 - Done Adding Resource Files" << endl
                                         << "1 - Add a Resource File" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Adding Resource Files
                                        break;
                                      case 1 : //Add a Resource File
                                        cls(h_);
                                        cout << "Enter the resource file or folder" << endl
                                             << "Resource folders end with '/'" << endl
                                             << "Make the file or folder relative to 'resource/module/" << lCase(m.name) << "/'" << endl << endl
                                             << ":";
                                        getline(cin, tmp); //get user input
                                        m.resourceFiles.push_back(tmp); //add resource file to module
                                        break;
                                    }
                                  } while (c > 0);
                                  break;
                                case 2: //Remove Resource File
                                  do {
                                    cls(h_);
                                    cout << "Resource File Count: " << m.resourceFiles.size() << endl
                                         << "Resource Files: " << endl;
                                    for (size_t i = 0; i < m.resourceFiles.size(); i++)
                                      cout << "    [" << i + 1 << "] " << m.resourceFiles[i] << endl; //list resource files to screen
                                    cout << endl
                                         << "0 - Done Removing Resource Files" << endl
                                         << "X - Remove Resource File [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int)m.resourceFiles.size()) //if user input is acceptable
                                      m.resourceFiles.erase(m.resourceFiles.begin() + c - 1); //remove resource file from module
                                  } while (c > 0);
                                  break;
                              }
                            } while (b > 0);
                            break;
                          case 8 : //Edit Data Files
                            do {
                              cls(h_);
                              cout << "Data File Count: " << m.dataFiles.size() << endl
                                   << "Data Files: "                            << endl;
                              for (size_t i = 0; i < m.dataFiles.size(); i++)
                                cout << "    " << m.dataFiles[i] << endl; //list data files to screen
                              cout << endl
                                   << "0 - Done Editing Data Files" << endl
                                   << "1 - Add Data File" << endl
                                   << "2 - Remove Data File" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0: //Done Editing Data Files
                                  alphabetize(m.dataFiles); //alphabetize
                                  break;
                                case 1: //Add Data File
                                  do {
                                    cls(h_);
                                    cout << "Data File Count: " << m.dataFiles.size() << endl
                                         << "Data Files: "                            << endl;
                                    for (size_t i = 0; i < m.dataFiles.size(); i++)
                                      cout << "    " << m.dataFiles[i] << endl; //list data files to screen
                                    cout << endl
                                         << "0 - Done Adding Data Files" << endl
                                         << "1 - Add a Data File" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Adding Data Files
                                        break;
                                      case 1 : //Add a Data File
                                        cls(h_);
                                        cout << "Enter the data file" << endl
                                             << "Make the file relative 'data/user/<user>/resource/module/" << lCase(m.name) << "/'" << endl
                                             << "This file can be created and saved by your module" << endl
                                             << "These files must be saved in 'data/user/<user>/resource/module/" << lCase(m.name) << "/'" << endl
                                             << ":";
                                        getline(cin, tmp); //get user input
                                        m.dataFiles.push_back(tmp); //add resource file to module
                                        break;
                                    }
                                  } while (c > 0);
                                  break;
                                case 2: //Remove Data File
                                  do {
                                    cls(h_);
                                    cout << "Data File Count: " << m.dataFiles.size() << endl
                                         << "Data Files: " << endl;
                                    for (size_t i = 0; i < m.dataFiles.size(); i++)
                                      cout << "    [" << i + 1 << "] " << m.dataFiles[i] << endl; //list data files to screen
                                    cout << endl
                                         << "0 - Done Removing Data Files" << endl
                                         << "X - Remove Data File [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int)m.dataFiles.size()) //if user input is acceptable
                                      m.dataFiles.erase(m.dataFiles.begin() + c - 1); //remove data file from module
                                  } while (c > 0);
                                  break;
                              }
                            } while (b > 0);
                            break;
                          case 9 : //Edit Solution Settings
                            do {
                              cls(h_);
                              cout << "Solution Settings:" << endl
                                   << "What do you want to do?" << endl << endl
                                   << "0 - Done Editing Solution Settings" << endl
                                   << "1 - Edit Solution Folders" << endl
                                   << "2 - Edit Solution Files" << endl
                                   << "3 - Edit Solution Properties" << endl
                                   << ":";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0 : //Done Editing Solution Settings
                                  break;
                                case 1 : //Edit Solution Folders
                                  do {
                                    cls(h_);
                                    cout << "Solution Folder Count: " << m.solutionFolders.size() << endl
                                         << "Solutions Folders:"                                  << endl;
                                    for (size_t i = 0; i < m.solutionFolders.size(); i++)
                                      cout << "    " << m.solutionFolders[i] << endl;
                                    cout << endl
                                         << "0 - Done Editing Solution Folders" << endl
                                         << "1 - Add Solution Folder" << endl
                                         << "2 - Remove Solution Folder" << endl
                                         << ":";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Editing Solution Folders
                                        break;
                                      case 1 : //Add Solution Folder
                                        do {
                                          cls(h_);
                                          cout << "Solution Folder Count: " << m.solutionFolders.size() << endl
                                               << "Solutions Folders:"                                  << endl;
                                          for (size_t i = 0; i < m.solutionFolders.size(); i++)
                                            cout << "    " << m.solutionFolders[i] << endl;
                                          cout << endl
                                               << "0 - Done Adding Solution Folders" << endl
                                               << "1 - Add a Solution Folder" << endl
                                               << ":";
                                          cin >> d; //get user input
                                          std::getline(cin, tmp);
                                          switch (d)
                                          {
                                            case 0 : //Done Adding Solution Folders
                                              break;
                                            case 1 : //Add a Solution Folder
                                              cls(h_);
                                              cout << "Enter solution folder" << endl
                                                   << ": ";
                                              std::getline(cin, tmp);
                                              m.solutionFolders.push_back(tmp); //add solution folder to module
                                              break;
                                          }
                                        } while (d > 0);
                                        break;
                                      case 2 : //Remove Solution Folder
                                        do {
                                          cls(h_);
                                          cout << "Solution Folder Count: " << m.solutionFolders.size() << endl
                                               << "Solutions Folders:"                                  << endl;
                                          for (size_t i = 0; i < m.solutionFolders.size(); i++)
                                            cout << "    [" << i + 1 << "] " << m.solutionFolders[i] << endl;
                                          cout << endl
                                               << "0 - Done Removing Solution Folders" << endl
                                               << "X - Remove Solution Folder [X]" << endl
                                               << ":";
                                          cin >> d; //get user input
                                          std::getline(cin, tmp);
                                          if (d > 0 && d <= (int)m.solutionFolders.size()) //if user input is acceptable
                                            m.solutionFolders.erase(m.solutionFolders.begin() + d - 1); //remove solution folder from module
                                        } while (d > 0);
                                        break;
                                    }
                                  } while (c > 0);
                                  break;
                                case 2 : //Edit Solution Files
                                  do {
                                    cls(h_);
                                    cout << "Solution File Count: " << m.solutionFiles.size() << endl
                                         << "Solutions Files:"                                << endl;
                                    for (size_t i = 0; i < m.solutionFiles.size(); i++)
                                      cout << "    " << m.solutionFiles[i] << endl;
                                    cout << endl
                                         << "0 - Done Editing Solution Files" << endl
                                         << "1 - Add Solution Files" << endl
                                         << "2 - Remove Solution Files" << endl
                                         << ":";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Editing Solution Files
                                        break;
                                      case 1 : //Add Solution Files
                                        do {
                                          cls(h_);
                                          cout << "Solution File Count: " << m.solutionFiles.size() << endl
                                               << "Solutions Files:"                                << endl;
                                          for (size_t i = 0; i < m.solutionFiles.size(); i++)
                                            cout << "    " << m.solutionFiles[i] << endl;
                                          cout << endl
                                               << "0 - Done Adding Solution Files" << endl
                                               << "1 - Add a Solution File" << endl
                                               << ":";
                                          cin >> d; //get user input
                                          std::getline(cin, tmp);
                                          switch (d)
                                          {
                                            case 0 : //Done Adding Solution Files
                                              break;
                                            case 1 : //Add a Solution File
                                              cls(h_);
                                              cout << "Enter solution file" << endl
                                                   << "Make the file relative to 'source/'." << endl
                                                   << ": ";
                                              std::getline(cin, tmp); 
                                              m.solutionFiles.push_back(tmp); //add solution file to module
                                              m.solutionProperties.push_back(vector<string>{}); //add new entry to solution properties
                                              m.solutionPropertyCounts.push_back(0);

                                              //alphabetize solution files
                                              for (size_t i = 0; i < m.solutionFiles.size(); i++) {
                                                for (size_t j = 0; j < m.solutionFiles.size(); j++) {
                                                  if (m.solutionFiles[j] > m.solutionFiles[i]) {
                                                    swap(m.solutionFiles[j], m.solutionFiles[i]);
                                                    swap(m.solutionProperties[j + 2], m.solutionProperties[i + 2]); //swap soluion file property index as well
                                                    swap(m.solutionPropertyCounts[j + 2], m.solutionPropertyCounts[i + 2]);
                                                  }
                                                }
                                              }
                                              break;
                                          }
                                        } while (d > 0);
                                        break;
                                      case 2 : //Remove Solution Files
                                        do {
                                          cls(h_);
                                          cout << "Solution File Count: " << m.solutionFiles.size() << endl
                                               << "Solutions Files:"                                << endl;
                                          for (size_t i = 0; i < m.solutionFiles.size(); i++)
                                            cout << "    [" << i + 1 << "] " << m.solutionFiles[i] << endl;
                                          cout << endl
                                               << "0 - Done Removing Solution Files" << endl
                                               << "X - Remove Solution File [X]" << endl
                                               << ":";
                                          cin >> d; //get user input
                                          std::getline(cin, tmp);
                                          if (d > 0 && d <= (int)m.solutionFiles.size()) { //if user input is acceptable
                                            m.solutionFiles.erase(m.solutionFiles.begin() + d - 1); //remove solution file from module
                                            m.solutionProperties.erase(m.solutionProperties.begin() + d - 1 + 2); //remove solution properties for that file as well
                                            m.solutionPropertyCounts.erase(m.solutionPropertyCounts.begin() + d - 1 + 2);
                                          }
                                        } while (d > 0);
                                        break;
                                    }
                                  } while (c > 0);
                                  break;
                                case 3 : //Edit Solution Properties
                                  do {
                                    cls(h_);
                                    cout << "Select a file to edit the properties of: " << endl
                                         << "    [1] " << lCase(m.name) << ".cpp" << endl
                                         << "    [2] " << lCase(m.name) << ".h"   << endl;
                                    for (size_t i = 0; i < m.solutionFiles.size(); i++)
                                      cout << "    [" << i + 3 << "] " << m.solutionFiles[i] << endl;
                                    cout << endl
                                      << "0 - Done Editing Solution Properties" << endl
                                      << "X - Edit Solution Properties of File [X]" << endl
                                      << ":";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int)m.solutionFiles.size() + 2) { //if user input is acceptable
                                      do {
                                        cls(h_);
                                        switch (c)
                                        {
                                          case 1 : //.cpp
                                            cout << "File: " << lCase(m.name) << ".cpp" << endl;
                                            break;
                                          case 2 : //.h
                                            cout << "File: " << lCase(m.name) << ".h" << endl;
                                            break;
                                          default:
                                            cout << "File: " << m.solutionFiles[c - 3] << endl;
                                        }
                                        cout << "Solution Property Count: " << m.solutionPropertyCounts[c - 1] << endl
                                             << "Solution Properties:" << endl;
                                        for (int i = 0; i < m.solutionPropertyCounts[c - 1]; i++)
                                          cout << "    " << m.solutionProperties[c - 1][i] << endl;
                                        cout << endl
                                             << "0 - Done Editing Solution File Properties" << endl
                                             << "1 - Add Property" << endl
                                             << "2 - Remove Property" << endl
                                             << ": ";
                                        cin >> d; //get user input
                                        std::getline(cin, tmp);
                                        switch (d)
                                        {
                                          case 0 : //Done Editing Solution File Properties
                                            break;
                                          case 1 : //Add Property
                                            do {
                                              cls(h_);
                                              switch (c)
                                              {
                                                case 1: //.cpp
                                                  cout << "File: " << lCase(m.name) << ".cpp" << endl;
                                                  break;
                                                case 2: //.h
                                                  cout << "File: " << lCase(m.name) << ".h" << endl;
                                                  break;
                                                default:
                                                  cout << "File: " << m.solutionFiles[c - 3] << endl;
                                              }
                                              cout << "Solution Property Count: " << m.solutionPropertyCounts[c - 1] << endl
                                                << "Solution Properties:" << endl;
                                              for (int i = 0; i < m.solutionPropertyCounts[c - 1]; i++)
                                                cout << "    " << m.solutionProperties[c - 1][i] << endl;
                                              cout << endl
                                                << "0 - Done Adding Properties" << endl
                                                << "1 - Add a Property" << endl
                                                << ": ";
                                              cin >> e; //get user input
                                              std::getline(cin, tmp);
                                              switch (e)
                                              {
                                                case 0 : //Done Adding Properties
                                                  break;
                                                case 1 : //Add a Property
                                                  cls(h_);
                                                  cout << "Enter or paste the property to add to the solution file." << endl
                                                       << ": ";
                                                  std::getline(cin, tmp);
                                                  m.solutionProperties[c - 1].push_back(tmp); //add solution property
                                                  m.solutionPropertyCounts[c - 1]++; //incrememnt solution property count
                                                  break;
                                              }
                                            } while (e > 0);
                                            break;
                                          case 2 : // Remove Property
                                            do {
                                              cls(h_);
                                              switch (c)
                                              {
                                                case 1: //.cpp
                                                  cout << "File: " << lCase(m.name) << ".cpp" << endl;
                                                  break;
                                                case 2: //.h
                                                  cout << "File: " << lCase(m.name) << ".h" << endl;
                                                  break;
                                                default:
                                                  cout << "File: " << m.solutionFiles[c - 3] << endl;
                                              }
                                              cout << "Solution Property Count: " << m.solutionPropertyCounts[c - 1] << endl
                                                << "Solution Properties:" << endl;
                                              for (int i = 0; i < m.solutionPropertyCounts[c - 1]; i++)
                                                cout << "    [" << i + 1 << "]  " << m.solutionProperties[c - 1][i] << endl;
                                              cout << endl
                                                << "0 - Done Removing Properties" << endl
                                                << "X - Remove Property [X]" << endl
                                                << ": ";
                                              cin >> e; //get user input
                                              std::getline(cin, tmp);
                                              if (e > 0 && e <= m.solutionPropertyCounts[c - 1]) {
                                                m.solutionProperties[c - 1].erase(m.solutionProperties[c - 1].begin() + e - 1); //remove solution property
                                                m.solutionPropertyCounts[c - 1]--; //decrememnt solution property count
                                              }
                                            } while (e > 0);
                                            break;
                                        }
                                      } while (d > 0);
                                    }
                                  } while (c > 0);
                                  break;
                              }
                            } while (b > 0);
                            break;
                          case 10 : //Edit Commands
                            do {
                              cls(h_);
                              cout << "Command Count: " << m.commandCount << endl
                                   << "Commands: "                        << endl;
                              for (int i = 0; i < m.commandCount; i ++)
                                cout << "    " << m.commands[i].name << endl; //print command list to screen
                              cout << endl
                                   << "0 - Done Editing Commands" << endl
                                   << "1 - Edit Command" << endl
                                   << "2 - Add Command" << endl
                                   << "3 - Remove Command" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0 : //Done Editing Commands
                                  //alphabetize commands by name element
                                  for (int i = 0; i < m.commandCount; i++) {
                                    for (int j = 0; j < m.commandCount; j++) {
                                      if (m.commands[j].name > m.commands[i].name)
                                        swap(m.commands[j], m.commands[i]);
                                    }
                                  }
                                  break;
                                case 1 : //Edit Command
                                  do {
                                    cls(h_);
                                    cout << "Command Count: " << m.commandCount << endl
                                         << "Commands: "                        << endl;
                                    for (int i = 0; i < m.commandCount; i ++)
                                      cout << "   [" << i + 1 << "] " << m.commands[i].name << endl; //enumerate commands on screen
                                    cout << endl
                                         << "0 - Done Editing Commands" << endl
                                         << "X - Edit Command X" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= m.commandCount) { //if user input is a valid command
                                      Command& n = m.commands[c - 1]; //store pointer to it in n
                                      do {
                                        cls(h_);
                                        n.dump(); //dump the command information
                                        cout << "0 - Done Editing Command [" << c << "]" << endl
                                             << "1 - Change Name" << endl
                                             << "2 - Edit Description" << endl
                                             << "3 - Edit Definition" << endl
                                             << "4 - Change Return Type" << endl
                                             << "5 - Change Priority" << endl
                                             << "6 - Edit Models" << endl
                                             << "7 - Change Output" << endl
                                             << "8 - Change Risk" << endl << endl
                                             << ": ";
                                        cin >> d;
                                        std::getline(cin, tmp);
                                        switch (d)
                                        {
                                          case 0 : //Done Editing Command
                                            break;
                                          case 1 : //Change Name
                                            cls(h_);
                                            cout << "Old Name: " << n.name << endl
                                                 << "New Name: ";
                                            std::getline(cin, tmp); //get user input of new name
                                            if (tmp > "") //if new name is not empty
                                              n.name = tmp; //change name
                                            break;
                                          case 2 : //Edit Description
                                            cls(h_);
                                            cout << "Old Description: " << n.description << endl
                                                 << "New Description: ";
                                            std::getline(cin, tmp); //get user input of new description
                                            if (tmp > "") //if new description is not empty
                                              n.description = tmp; //change description
                                            break;
                                          case 3 : //Edit Definition
                                            cls(h_);
                                            cout << "Old Definition: " << n.definition << endl
                                                 << "New Definition: ";
                                            std::getline(cin, tmp); //get user input of new definition
                                            if (tmp > "") //if new definition is not empty
                                              n.definition = tmp; //change definition
                                            break;
                                          case 4 : //Change Return Type
                                            cls(h_);
                                            cout << "RETURN TYPES:" << endl //print possible return type values
                                                 << "0  - DECLTYPE" << endl
                                                 << "1  - VOID" << endl
                                                 << "2  - BOOL" << endl
                                                 << "3  - SCHAR" << endl
                                                 << "4  - UCHAR" << endl
                                                 << "5  - SHORT" << endl
                                                 << "6  - USHORT" << endl
                                                 << "7  - INT" << endl
                                                 << "8  - UINT" << endl
                                                 << "9  - LONG" << endl
                                                 << "10 - ULONG" << endl
                                                 << "11 - LONGLONG" << endl
                                                 << "12 - ULONGLONG" << endl
                                                 << "13 - FLOAT" << endl
                                                 << "14 - DOUBLE" << endl
                                                 << "15 - LONGDOUBLE" << endl
                                                 << "16 - CHAR" << endl
                                                 << "17 - CHAR16_T" << endl
                                                 << "18 - CHAR32_T" << endl
                                                 << "19 - WCHAR_T" << endl
                                                 << "20 - STRING" << endl
                                                 << "21 - UDEF" << endl << endl;
                                            cout << "Old Return Type: " << n.returnType << endl
                                                 << "New Return Type: ";
                                            cin >> tmpI; //get user input of new return type
                                            std::getline(cin, tmp);
                                            if (tmpI >= RETURN_TYPE_DECLTYPE && tmpI <= RETURN_TYPE_UDEF) //if user input is appropriate
                                              n.returnType = tmpI; //change return type
                                            break;
                                          case 5 : //Change Priority
                                            cls(h_);
                                            cout << "Priority is a double between 0 and 100 inclusive" << endl << endl; //print range for priority
                                            cout << "Old Priority: " << n.priority << endl
                                                 << "New Priority: ";
                                            cin >> tmpD; //get user input of new priority
                                            if (tmpD >= 0 && tmpD <= 100) //if user input is appropriate
                                              n.priority = tmpD; //change priority
                                            break;
                                          case 6 : //Edit Models
                                            do {
                                              cls(h_);
                                              cout << "Model Count: " << n.modelCount << endl
                                                   << "Models: "                      << endl;
                                              for (int i = 0; i < n.modelCount; i ++)
                                                cout << "    " << n.models[i].match << endl; //print models to screen by match element
                                              cout << endl
                                                   << "0 - Done Editing Models" << endl
                                                   << "1 - Edit Model" << endl
                                                   << "2 - Add Model" << endl
                                                   << "3 - Remove Model" << endl << endl
                                                   << ": ";
                                              cin >> e; //get user input
                                              std::getline(cin, tmp);
                                              switch (e)
                                              {
                                                case 0 : //Dome Editing Models
                                                  //alphabetize models by match element
                                                  for (int i = 0; i < n.modelCount; i++) {
                                                    for (int j = 0; j < n.modelCount; j++) {
                                                      if (n.models[j].match > n.models[i].match)
                                                        swap(n.models[j], n.models[i]);
                                                    }
                                                  }
                                                  break;
                                                case 1 : //Edit Model
                                                  do {
                                                    cls(h_);
                                                    cout << "Model Count: " << n.models.size() << endl
                                                         << "Models: "                         << endl;
                                                    for (size_t i = 0; i < n.models.size(); i ++)
                                                      cout << "    [" << i + 1 << "] " << n.models[i].match << endl; //enumerate models to screen by match element
                                                    cout << endl
                                                         << "0 - Done Editing Models" << endl
                                                         << "X - Edit Model [X]" << endl << endl
                                                         << ": ";
                                                    cin >> f; //get user input
                                                    std::getline(cin, tmp);
                                                    if (f > 0 && f <= (int) n.models.size()) { //if user input is a valid model
                                                      Command::Model& p = n.models[f - 1]; //store pointer to model in p
                                                      do {
                                                        cls(h_);
                                                        p.dump(); //print information about the model
                                                        cout << "0 - Done Editing Model [" << f << "]" << endl
                                                             << "1 - Change Match" << endl
                                                             << "2 - Change Priority" << endl
                                                             << "3 - Change Command Type" << endl
                                                             << "4 - Change Question Type" << endl
                                                             << "5 - Change Question" << endl << endl
                                                             << ": ";
                                                        cin >> g; //get user input
                                                        std::getline(cin, tmp);
                                                        switch (g)
                                                        {
                                                          case 0 : //Done Editing Model
                                                            break;
                                                          case 1 : //Change Match
                                                            cls(h_);
                                                            cout << "Old Match: " << p.match << endl
                                                                 << "New Match: ";
                                                            std::getline(cin, tmp); //get user input of new match string
                                                            if (tmp > "") //if new match string is not empty
                                                              p.match = uCase(tmp); //change match string
                                                            break;
                                                          case 2 : //Change Priority
                                                            cls(h_);
                                                            cout << "Priority is a double between 0 and 100 inclusive" << endl << endl; //print range for priority
                                                            cout << "Old Priority: " << p.priority << endl
                                                                 << "New Priority: ";
                                                            cin >> tmpD; //get user input of new priority
                                                            std::getline(cin, tmp);
                                                            if (tmpD >= 0 && tmpD <= 100) //if user input is appropriate
                                                              p.priority = tmpD; //change priority
                                                            break;
                                                          case 3 : //Change Command Type
                                                            cls(h_);
                                                            cout << "COMMAND TYPES:" << endl //print possible values for command types
                                                                 << "0 - Null" << endl
                                                                 << "1 - Command" << endl
                                                                 << "2 - Question" << endl
                                                                 << "3 - Statement" << endl << endl;
                                                            cout << "Old Command Type: " << p.commandType << endl
                                                                 << "New Command Type: ";
                                                            cin >> tmpI; //get user input of new command type
                                                            std::getline(cin, tmp);
                                                            if (tmpI >= COMMAND_TYPE_NULL && tmpI <= COMMAND_TYPE_STATEMENT) //if user input is appropriate
                                                              p.commandType = tmpI; //change command type
                                                            break;
                                                          case 4 : //Change Question Type
                                                            cls(h_);
                                                            cout << "QUESTION TYPES:" << endl //print possible values for question types
                                                                 << "0 - Null" << endl
                                                                 << "1 - Basic" << endl
                                                                 << "2 - Complex" << endl << endl;
                                                            cout << "Old Question Type: " << p.questionType << endl
                                                                 << "New Question Type: ";
                                                            cin >> tmpI; //get user input of new question type
                                                            std::getline(cin, tmp);
                                                            if (tmpI >= QUESTION_TYPE_NULL && tmpI <= QUESTION_TYPE_COMPLEX) //if user input is appropriate
                                                              p.questionType = tmpI; //change question type
                                                            break;
                                                          case 5 : //Change Question
                                                            vector<string> questions; //stores question strings
                                                            bool validQ = true; //initiailize bool test
                                                            string qFile;
                                                            switch (p.questionType)
                                                            {
                                                              case QUESTION_TYPE_BASIC : //if basic
                                                                qFile = loc_ + "resource/data/question_words_basic"; //data file
                                                                break;
                                                              case QUESTION_TYPE_COMPLEX : //if complex
                                                                qFile = loc_ + "resource/data/question_words_complex"; //data file
                                                                break;
                                                              default :
                                                                validQ = false; //if neither value than bool is set to false
                                                            }
                                                            if (validQ) { //if true
                                                              loadFileS(qFile, questions); //load data file into questions vector
                                                              cls(h_);
                                                              switch (p.questionType)
                                                              {
                                                                case QUESTION_TYPE_BASIC : //if basic
                                                                  cout << "BASIC QUESTIONS:" << endl;
                                                                  break;
                                                                case QUESTION_TYPE_COMPLEX : //if complex
                                                                  cout << "COMPLEX QUESTIONS:" << endl;
                                                                  break;
                                                              }
                                                              cout << "0 - NULL" << endl;
                                                              for (size_t q = 0; q < questions.size(); q++)
                                                                cout << q + 1 << " - " << questions[q] << endl; //print possible question values
                                                              cout << endl
                                                                   << "Old Question: " << p.question << endl
                                                                   << "New Question: ";
                                                              cin >> tmpI; //get user input of new question
                                                              std::getline(cin, tmp);
                                                              if (tmpI >= 0 && tmpI <= (int)questions.size())
                                                                p.question = tmpI;
                                                            }
                                                            break;
                                                        }
                                                      } while (g > 0); //loop until 0
                                                    }
                                                  } while (f > 0); //loop until 0
                                                  break;
                                                case 2 : //Add Model
                                                  do {
                                                    cls(h_);
                                                    cout << "Model Count: " << n.models.size() << endl
                                                         << "Models: "                         << endl;
                                                    for (size_t i = 0; i < n.models.size(); i ++)
                                                      cout << "    " << n.models[i].match << endl; //print models to screen as match element
                                                    cout << endl
                                                         << "0 - Done Adding Models" << endl
                                                         << "1 - Add a Model" << endl << endl
                                                         << ": ";
                                                    cin >> f; //get user input
                                                    std::getline(cin, tmp);
                                                    switch (f)
                                                    {
                                                      case 0 : //Done Adding Models
                                                        break;
                                                      case 1 : //Add a Model
                                                        Command::Model x; //create new Model
                                                        bool isnew = true; //initialize bool test

                                                        //match
                                                        do {
                                                          cls(h_);
                                                          cout << "Enter the match string for the new Model." << endl;
                                                          cout << "Match: ";
                                                          std::getline(cin, tmp); //get user input
                                                          if (tmp > "") { //if user input is not empty
                                                            for (size_t i = 0; i < n.models.size(); i ++) //for each model
                                                              if (uCase(n.models[i].match) == uCase(tmp)) //if user input is equal to another match
                                                                isnew = false; //it is not new
                                                            if (isnew) //if it is new
                                                              x.match = uCase(tmp); //store match
                                                          }
                                                        } while (!isnew); //loop until a good match string

                                                        //priority
                                                        do {
                                                          cls(h_);
                                                          cout << "Enter priority as a double between 0 and 100 inclusive." << endl; //print range for priority
                                                          cout << "Priority: ";
                                                          cin >> tmpD; //get user input
                                                          std::getline(cin, tmp);
                                                          if (tmpD >= 0 && tmpD <= 100) //if user input is in valid range
                                                            x.priority = tmpD; //store priority
                                                        } while (!(tmpD >= 0 && tmpD <= 100)); //loop until good priority

                                                        //command type
                                                        do {
                                                          cls(h_);
                                                          cout << "COMMAND TYPES:" << endl //print possible values for command types
                                                               << "0 - Null" << endl
                                                               << "1 - Command" << endl
                                                               << "2 - Question" << endl
                                                               << "3 - Statement" << endl << endl;
                                                          cout << "Command Type: ";
                                                          cin >> tmpI; //get user input of command type of new model
                                                          std::getline(cin, tmp);
                                                          if (tmpI >= COMMAND_TYPE_NULL && tmpI <= COMMAND_TYPE_STATEMENT) //if user input is appropriate
                                                            x.commandType = tmpI; //store command type
                                                        } while (!(tmpI >= COMMAND_TYPE_NULL && tmpI <= COMMAND_TYPE_STATEMENT)); //loop until good command type

                                                        //question type
                                                        do {
                                                          cls(h_);
                                                          cout << "QUESTION TYPES:" << endl //print possible values for question types
                                                            << "0 - Null" << endl
                                                            << "1 - Basic" << endl
                                                            << "2 - Complex" << endl << endl;
                                                          cout << "Question Type: ";
                                                          cin >> tmpI; //get user input of question type of new model
                                                          std::getline(cin, tmp);
                                                          if (tmpI >= QUESTION_TYPE_NULL && tmpI <= QUESTION_TYPE_COMPLEX) //if user input is appropriate
                                                            x.questionType = tmpI; //store question type
                                                        } while (!(tmpI >= QUESTION_TYPE_NULL && tmpI <= QUESTION_TYPE_COMPLEX)); //loop until good question type

                                                        //question
                                                        vector<string> questions; //stores question strings
                                                        string qFile;
                                                        switch (x.questionType)
                                                        {
                                                          case QUESTION_TYPE_BASIC: //if basic
                                                            qFile = loc_ + "resource/data/question_words_basic"; //data file
                                                            break;
                                                          case QUESTION_TYPE_COMPLEX: //if complex
                                                            qFile = loc_ + "resource/data/question_words_complex"; //data file
                                                            break;
                                                        }
                                                        loadFileS(qFile, questions); //load data file into questions vector
                                                        do {
                                                          cls(h_);
                                                          switch (x.questionType)
                                                          {
                                                            case QUESTION_TYPE_BASIC: //if basic
                                                              cout << "BASIC QUESTIONS:" << endl;
                                                              break;
                                                            case QUESTION_TYPE_COMPLEX: //if complex
                                                              cout << "COMPLEX QUESTIONS:" << endl;
                                                              break;
                                                          }
                                                          cout << "0 - NULL" << endl;
                                                          for (size_t q = 0; q < questions.size(); q++)
                                                            cout << q + 1 << " - " << questions[q] << endl; //print possible question values
                                                          cout << endl
                                                               << "Question: ";
                                                          cin >> tmpI; //get user input of question of new model
                                                          std::getline(cin, tmp);
                                                          if (tmpI >= 0 && tmpI <= (int)questions.size()) //if user input is appropriate
                                                            x.question = tmpI; //store question
                                                        } while (!(tmpI >= 0 && tmpI <= (int)questions.size())); //loop until good question

                                                        n.models.push_back(x); //add model to command
                                                        n.modelCount++; //increase model count
                                                        break;
                                                    }
                                                  } while (f > 0); //loop until 0
                                                  break;
                                                case 3 : //Remove Model
                                                  do {
                                                    cls(h_);
                                                    cout << "Model Count: " << n.models.size() << endl
                                                         << "Models: "                         << endl;
                                                    for (size_t i = 0; i < n.models.size(); i ++)
                                                      cout << "    [" << i + 1 << "] " << n.models[i].match << endl; //enumerate models by match element
                                                    cout << endl
                                                         << "0 - Done Removing Models" << endl
                                                         << "X - Remove Model [X]" << endl << endl
                                                         << ": ";
                                                    cin >> f; //get user input
                                                    std::getline(cin, tmp);
                                                    if (f > 0 && f <= (int) n.models.size()) { //if user input is appropriate
                                                      n.models.erase(n.models.begin() + f - 1); //remove model
                                                      n.modelCount--; //decrease model count
                                                    }
                                                  } while (f > 0); //loop until 0
                                                  break;
                                              }
                                            } while (e > 0); //loop until 0
                                            break;
                                          case 7 : //Change Output
                                            cls(h_);
                                            cout << "Old Output: " << n.output << endl
                                                 << "New Output: ";
                                            std::getline(cin, tmp); //get user input
                                            if (tmp > "") //is user input is not empty
                                              n.output = tmp; //change output
                                            break;
                                          case 8 : //Change Risk
                                            cls(h_);
                                            cout << "Risk is a double between 0 and 100 inclusive" << endl << endl; //print range for risk
                                            cout << "Old Risk: " << n.risk << endl
                                                 << "New Risk: ";
                                            cin >> tmpD; //get user input
                                            if (tmpD >= 0 && tmpD <= 100) //is user input is appropriate
                                              n.risk = tmpD; //change risk
                                            break;
                                        }
                                      } while (d > 0); //loop until 0
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 2 : //Add Command
                                  do {
                                    cls(h_);
                                    cout << "Command Count: " << m.commandCount << endl
                                         << "Commands: "                        << endl;
                                    for (int i = 0; i < m.commandCount; i ++)
                                      cout << "    " << m.commands[i].name << endl; //print list of commands
                                    cout << endl
                                         << "0 - Done Adding Commands" << endl
                                         << "1 - Add a Command" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c)
                                    {
                                      case 0 : //Done Adding Commands
                                        break;
                                      case 1 : //Add a Command
                                        Command x; //create new command
                                        bool isnew = true; //initialize bool test

                                        //name
                                        do {
                                          cls(h_);
                                          cout << "Enter a name for the new command, the same name as the function in the source" << endl;
                                          cout << "Name: ";
                                          std::getline(cin, tmp); //get user input
                                          if (tmp > "") { //if user input is not empty
                                            for (int i = 0; i < m.commandCount; i ++) //for each command
                                              if (uCase(m.commands[i].name) == uCase(tmp)) //if user input matches name of another command
                                                isnew = false; //it is not new
                                            if (isnew) //if it is new
                                              x.name = tmp; //store name
                                          }
                                        } while (!isnew); //loop until good name

                                        //description
                                        cls(h_);
                                        cout << "Enter a description for the command" << endl;
                                        cout << "Description: ";
                                        std::getline(cin, tmp); //get user input
                                        x.description = tmp;

                                        //definition
                                        x.definition = "string " + x.name + "();"; //generate definition from name

                                        //return type
                                        do {
                                          cls(h_);
                                          cout << "Enter a return type for the command, all commands must return string, but enter the type of variable the string will contain" << endl;
                                          cout << "RETURN TYPES:" << endl //print possible return type values
                                               << "0  - DECLTYPE" << endl
                                               << "1  - VOID" << endl
                                               << "2  - BOOL" << endl
                                               << "3  - SCHAR" << endl
                                               << "4  - UCHAR" << endl
                                               << "5  - SHORT" << endl
                                               << "6  - USHORT" << endl
                                               << "7  - INT" << endl
                                               << "8  - UINT" << endl
                                               << "9  - LONG" << endl
                                               << "10 - ULONG" << endl
                                               << "11 - LONGLONG" << endl
                                               << "12 - ULONGLONG" << endl
                                               << "13 - FLOAT" << endl
                                               << "14 - DOUBLE" << endl
                                               << "15 - LONGDOUBLE" << endl
                                               << "16 - CHAR" << endl
                                               << "17 - CHAR16_T" << endl
                                               << "18 - CHAR32_T" << endl
                                               << "19 - WCHAR_T" << endl
                                               << "20 - STRING" << endl
                                               << "21 - UDEF" << endl << endl;
                                          cout << "Return Type: ";
                                          cin >> tmpI; //get user input of return type of new command
                                          std::getline(cin, tmp);
                                          if (tmpI >= RETURN_TYPE_DECLTYPE && tmpI <= RETURN_TYPE_UDEF) //if user input is appropriate
                                            x.returnType = tmpI; //store return type
                                        } while (!(tmpI >= RETURN_TYPE_DECLTYPE && tmpI <= RETURN_TYPE_UDEF)); //loop until good return type

                                        //set module elements
                                        x.module = m.name;
                                        x.modulePriority = m.priority;

                                        //priority
                                        do {
                                          cls(h_);
                                          cout << "Enter priority as a double between 0 and 100 inclusively" << endl; //print range for priority
                                          cout << "Priority: ";
                                          cin >> tmpD; //get user input
                                          std::getline(cin, tmp);
                                          if (tmpD >= 0 && tmpD <= 100) //if user input is appropriate
                                            x.priority = tmpD; //store priority
                                        } while (!(tmpD >= 0 && tmpD <= 100)); //loop until good priority
                              
                                        //output
                                        cls(h_);
                                        cout << "Enter a string that describes the output of the command" << endl;
                                        cout << "Output: ";
                                        std::getline(cin, tmp); //get user input
                                        x.output = tmp;

                                        //risk
                                        do {
                                          cls(h_);
                                          cout << "Enter the risk as a double between 0 and 100 inclusively" << endl; //print range for risk
                                          cout << "Risk: ";
                                          cin >> tmpD; //get user input
                                          std::getline(cin, tmp);
                                          if (tmpD >= 0 && tmpD <= 100) //if user input is appropriate
                                            x.risk = tmpD;
                                        } while (!(tmpD >= 0 && tmpD <= 100)); //loop until good risk

                                        m.commands.push_back(x); //add command to module
                                        m.commandCount++; //increase command count
                                        break;
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 3 : //Remove Command
                                  do {
                                    cls(h_);
                                    cout << "Command Count: " << m.commandCount << endl
                                         << "Commands: "                        << endl;
                                    for (int i = 0; i < m.commandCount; i ++)
                                      cout << "    [" << i + 1 << "] " << m.commands[i].name << endl; //enumerate commands
                                    cout << endl
                                         << "0 - Done Removing Commands" << endl
                                         << "X - Remove Commands [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= m.commandCount) { //if user input is appropriate
                                      m.commands.erase(m.commands.begin() + c - 1); //remove command
                                      m.commandCount--; //decrease command count
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                              }
                            } while (b > 0); //loop until 0
                            break;
                          case 11 : //Edit Settings
                            do {
                              cls(h_);
                              cout << uCase(m.name) << " Setting Count: " << settingNames.size() << endl
                                   << "Settings:" << endl;
                              for (size_t i = 0; i < settingNames.size(); i++) {
                                cout << "    " << uCase(settingNames[i]) << ": " << settingValues[i] << endl; //list setting names and values
                              }
                              cout << endl
                                   << "0 - Done Editing Settings" << endl
                                   << "1 - Edit Setting" << endl
                                   << "2 - Add Setting" << endl
                                   << "3 - Remove Setting" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b)
                              {
                                case 0: //Done Editing Settings
                                  //alphabetize
                                  for (size_t i = 0; i < settingNames.size(); i++) {
                                    for (size_t j = 0; j < settingNames.size(); j++) {
                                      if (settingNames[j] > settingNames[i]) {
                                        swap(settingNames[j], settingNames[i]);
                                        swap(settingValues[j], settingValues[i]);
                                      }
                                    }
                                  }
                                  break;
                                case 1 : //Edit Setting
                                  do {
                                    cls(h_);
                                    cout << uCase(m.name) << " Setting Count: " << settingNames.size() << endl
                                         << "Settings: "                              << endl;
                                    for (size_t i = 0; i < settingNames.size(); i ++)
                                      cout << "    [" << i + 1 << "] " << uCase(settingNames[i]) << ": " << settingValues[i] << endl; //enumerate setting names and values
                                    cout << endl
                                         << "0 - Done Editing Settings" << endl
                                         << "X - Edit Setting [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int) settingNames.size()) { //if user input is appropriate
                                      do {
                                        cls(h_);
                                        cout << "Name: " << uCase(settingNames[c - 1]) << endl
                                             << "Value: " << settingValues[c - 1] << endl << endl;
                                        cout << "0 - Done Editing Setting [" << c << "]" << endl
                                             << "1 - Change Name" << endl
                                             << "2 - Change Value" << endl << endl
                                             << ": ";
                                        cin >> g; //get user input
                                        std::getline(cin, tmp);
                                        switch (g)
                                        {
                                          case 0 : //Done Editing Setting
                                            break;
                                          case 1 : //Change Name
                                            cls(h_);
                                            cout << "Old Name: " << uCase(settingNames[c - 1]) << endl
                                                 << "New Name: ";
                                            std::getline(cin, tmp); //get user input
                                            if (tmp > "") { //if user input is not empty
                                              newTmp = "";
                                              for (size_t i = 0; i < tmp.length(); i++) { //move through tmp char by char
                                                if (tmp[i] == ' ' || tmp[i] == '\0') //if it is a space
                                                  newTmp += "_"; //replace with an underscore
                                                else
                                                  newTmp += tmp[i]; //otherwise copy char
                                              }
                                              tmp = newTmp;
                                              bool isnew = true; //initialize bool test
                                              for (size_t i = 0; i < settingNames.size(); i++) { //for each setting
                                                if (uCase(settingNames[i]) == uCase(tmp) && i != c - 1) //if name is the same as user input
                                                  isnew = false; //it is not new
                                              }
                                              if (isnew) //if it is new
                                                settingNames[c - 1] = tmp; //change setting name
                                            }
                                            break;
                                          case 2 : //Chage Value
                                            cls(h_);
                                            cout << "Old Value: " << settingValues[c - 1] << endl
                                                 << "New Value: ";
                                            std::getline(cin, tmp); //get user input
                                            if (tmp > "") //if user input is not empty
                                              settingValues[c - 1] = tmp; //change setting value
                                            break;
                                        }
                                      } while (d > 0); //loop until 0
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 2 : //Add Setting
                                  do {
                                    cls(h_);
                                    cout << uCase(m.name) << " Setting Count: " << settingNames.size() << endl
                                         << "Settings:" << endl;
                                    for (size_t i = 0; i < settingNames.size(); i++)
                                      cout << "    " << uCase(settingNames[i]) << ": " << settingValues[i] << endl; //enumerate setting names and values
                                    cout << endl
                                         << "0 - Done Adding Settings" << endl
                                         << "1 - Add a Setting" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    switch (c) {
                                      case 0 : //Done Adding Settings
                                        break;
                                      case 1 : //Add a Setting
                                        //name
                                        bool isnew = false; //preinitialize bool test
                                        do {
                                          cls(h_);
                                          cout << "Enter the name of the new setting" << endl;
                                          cout << "Name: ";
                                          std::getline(cin, tmp); //get user input
                                          if (tmp > "") { //if user input is not empty
                                            newTmp = "";
                                            for (size_t i = 0; i < tmp.length(); i++) { //move through tmp char by char
                                              if (tmp[i] == ' ' || tmp[i] == '\0') //if char is a space
                                                newTmp += "_"; //replace with an underscore
                                              else
                                                newTmp += tmp[i]; //otherwise copy char
                                            }
                                            tmp = newTmp;
                                            isnew = true; //initialize bool test
                                            for (size_t i = 0; i < settingNames.size(); i++) { //for each setting
                                              if (uCase(settingNames[i]) == uCase(tmp)) //if name matches user input
                                                isnew = false; //it is not new
                                            }
                                          }
                                          if (isnew) { //if it is new
                                            settingNames.push_back(lCase(tmp)); //add setting name to list
                                          }
                                          else {
                                            cls(h_);
                                            cout << "You can not use this name" << endl;
                                            system("PAUSE");
                                          }
                                        } while (!isnew); //loop until good name

                                        //value
                                        cls(h_);
                                        cout << "Enter the value of the new setting" << endl;
                                        cout << "Value: ";
                                        std::getline(cin, tmp); //get user input
                                        settingValues.push_back(tmp);

                                        break;
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                                case 3 : //Remove Setting
                                  do {
                                    cls(h_);
                                    cout << uCase(m.name) << " Setting Count: " << settingNames.size() << endl
                                         << "Settings: "                              << endl;
                                    for (size_t i = 0; i < settingNames.size(); i ++)
                                      cout << "    [" << i + 1 << "] " << uCase(settingNames[i]) << ": " << settingValues[i] << endl; //enumerate setting names and values
                                    cout << endl
                                         << "0 - Done Removing Settings" << endl
                                         << "X - Remove Setting [X]" << endl << endl
                                         << ": ";
                                    cin >> c; //get user input
                                    std::getline(cin, tmp);
                                    if (c > 0 && c <= (int) settingNames.size()) { //if user input is appropriate
                                      settingNames.erase(settingNames.begin() + c - 1); //remove setting name
                                      settingValues.erase(settingValues.begin() + c - 1); //remove settin value
                                    }
                                  } while (c > 0); //loop until 0
                                  break;
                              }
                            } while (b > 0); //loop until 0
                            break;
                          case 12 : //Add to DLA
                            if (moduleUser[a1 - 1] == "") { //if it is not a user specific module
                              //test if system has dependencies
                              ready = true;
                              for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                has = false; //initialize bool test
                                for (size_t i = 0; i < modules.size(); i++) { //for each module
                                  if (uCase(modules[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                    has = true; //has
                                }
                                if (!has)
                                  ready = false;
                              }
                              cls(h_);
                              if (ready) { //if it has all dependencies
                                addToDla(m, loc_); //add to DLA
                                cout << "This module has been added to the system." << endl;
                              }
                              else { //if it is missing dependencies
                                cout << "Before you can add this module you must add its dependencies." << endl
                                     << "Missing Dependencies:" << endl;
                                for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                  has = false; //initialize bool test
                                  for (size_t i = 0; i < modules.size(); i++) { //for each module
                                    if (uCase(modules[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                      has = true; //has
                                  }
                                  if (!has)
                                    cout << "  " << uCase(m.dependencies[pre]) << endl; //print missing dependecy
                                }
                              }
                              system("PAUSE");
                            }
                            break;
                          case 13 : //Remove from DLA
                            if (moduleUser[a1 - 1] == "") { //if it is not a user specific module
                              cls(h_);
                              cout << "Are you sure you want to remove this module from the system?" << endl
                                   << "By doing so, all users on this system that had included the modules will no longer have access to them. Also all user-specific modules of the same name will not be deleted but will be removed from the user's access" << endl
                                   << "0 - Do not remove the module" << endl
                                   << "1 - I understand, remove the module" << endl
                                   << ": ";
                              cin >> b; //get user input
                              std::getline(cin, tmp);
                              switch (b) {
                                case 0: //Do not remove the module
                                  break;
                                case 1: //I understand, remove the module
                                  removeFromDla(m, loc_); //remove from DLA
                                  cls(h_);
                                  cout << "This module has been removed from the system." << endl
                                       << "This module has been removed from all user accounts including user-specific modules of the same name." << endl;
                                  system("PAUSE");
                                  break;
                              }
                            }
                            break;
                          case 14: //Add to User
                            if (moduleUser[a1 - 1] == "") { //if it is not a user specific module
                              do {
                                cls(h_);
                                cout << "Which user would you like to add this module to?" << endl
                                  << "0 - Cancel" << endl
                                  << "1 - All Users" << endl;
                                loadLists(loc_, modules, moduleUser, users); //load lists
                                for (size_t i = 0; i < users.size(); i++)
                                  cout << i + 2 << " - " << users[i] << endl; //enumerate users
                                cout << endl << ": ";
                                cin >> b; //get user input
                                std::getline(cin, tmp);
                                switch (b) {
                                  case 0: //Cancel
                                    break;
                                  case 1: //All Users
                                    cls(h_);
                                    for (size_t i = 0; i < users.size(); i++) { //for each user
                                      cout << users[i] << "-" << endl;
                                      arr.clear();
                                      loadFileS(loc_ + "data/user/" + users[i] + "/data/modules", arr);
                                      //test if user has dependencies
                                      ready = true;
                                      for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                        has = false; //initialize bool test
                                        for (size_t i = 0; i < arr.size(); i++) { //for each module
                                          if (uCase(arr[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                            has = true; //has
                                        }
                                        if (!has)
                                          ready = false;
                                      }
                                      if (ready) { //if has all dependencies
                                        addToUser(m, loc_, users[i]); //add to user
                                        cout << "  This module has been added to " << users[i] << "." << endl;
                                      }
                                      else { //if missing dependencies
                                        cout << "  Before you add this module to " << users[i] << ", you must add its dependencies." << endl
                                             << "  Missing Dependencies:" << endl;
                                        for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                          has = false; //initialize bool test
                                          for (size_t i = 0; i < arr.size(); i++) { //for each module
                                            if (uCase(arr[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                              has = true; //has
                                          }
                                          if (!has)
                                            cout << "    " << uCase(m.dependencies[pre]) << endl; //print missing dependecy
                                        }
                                      }
                                    }
                                    arr.clear();
                                    system("PAUSE");
                                    break;
                                  default:
                                    if (b > 1 && b < (int)users.size() + 2) { //if user input is appropriate
                                      arr.clear();
                                      loadFileS(loc_ + "data/user/" + users[b - 2] + "/data/modules", arr);
                                      //test if user has dependencies
                                      ready = true;
                                      for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                        has = false; //initialize bool test
                                        for (size_t i = 0; i < arr.size(); i++) { //for each module
                                          if (uCase(arr[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                            has = true; //has
                                        }
                                        if (!has)
                                          ready = false;
                                      }
                                      cls(h_);
                                      if (ready) { //if has all dependecies
                                        addToUser(m, loc_, users[b - 2]); //add to user
                                        cout << "This module has been added to " << users[b - 2] << "." << endl;
                                      }
                                      else { //if missing depenedencies
                                        cout << "Before you add this module to " << users[b - 2] << ", you must add its dependencies." << endl
                                             << "Missing Dependencies:" << endl;
                                        for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                          has = false; //initialize bool test
                                          for (size_t i = 0; i < arr.size(); i++) { //for each module
                                            if (uCase(arr[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                              has = true; //has
                                          }
                                          if (!has)
                                            cout << "  " << uCase(m.dependencies[pre]) << endl; //print missing dependecy
                                        }
                                      }
                                      arr.clear();
                                      system("PAUSE");
                                    }
                                    break;
                                }
                              } while (b > 0); //loop until 0
                            }
                            else { //if it is a user specific module
                              arr.clear();
                              loadFileS(loc_ + "data/user/" + users[b - 2] + "/data/modules", arr);
                              //test if user has dependencies
                              ready = true;
                              for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                has = false; //initialize bool test
                                for (size_t i = 0; i < arr.size(); i++) { //for each module
                                  if (uCase(arr[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                    has = true; //has
                                }
                                if (!has)
                                  ready = false;
                              }
                              cls(h_);
                              if (ready) {
                                addToUser(m, loc_, moduleUser[a1 - 1]); //add to user
                                addUserModule(m, loc_, moduleUser[a1 - 1]); //add user module
                                cout << "This module has been added to " << moduleUser[a1 - 1] << "." << endl;
                              }
                              else {
                                cout << "Before you add this module to " << moduleUser[a1 - 1] << ", you must install its dependencies." << endl
                                     << "Missing Dependencies:" << endl;
                                for (size_t pre = 0; pre < m.dependencies.size(); pre++) {
                                  has = false; //initialize bool test
                                  for (size_t i = 0; i < arr.size(); i++) { //for each module
                                    if (uCase(arr[i]) == uCase(m.dependencies[pre])) //if module name matches dependency
                                      has = true; //has
                                  }
                                  if (!has)
                                    cout << "  " << uCase(m.dependencies[pre]) << endl; //print dependency
                                }
                              }
                              arr.clear();
                              system("PAUSE");
                            }
                            break;
                          case 15: //Remove from User
                            if (moduleUser[a1 - 1] == "") { //if is not a user specific module
                              do {
                                cls(h_);
                                cout << "Which user would you like to remove this module from?" << endl
                                  << "0 - Cancel" << endl
                                  << "1 - All Users" << endl;
                                loadLists(loc_, modules, moduleUser, users); //load lists
                                for (size_t i = 0; i < users.size(); i++)
                                  cout << i + 2 << " - " << users[i] << endl; //enumerate users
                                cout << endl << ": ";
                                cin >> b; //get user input
                                std::getline(cin, tmp);
                                switch (b) {
                                  case 0: //Cancel
                                    break;
                                  case 1: //All Users
                                    for (size_t i = 0; i < users.size(); i++) //for each user
                                      removeFromUser(m, loc_, users[i]); //remove from user
                                    cls(h_);
                                    cout << "This module has been removed from all users." << endl;
                                    system("PAUSE");
                                    break;
                                  default:
                                    if (b > 1 && b < (int)users.size() + 2) { //if user input is appropriate
                                      removeFromUser(m, loc_, users[b - 2]); //remove from user
                                      cls(h_);
                                      cout << "This module has been removed from the user " << users[b - 2] << "." << endl;
                                      system("PAUSE");
                                    }
                                    break;
                                }
                              } while (b > 0); //loop until 0
                            }
                            else { //if it is a user specific module
                              removeFromUser(m, loc_, moduleUser[a1 - 1]); //remove from user it belongs to
                              removeUserModule(m, loc_, moduleUser[a1 - 1]); //remove user module
                              cls(h_);
                              cout << "This module has been removed from the user " << moduleUser[a1 - 1] << "." << endl;
                              system("PAUSE");
                            }
                            break;
                          case 16: //Create User Copy
                            if (moduleUser[a1 - 1] == "") { //if it is not a user specific module
                              do {
                                loadLists(loc_, modules, moduleUser, users); //load lists
                                string tmpStr = "";
                                cls(h_);
                                cout << "Select a user to add the private module to." << endl;
                                for (size_t i = 0; i < users.size(); i++)
                                  cout << "[" << i + 1 << "] - " << users[i] << endl; //enumerate users
                                cout << endl
                                  << "0 - Return" << endl
                                  << "X - Add to User [X]" << endl
                                  << ": ";
                                cin >> b; //get user input
                                std::getline(cin, tmpStr);
                                switch (b) {
                                  case 0: //Return
                                    break;
                                  default: //Add to User
                                    if (b > 0 && b < (int)users.size() + 1) { //if user input is appropriate

                                      if (!fileExists(loc_ + "data/user/" + users[b - 1] + "/data/module/" + m.name + ".module")) { //if the user doesnt already have this file
                                        arr.clear();
                                        loadFileS(loc_ + "data/user/" + users[b - 1] + "/data/modules", arr);
                                        has = false;
                                        for (size_t mods = 0; mods < arr.size(); mods++) {
                                          if (uCase(arr[mods]) == uCase(m.name))
                                            has = true;
                                        }
                                        if (has) { //it has module installed
                                          copyFile(loc_ + "data/module/" + lCase(m.name) + ".module", loc_ + "data/user/" + users[b - 1] + "/data/module/" + lCase(m.name) + ".module"); //copy it
                                          addToUser(m, loc_, users[b - 1]); //add to user
                                          addUserModule(m, loc_, users[b - 1]); //add user module
                                          cls(h_);
                                          cout << "An instance of this module has been copied to " << users[b - 1] << "." << endl;
                                          system("PAUSE");
                                        }
                                        else {
                                          cls(h_);
                                          cout << "You must first add this module normally to the user " << users[b - 1] << "." << endl;
                                          system("PAUSE");
                                        }
                                      }
                                      else { //user already has a copy of this module
                                        cls(h_);
                                        cout << "This user already has an instance of this module." << endl;
                                        system("PAUSE");
                                      }
                                    }
                                    break;
                                }
                              } while (b > 0); //loop until 0
                            }
                            break;
                          case 17: //Compile for Release

                            string tmpDir = loc_ + "temp/" + lCase(m.name);
                            deleteFolder(tmpDir);
                            makeFolder(tmpDir);
                            makeFolder(tmpDir + "/variable");
                            makeFolder(tmpDir + "/source");
                            makeFolder(tmpDir + "/resource");

                            copyFile(loc_ + "data/module/" + lCase(m.name) + ".module", tmpDir + "/" + lCase(m.name) + ".module");
                            copyFile(loc_ + "data/module/" + lCase(m.name) + ".functions", tmpDir + "/" + lCase(m.name) + ".functions");
                            copyFile(loc_ + "data/setting/module/" + lCase(m.name) + ".settings", tmpDir + "/" + lCase(m.name) + ".settings");
                            copyFile(loc_ + "source/module/" + lCase(m.name) + "_M.cpp", tmpDir + "/" + lCase(m.name) + "_M.cpp");
                            copyFile(loc_ + "source/module/" + lCase(m.name) + "_M.h", tmpDir + "/" + lCase(m.name) + "_M.h");
                            copyFile(loc_ + "source/resource/module/" + lCase(m.name) + ".resource", tmpDir + "/" + lCase(m.name) + ".resource");

                            for (int i = 0; i < (int)m.variables.size(); i++) {
                              copyFile(loc_ + "source/variable/" + lCase(m.variables[i]) + "_V.cpp", tmpDir + "/variable/" + lCase(m.variables[i]) + "_V.cpp");
                              copyFile(loc_ + "source/variable/" + lCase(m.variables[i]) + "_V.h", tmpDir + "/variable/" + lCase(m.variables[i]) + "_V.h");
                              copyFile(loc_ + "data/variable/" + lCase(m.variables[i]) + ".variable", tmpDir + "/variable/" + lCase(m.variables[i]) + ".variable");
                            }

                            for (int i = 0; i < (int)m.solutionFolders.size(); i++)
                              makeFolder(tmpDir + "/source/" + m.solutionFolders[i]);

                            for (int i = 0; i < (int)m.solutionFiles.size(); i++)
                              copyFile(loc_ + "source/" + m.solutionFiles[i], tmpDir + "/source/" + m.solutionFiles[i]);

                            for (int i = 0; i < (int)m.resourceFiles.size(); i++) {
                              if (m.resourceFiles[i][m.resourceFiles[i].length() - 1] == '/')
                                makeFolder(tmpDir + "/resource/" + m.resourceFiles[i].substr(0, m.resourceFiles[i].length() - 1));
                              else
                                copyFile(loc_ + "resource/module/" + lCase(m.name) + "/" + m.resourceFiles[i], tmpDir + "/resource/" + m.resourceFiles[i]);
                            }

                            if (SetCurrentDirectory(loc_.c_str()))
                              runBatch("bin/batch/zip.bat", vector<string>{"temp", "../", lCase(m.name) + ".dmi", lCase(m.name), "1"}, true);

                            cls(h_);
                            cout << "Compiled module file has been created in 'DLA/temp/'" << endl;
                            system("PAUSE");
                            break;
                        }
                      } while (a > 0); //loop until 0
                    }
                  }
                } while (a1 > 0); //loop until 0
                break;
              case 2 : //Add Module
                if (!user.getAdmin()) {
                  cls(h_);
                  cout << "You need administrator priviledges to add new modules." << endl;
                  system("PAUSE");
                }
                else {
                  do {
                    loadLists(loc_, modules, moduleUser, users); //load lists
                    cls(h_);
                    cout << "Module Count: " << modules.size() << endl
                      << "Modules" << endl;
                    for (size_t i = 0; i < modules.size(); i++)
                      cout << "    " << modules[i] << endl; //print modules to screen
                    cout << endl
                      << "0 - Done Adding Modules" << endl
                      << "1 - Add a Module" << endl << endl
                      << ": ";
                    cin >> a1; //get user input
                    std::getline(cin, tmp);
                    switch (a1)
                      {
                      case 0: //Done Adding Modules
                        break;
                      case 1: //Add a Module
                        Module x; //create new Module
                        bool isnew = true; //initialize bool test

                        //name
                        do {
                          cls(h_);
                          cout << "Enter a name for the new module" << endl;
                          cout << "Name: ";
                          std::getline(cin, tmp); //get user input
                          if (tmp > "") { //if user input is not empty
                            for (size_t i = 0; i < modules.size(); i++) //for each module
                            if (uCase(modules[i]) == uCase(tmp)) //if name matches user input
                              isnew = false; //it is not new
                            if (isnew) //if it is new
                              x.name = tmp; //store name
                          }
                        } while (!isnew); //loop until good name
                        
                        //author
                        x.author = user.getId();

                        //description
                        cls(h_);
                        cout << "Enter a description of the new module" << endl;
                        cout << "Description: ";
                        std::getline(cin, tmp); //get user input
                        x.description = tmp;

                        //Priority
                        do {
                          cls(h_);
                          cout << "Enter a priority for the new module as a double between 0 and 100 inclusive" << endl;
                          cout << "Priority: ";
                          cin >> tmpD; //get user input
                          std::getline(cin, tmp);
                          if (tmpD >= 0 && tmpD <= 100)
                            x.priority = tmpD;
                        } while (!(tmpD >= 0 && tmpD <= 100)); //loop until good priority

                        //save
                        string modfile = loc_ + "data/module/" + x.name; //modfile string
                        makeFunctions(x, loc_); //make .function file
                        makeCpp(x, loc_); //make .cpp file
                        makeH(x, loc_); //make .h file
                        makeSettings(x, loc_); //make .settings file
                        makeResource(x, loc_); //make .resource file
                        saveModule(x, loc_, "", modfile); //save .module file
                        break;
                    }
                  } while (a1 > 0); //loop until 0
                }
                break;
              case 3 : //Remove Module
                do {
                  loadLists(loc_, modules, moduleUser, users); //load lists
                  cls(h_);
                  cout << "Proceed with caution!!!" << endl << endl;
                  cout << "Module Count: " << modules.size() << endl
                       << "Modules: "                        << endl;
                  for (size_t i = 0; i < modules.size(); i ++)
                    cout << "    [" << i + 1 << "] " << modules[i] << endl; //enumerate modules to screen
                  cout << endl
                       << "0 - Done Removing Modules" << endl
                       << "X - Remove Module [X]" << endl << endl
                       << ": ";
                  cin >> a1; //get user input
                  std::getline(cin, tmp);
                  if (a1 > 0 && a1 <= (int)modules.size()) { //if user input is appropriate

                    if (!(user.getAdmin() || uCase(user.getUsername()) == uCase(moduleUser[a1 - 1]))) {
                      cls(h_);
                      cout << "You need administrator priviledges to delete modules that don't belong to you." << endl;
                      system("PAUSE");
                    }
                    else {
                      string modfile = loc_ + "data/module/" + modules[a1 - 1] + ".module"; //file for module
                      Module t(modfile); //load Module from file
                      if (moduleUser[a1 - 1] > "") { //if it is a user specific module
                        backupModule(t, loc_, "data/user/" + moduleUser[a1 - 1] + "/data"); //backup module
                        deleteFile(modfile); //delete the .module file
                        removeUserModule(t, loc_, moduleUser[a1 - 1]); //remove module from user
                      }
                      else { //if it is not a user specific module
                        if (user.getId() == t.author)
                          backupModule(t, loc_, "data", true); //final backup the module as author
                        else
                          backupModule(t, loc_, "data"); //backup the module
                        deleteFile(modfile); //delete the .module file
                        modfile = loc_ + "data/module/" + modules[a1 - 1] + ".functions"; //locate the .functions file
                        deleteFile(modfile); //delete the .functions file
                        modfile = loc_ + "source/module/" + lCase(t.name) + "_M.cpp"; //locate the .cpp file
                        deleteFile(modfile); //delete the .cpp file
                        modfile = loc_ + "source/module/" + lCase(t.name) + "_M.h"; //locate the .h file
                        deleteFile(modfile); //delete the .h file

                        //remove resource folder
                        deleteFolder(loc_ + "resource/module/" + lCase(t.name));

                        vector<string> tmpModules; //stores module list
                        vector<string> tmpModuleUser; //store user owner of each module in tmpModule
                        vector<string> tmpUser; //stores a list of users
                        loadLists(loc_, tmpModules, tmpModuleUser, tmpUser); //load lists
                        vector<Module> mods; //stores temporary Module objects
                        for (size_t i = 0; i < tmpModules.size(); i++) { //for each module
                          string mfile = loc_ + "data/module/" + tmpModules[i] + ".module"; //find its file
                          Module tmpMod(mfile); //load the module
                          if (uCase(tmpMod.name) != uCase(t.name)) //if it isnt the selected module
                            mods.push_back(tmpMod); //add it to mods vector
                          tmpMod.~Module(); //free Module
                        }

                        for (size_t v = 0; v < t.variables.size(); v++) { //for each variable in selected module
                          bool stillNeeds = false; //initialize bool test

                          for (size_t i = 0; i < mods.size(); i++) { //for each Module in mods vector
                            for (size_t j = 0; j < mods[i].variables.size(); j++) { //for each variable in mods[i]
                              if (uCase(mods[i].variables[j]) == uCase(t.variables[v])) //if variable is in another module
                                stillNeeds = true; //it is still needed
                            }
                          }

                          if (stillNeeds == false) { //if variable is not still needed
                            //delete .cpp file
                            modfile = loc_ + "source/variable/" + lCase(t.variables[v]) + "_V.cpp";
                            deleteFile(modfile);

                            //delete .h file
                            modfile = loc_ + "source/variable/" + lCase(t.variables[v]) + "_V.h";
                            deleteFile(modfile);

                            //delete .variable file
                            modfile = loc_ + "data/variable/" + lCase(t.variables[v]) + ".variable";
                            deleteFile(modfile);

                            //remove from data/variable/ptrs file
                            vector<string> varptriables; //stores lines of data/variable/ptrs file
                            string varptrnamefile = loc_ + "data/variable/ptrs"; //data/variable/ptrs file
                            fstream dvptrs; //create a file stream
                            loadFileS(varptrnamefile, varptriables); //load varptrnamefile into varptriables vector
                            dvptrs.open(varptrnamefile, fstream::out); //open same file for output
                            for (size_t i = 0; i < varptriables.size(); i++) { //for each line in varptriables
                              if (varptriables[i] != "#include \"" + lCase(t.variables[v]) + ".variable\"") //if it is not the line for the variable we are removing
                                dvptrs << varptriables[i] << endl; //print it back to the file
                            }
                            dvptrs.close(); //close the file

                            //remove from source/resource/variables file
                            vector<string> variables; //stores lines from source/resource/variables file
                            string varnamefile = loc_ + "source/resource/variables"; //source/resource/variables file
                            fstream srvars; //create a file stream
                            loadFileS(varnamefile, variables); //load varnamefile into variables vector
                            srvars.open(varnamefile, fstream::out); //open same file for output
                            for (size_t i = 0; i < variables.size(); i++) { //for each line in variables
                              if (variables[i] != "#include \"../variable/" + lCase(t.variables[v]) + "_V.h\"") //if it is not the line for the variable we are removing
                                srvars << variables[i] << endl; //print it back to the file
                            }
                            srvars.close(); //close file
                          }
                        }

                        for (size_t i = 0; i < mods.size(); i++) //for each Module in mods
                          mods[i].~Module(); //free Module

                        removeFromDla(t, loc_); //remove from DLA
                      }
                      cls(h_);
                      cout << "A final backup has been created in 'backup/module'. To restore a deleted module, replace the files." << endl;
                      system("PAUSE");
                    }
                  }
                } while (a1 > 0); //loop until 0
                break;
            }
          } while (a0 > 0); //loop until 0
        }
        user.~User();
        break;
    }
  } while (a00 > 0);
  return 0; //exit
}


void loadLists(string loc, vector<string>& modules, vector<string>& moduleUser, vector<string>& users)
{
  modules.clear(); //empty modules
  moduleUser.clear(); //empty moduleUser
  users.clear(); //empty users
  vector<string> tmpModules;
  readDirectory(loc + "data/module/", ".module", modules); //read module directory for all .modules files and store list in modules vector
  for (size_t i = 0; i < modules.size(); i++) //for each module
    moduleUser.push_back(""); //make a space in moduleUser
  loadFileS(loc + "data/users", users); //load users file into users vector
  for (size_t i = 0; i < users.size(); i++) { //for each user
    tmpModules.clear(); //empty tmpModules
    readDirectory(loc + "data/user/" + users[i] + "/data/module/", ".module", tmpModules); //read user module directory for all .module files and store list in tmpModules vector
    for (size_t j = 0; j < tmpModules.size(); j++) { //for each tmpModule
      modules.push_back("../user/" + users[i] + "/data/module/" + tmpModules[j]); //add to modules vector with user indicator
      moduleUser.push_back(users[i]); //add name of user to cooresponding index in moduleUser
    }
  }
}


void backupModule(Module m, string loc, string modloc)
{
  backupModule(m, loc, modloc, false);
}
void backupModule (Module m, string loc, string modloc, bool resource)
{
  //modloc might be "data" or "data/user/<username>/data"
  string modfile = "";
  string newfile = "";
  string forwardStr = "";
  string backStr = "";

  bool isUser = false;
  if (modloc.find("data/user/") != string::npos)
    isUser = true;

  //make backup folder directories
  forwardStr = modloc + "/backup/module/" + lCase(m.name);
  makeFolder(loc + forwardStr);
  string timeStr = str((int)time(0));
  string newdir =  loc + forwardStr + "/" + timeStr; //folder name as timestamp
  makeFolder(newdir);
  
  if (!isUser)
    backStr = "../../../../";
  else
    backStr = "../../../../../../";

  //copy .module file
  modfile = loc + modloc + "/module/" + lCase(m.name) + ".module";
  newfile = newdir + "/" + lCase(m.name) + ".module";
  if (fileExists(modfile))
    copyFile(modfile, newfile);

  //copy .functions file
  modfile = loc + "data/module/" + lCase(m.name) + ".functions";
  newfile = newdir + "/" + lCase(m.name) + ".functions";
  if (fileExists(modfile))
    copyFile(modfile, newfile);

  //copy .cpp file
  modfile = loc + "source/module/" + lCase(m.name) + "_M.cpp";
  newfile = newdir + "/" + lCase(m.name) + "_M.cpp";
  if (fileExists(modfile))
    copyFile(modfile, newfile);

  //copy .h file
  modfile = loc + "source/module/" + lCase(m.name) + "_M.h";
  newfile = newdir + "/" + lCase(m.name) + "_M.h";
  if (fileExists(modfile))
    copyFile(modfile, newfile);

  //copy variables
  for (size_t i = 0; i < m.variables.size(); i++) { //for each variable
    //copy .cpp file
    modfile = loc + "source/variable/" + lCase(m.variables[i]) + "_V.cpp";
    newfile = newdir + "/" + lCase(m.variables[i]) + "_V.cpp";
    if (fileExists(modfile))
      copyFile(modfile, newfile);

    //copy .h file
    modfile = loc + "source/variable/" + lCase(m.variables[i]) + "_V.h";
    newfile = newdir + "/" + lCase(m.variables[i]) + "_V.h";
    if (fileExists(modfile))
      copyFile(modfile, newfile);
  }

  //copy solution files
  makeFolder(newdir + "/source");
  //function copy file with folder structure
  for (size_t i = 0; i < m.solutionFiles.size(); i++) {
    modfile = loc + "source/" + m.solutionFiles[i];
    newfile = newdir + "/" + 
  }

  //copy .settings file
  if (!isUser)
    modfile = loc + "data/setting/module/" + lCase(m.name) + ".settings";
  else
    modfile = loc + modloc + "data/settings/module/" + lCase(m.name) + ".settings";
  newfile = newdir + "/" + lCase(m.name) + ".settings";
  if (fileExists(modfile))
    copyFile(modfile, newfile);

  //copy .resource file
  modfile = loc + "source/resource/module/" + lCase(m.name) + ".resource";
  newfile = newdir + "/" + lCase(m.name) + ".resource";
  if (fileExists(modfile))
    copyFile(modfile, newfile);
  
  //copy resource files
  if (resource)
    copyFolder(loc + "resource/module/" + lCase(m.name), newdir + "/resources");

  //compress backup
  runBatch("../../DLA/bin/batch/zip.bat", vector<string>{forwardStr, backStr, timeStr + ".zip", timeStr, "1"}, true);
}


void saveModule (Module m, string loc, string user, string modloc)
{
  if (modloc.find("..") != string::npos) //if it is a user specific file
    backupModule(m, loc, "data/user/" + user + "/data"); //backup to user folder
  else
    backupModule(m, loc, "data"); //backup to shared folder

  string modfile = modloc + ".module"; //.module file

  fstream moduleFile; //create mew file stream
  moduleFile.open(modfile.c_str(), fstream::out); //open module file as output
  moduleFile << uCase(m.name) << endl //print module elements
             << m.author << endl
             << m.description << endl
             << m.priority << endl;
  
  alphabetize(m.keywords); //alphabetize keywords
  string keystr = "";
  for (size_t i = 0; i < m.keywords.size(); i ++) { //for each keyword
    if (keystr > "") //if it has length
      keystr += ","; //add a comma
    keystr += uCase(m.keywords[i]); //add the keyword to the string
  }
  moduleFile << keystr << endl; //print keyword string to file
  
  alphabetize(m.dependencies); //alphabetize dependencies
  string depstr = "";
  for (size_t i = 0; i < m.dependencies.size(); i ++) { //for each dependency
    if (depstr > "") //if it has length
      depstr += ","; //add a comma
    depstr += uCase(m.dependencies[i]); //add the dependency to the string
  }
  moduleFile << depstr << endl; //print dependency string to file
  
  alphabetize(m.variables); //alphabetize variables
  string varstr = "";
  for (size_t i = 0; i < m.variables.size(); i ++) { //for each variable
    if (varstr > "") //if it has length
      varstr += ","; //add a comma
    varstr += uCase(m.variables[i]); //add the variable to the string
  }
  moduleFile << varstr << endl; //print variable string to file
  
  alphabetize(m.resourceFiles); //alphabetize resource files
  string rscfstr = "";
  for (size_t i = 0; i < m.resourceFiles.size(); i ++) { //for each resource file
    if (rscfstr > "") //if it has length
      rscfstr += ","; //add a comma
    rscfstr += m.resourceFiles[i]; //add the resource file to the string
  }
  moduleFile << rscfstr << endl; //print resource file string to file
  
  alphabetize(m.dataFiles); //alphabetize data files
  string datafstr = "";
  for (size_t i = 0; i < m.dataFiles.size(); i ++) { //for each data file
    if (datafstr > "") //if it has length
      datafstr += ","; //add a comma
    datafstr += m.dataFiles[i]; //add the data file to the string
  }
  moduleFile << datafstr << endl; //print data file string to file
  
  alphabetize(m.solutionFolders); //alphabetize solution folders
  string slnfldstr = "";
  for (size_t i = 0; i < m.solutionFolders.size(); i ++) { //for each solution folder
    if (slnfldstr > "") //if it has length
      slnfldstr += ","; //add a comma
    slnfldstr += m.solutionFolders[i]; //add the solution folder to the string
  }
  moduleFile << slnfldstr << endl; //print solution folder string to file
  
  //alphabetize(m.solutionFiles); //alphabetize solution files
  string slnfstr = "";
  for (size_t i = 0; i < m.solutionFiles.size(); i ++) { //for each solution file
    if (slnfstr > "") //if it has length
      slnfstr += ","; //add a comma
    slnfstr += m.solutionFiles[i]; //add the solution file to the string
  }
  moduleFile << slnfstr << endl; //print solution file string to file

  for (size_t i = 0; i < m.solutionProperties.size(); i++) { //for each solution properties group
    moduleFile << i + 1 << endl; //print number of property group
    moduleFile << m.solutionPropertyCounts[i] << endl; //print count of properties in the group
    for (int j = 0; j < m.solutionPropertyCounts[i]; j++) //for each property
      moduleFile << m.solutionProperties[i][j] << endl; //print to file
  }
  moduleFile << "0" << endl; //indicate end of solution properties in file

  //alphabetize commands by name
  for (int i = 0; i < m.commandCount; i++) {
    for (int j = 0; j < m.commandCount; j++) {
      if (m.commands[j].name > m.commands[i].name)
        swap(m.commands[j], m.commands[i]);
    }
  }

  for (int i = 0; i < m.commandCount; i ++) { //for each command
    Command& n = m.commands[i]; //store pointer to Command in n
    moduleFile << i + 1 << endl //print command elements
               << n.name << endl
               << n.description << endl
               << n.definition << endl
               << n.returnType << endl
               << n.module << endl
               << n.modulePriority << endl
               << n.priority << endl;

    //alphabetize models by match
    for (int j = 0; j < n.modelCount; j++) {
      for (int k = 0; k < n.modelCount; k++) {
        if (n.models[k].match > n.models[j].match)
          swap(n.models[k], n.models[j]);
      }
    }

    for (int j = 0; j < n.modelCount; j ++) { //for each model of command n
      Command::Model& p = n.models[j]; //store pointer to Model in p
      moduleFile << j + 1 << endl //print model elements
                 << p.match << endl
                 << p.priority << endl
                 << p.commandType << endl
                 << p.questionType << endl
                 << p.question << endl;
    }
    moduleFile << "0" << endl //indicate end of models in file
               << n.output << endl //print command elements
               << n.risk << endl;
  }
  moduleFile << "0"; //indicate end of commands in file
  makeFunctions(m, loc); //make .functions file
}


void loadSettings(string loc, vector<string>& names, vector<string>& values, string file)
{
  string tmpStr;
  fstream fs; //create new file stream
  fs.open(file, fstream::in); //open file for input
  names.clear(); //clear names vector
  values.clear(); //clear values vector
  std::getline(fs, tmpStr); //read name of setting module
  while (std::getline(fs, tmpStr)) { //until end of file
    names.push_back(tmpStr); //add name to vector
    std::getline(fs, tmpStr); //read value
    values.push_back(tmpStr); //add value to vector
  }
  fs.close(); //close file
}
void saveSettings(Module m, string loc, vector<string>& names, vector<string>& values, string file)
{
  bool userModule = (file.find("data/user/") != string::npos); //determine if it is a user specific module or not

  if (!userModule) { //if it is not a user specific module
    vector<string> users; //stores users list
    loadFileS(loc + "data/users", users); //load users list

    vector<string> defNames; //stores tmp setting names
    vector<string> defValues; //stores tmp setting values
    string defFile = loc + "data/setting/module/" + lCase(m.name) + ".settings"; //the default setting file before it was changed
    if (fileExists(defFile)) { //if the setting existed before

      loadSettings(loc, defNames, defValues, defFile); //load it

      for (size_t i = 0; i < users.size(); i++) { //look at settings of module m for each user
        vector<string> uNames;
        vector<string> uValues;
        string userFile = loc + "data/user/" + users[i] + "/data/setting/module/" + lCase(m.name) + ".settings"; //the setting file for user users[i] and module m
        if (fileExists(userFile)) { //if user has module installed
          loadSettings(loc, uNames, uValues, userFile); //load their settings
          for (size_t j = 0; j < names.size(); j++) { //for each setting
            bool alreadyHas = false; //initialize bool test
            int settingIndex = -1; //set index to unusable value
            for (size_t k = 0; k < uNames.size(); k++) { //for each setting in module
              if (uCase(uNames[k]) == uCase(names[j])) { //if the setting name matches
                alreadyHas = true; //it already has it
                settingIndex = k; //store setting index
              }
            }
            if (!alreadyHas) { //if it is a new variable
              uNames.push_back(names[j]);
              uValues.push_back(values[j]);
              settingIndex = (int)uNames.size() - 1; //store setting index
            }
            else { //if it does already have it
              int defIndex = -1;
              for (size_t k = 0; k < defNames.size(); k++) { //find setting in default
                if (uCase(defNames[k]) == uCase(names[j])) { //if it matches a name in default settings
                  defIndex = k; //store index
                }
              }
              if (defIndex > -1) { //the setting was found in default
                if (uValues[settingIndex] == defValues[defIndex]) //the user setting was the default value
                  uValues[settingIndex] = values[j]; //set user setting to the new default
              }
            }
          }

          fstream fs;
          fs.open(userFile, fstream::out);
          fs << uCase(m.name) << endl; //print name to file
          for (size_t j = 0; j < uNames.size(); j++) { //for each setting
            fs << uNames[j] << endl << uValues[j] << endl; //print name and value to file
          }
        }

      }
    }
  }

  //save normal setting file
  fstream fs; //create new file stream
  fs.open(file, fstream::out); //open file for output
  fs << lCase(m.name) << endl; //print name
  for (size_t i = 0; i < names.size(); i++) { //for each setting
    fs << lCase(names[i]) << endl << values[i] << endl; //print name and value to file
  }
  fs.close(); //close file
  
}


void makeFunctions (Module m, string loc)
{
  fstream fsi; //create new input file stream
  fstream fso; //create new output file stream
  fsi.open(loc + "source/module/" + lCase(m.name) + "_M.h", fstream::in); //open .h file for input
  fso.open(loc + "data/module/" + lCase(m.name) + ".functions", fstream::out); //open .functions file for output
  string line = "";

  bool reading = false; //initialize reading to off
  while (std::getline(fsi, line)) { //read lines until end of file
    if (line == "  namespace PRIVATE") //if found end marker
      reading = false; //turn off reading
    if (reading) { //if reading
      if (line != "" && line != "{") { //if it isnt an empty line or brace line
        string name = "";
        bool hit = false; //initialize bool test
        int j = 0; //initialize counter to 0
        for (size_t i = 0; i < line.length(); i++) { //go through line char by char
          if (hit == true && name == "") { //if hit and name is empty
            if (line.substr(i, 1) == " ") { //if next char is space
              do { 
                i++;
              } while (line.substr(i, 1) == " "); //move until space is over
              name = line.substr(i); //save name
              do {
                j++;
              } while (name.substr(j, 1) != " " && name.substr(j, 1) != "("); //move until next char in name is a space or parenthesis
              name = name.substr(0, j); //modify name
            }
          }
          if (line.substr(i, 1) != " ") //if found non space char
            hit = true; //hit
        }

        fso << "CommandPtrs_[\"" << uCase(m.name) << "::" << name << "\"] = &" << uCase(m.name) << "::" << name << ";" << endl; //output line to .functions file
      }
    }
    if (line == "namespace " + uCase(m.name)) //if found beginning marker
      reading = true; //turn reading on
  }

  fsi.close(); //close file
  fso.close(); //close file
}
void makeCpp (Module m, string loc)
{
  fstream fs; //create new file stream
  string openfile = loc + "source/module/" + m.name + "_M.cpp"; //.cpp file
  fs.open(openfile.c_str(), fstream::out); //open .cpp file for output

  //output standard file
  fs << "////////////////////////////////////////////////////////////////////////////////" << endl
    << "//File:   " << m.name << "_M.cpp" << endl
    << "//Dir:    source/module/" << endl
    << "//Date:   YYYY-MM-DD" << endl
    << "//Author: Name" << endl
    << "//Implementation of " << uCase(m.name) << " module" << endl
    << "////////////////////////////////////////////////////////////////////////////////" << endl << endl << endl
    << "//constants" << endl
    << "#include \"../resource/const.h\"" << endl << endl << endl
    << "//standard includes" << endl
    << "#include <string>" << endl << endl << endl
    << "//includes" << endl
    << "#include \"" << m.name << ".h\"" << endl
    << "#include \"../header/module.h\"" << endl
    << "#include \"../header/settings.h\"" << endl << endl << endl
    << "//namespaces" << endl
    << "using namespace std;" << endl << endl << endl
    << "//functions" << endl << endl
    << "string " << uCase(m.name) << "::main ()" << endl
    << "{" << endl
    << "  if (!settings_.hasSettingGroup(\"" << uCase(m.name) << "\", SETTING_MODULE))" << endl
    << "    " << uCase(m.name) << "::PRIVATE::defaultSettings();" << endl << endl
    << "  " << uCase(m.name) << "::PRIVATE::loadResources();" << endl << endl
    << "  return \"\";" << endl
    << "}" << endl << endl << endl << endl << endl << endl
    << "void " << uCase(m.name) << "::PRIVATE::defaultSettings()" << endl
    << "{" << endl
    << "  SettingGroup sg;" << endl
    << "  sg.setName(\"" << uCase(m.name) << "\");" << endl
    << "  sg.setType(SETTING_MODULE);" << endl
    << "  sg.setFile(\"data/setting/module/" << lCase(m.name) << ".settings\");" << endl
    << "  settings_.addSettingGroup(sg);" << endl
    << "  return;" << endl
    << "}" << endl << endl << endl << endl
    << "void " << uCase(m.name) << "::PRIVATE::loadResources()" << endl
    << "{" << endl
    << "  return;" << endl
    << "}";

  fs.close(); //close file
}
void makeH (Module m, string loc)
{
  fstream fs; //create new file stream
  string openfile = loc + "source/module/" + m.name + "_M.h"; //.h file
  fs.open(openfile.c_str(), fstream::out); //open .h file for output

  //output standard file
  fs << "////////////////////////////////////////////////////////////////////////////////" << endl
     << "//File:   " << m.name << "_M.h" << endl
     << "//Dir:    source/module/" << endl
     << "//Date:   YYYY-MM-DD" << endl
     << "//Author: Name" << endl
     << "//Interface of " << uCase(m.name) << " module" << endl
     << "////////////////////////////////////////////////////////////////////////////////" << endl << endl << endl
     << "#ifndef _JARVIS_SM_" << uCase(m.name) << "_H_" << endl
     << "#define _JARVIS_SM_" << uCase(m.name) << "_H_" << endl << endl << endl
     << "//constants" << endl
     << "#include \"../resource/const.h\"" << endl << endl << endl
     << "//standard includes" << endl
     << "#include <string>" << endl
     << "#include <vector>" << endl << endl << endl
     << "//includes" << endl
     << "#include \"../header/module.h\"" << endl
     << "#include \"../header/settings.h\"" << endl << endl << endl
     << "//namespace defintions" << endl
     << "namespace " << uCase(m.name) << endl
     << "{" << endl
     << "  std::string main       ();" << endl << endl
     << "  namespace PRIVATE" << endl
     << "  {" << endl
     << "    void defaultSettings ();" << endl
     << "  } ;" << endl
     << "} ;" << endl << endl << endl
     << "//global variable definitions" << endl
     << "extern bool                 terminate_;" << endl
     << "extern int                  moduleCount_;" << endl
     << "extern std::vector<Module>  modules_;" << endl
     << "extern Settings             settings_;" << endl << endl << endl
     << "#endif";

  fs.close(); //close file
}
void makeSettings(Module m, string loc)
{
  fstream fs; //create new file stream
  string openfile = loc + "data/setting/module/" + m.name + ".settings"; //.settings file
  fs.open(openfile.c_str(), fstream::out); //open .settings file for output 

  //output standard file
  fs << lCase(m.name) << endl;
  
  fs.close(); //close file
}
void makeVariables(Module m, string loc)
{
  for (size_t i = 0; i < m.variables.size(); i++) { //for each variable
    fstream fs; //create new file stream
    //.h
    string openfile = loc + "source/variable/" + lCase(m.variables[i]) + "_V.h"; //.h file
    if (!fileExists(openfile)) { //if it doesnt exists
      fs.open(openfile.c_str(), fstream::out); //open .h file for output
      
      //output standard file
      fs << "////////////////////////////////////////////////////////////////////////////////" << endl
         << "//File:   " << lCase(m.variables[i]) << "_V.h" << endl
         << "//Dir:    source/variable/" << endl
         << "//Date:   YYYY-MM-DD" << endl
         << "//Author: Name" << endl
         << "//Interface of " << uCase(m.variables[i]) << " Variable" << endl
         << "////////////////////////////////////////////////////////////////////////////////" << endl << endl << endl
         << "#ifndef _DLA_SV_" << uCase(m.variables[i]) << "_H_" << endl
         << "#define _DLA_SV_" << uCase(m.variables[i]) << "_H_" << endl << endl << endl
         << "//constants" << endl
         << "#include \"../resource/const.h\"" << endl << endl << endl
         << "//standard includes" << endl
         << "#include <string>" << endl << endl << endl
         << "//function definitions" << endl
         << "double is" << uCase(m.variables[i]) << " (std::string);" << endl << endl << endl
         << "//shared function definitions" << endl
         << "#include \"../resource/common.h\"" << endl << endl << endl
         << "#endif";

      fs.close(); //close file
    }

    //.cpp    
    openfile = loc + "source/variable/" + lCase(m.variables[i]) + "_V.cpp"; //.cpp file
    if (!fileExists(openfile)) { //if it doesnt exist
      fs.open(openfile.c_str(), fstream::out); //open .cpp file for output

      //output standard file
      fs << "////////////////////////////////////////////////////////////////////////////////" << endl
         << "//File:   " << lCase(m.variables[i]) << "_V.cpp" << endl
         << "//Dir:    source/variable/" << endl
         << "//Date:   YYYY-MM-DD" << endl
         << "//Author: Name" << endl
         << "//Implementation of " << uCase(m.variables[i]) << " variable" << endl
         << "////////////////////////////////////////////////////////////////////////////////" << endl << endl << endl
         << "//constants" << endl
         << "#include \"../resource/const.h\"" << endl << endl << endl
         << "//standard includes" << endl
         << "#include <string>" << endl << endl << endl
         << "//includes" << endl
         << "#include \"" << lCase(m.variables[i]) << "_V.h\"" << endl << endl << endl
         << "//namespaces" << endl
         << "using namespace std;" << endl << endl << endl
         << "//functions" << endl
         << "double is" << uCase(m.variables[i]) << "(string s)" << endl
         << "{" << endl
         << "  double maxScore = 100.0;" << endl << endl
         << "  double score = 0.0;" << endl
         << "  //calculate score" << endl
         << "  return score / maxScore;" << endl
         << "}";

      fs.close(); //close file
    }

    //.variable 
    openfile = loc + "data/variable/" + lCase(m.variables[i]) + ".variable"; //.variable file
    if (!fileExists(openfile)) { //if it doesnt exist
      fs.open(openfile.c_str(), fstream::out); //open .variable file for output

      //output standard file
      fs << "if (!hasVariable(\"%" << uCase(m.variables[i]) << "%\")) {" << endl
         << "VariablePtrs_[\"%" << uCase(m.variables[i]) << "%\"] = &is" << uCase(m.variables[i]) << ";" << endl
         << "}";

      fs.close(); //close file
    }

  }
}
void makeResource(Module m, string loc) {
  fstream fs; //create new file stream
  string openfile = loc + "source/resource/variable/" + lCase(m.name) + ".resource"; //.resource file
  if (!fileExists(openfile)) { //if it doesnt exists
    fs.open(openfile.c_str(), fstream::out); //open .resource file for output
    fs.close();
  }
}

void addToDla(Module m, string loc)
{
  removeFromDla(m, loc, false); //first remove from dla

  //create module directory
  makeFolder(loc + "resource/module/" + lCase(m.name));

  //add name to module file
  vector<string> modules; //stores module names
  loadFileS(loc + "data/modules", modules); //load modules file into modules vector
  bool has = false; //initialize bool test
  for (size_t i = 0; i < modules.size(); i++) { //for each module
    if (uCase(modules[i]) == uCase(m.name)) //if module name matches one in file
      has = true; //already has
  }
  if (has == false) //if it doesnt already have it
    modules.push_back(lCase(m.name)); //add it to vector
  alphabetize(modules); //alphabetize modules vector
  fstream fs1; //create new file stream
  string openfile = loc + "modules"; //modules file
  fs1.open(openfile.c_str(), fstream::out); //open modules file for output
  for (size_t i = 0; i < modules.size(); i++) //for each module name in modules
    fs1 << modules[i] << endl; //print it to file
  fs1.close(); //close file

  //add name to module/ptrs file
  modules.clear(); //empty the modules vector
  loadFileS(loc + "data/module/ptrs", modules); //load lines of module/ptrs file
  has = false; //initialize bool test
  string ptrStr = "#include \"" + lCase(m.name) + ".functions\""; //determine line that would coorespond to current module
  for (size_t i = 0; i < modules.size(); i++) { //for each module
    if (uCase(modules[i]) == uCase(ptrStr)) //if line matches one that exists
      has = true; //already has
  }
  if (has == false) //if doesnt already have
    modules.push_back(ptrStr); //add to list
  alphabetize(modules); //alphabetize modules vector
  fstream fs2; //create new file stream
  openfile = loc + "data/module/ptrs"; //module/ptrs file
  fs2.open(openfile.c_str(), fstream::out); //open module/ptrs file for output
  for (size_t i = 0; i < modules.size(); i++) //for each line in modules
    fs2 << modules[i] << endl; //print to file
  fs2.close(); //close file

  //add to resource/modules file
  modules.clear(); //empty modules vector
  loadFileS(loc + "source/resource/modules", modules); //load resource/modules file
  for (size_t i = 0; i < modules.size(); i++) { //for each module
    if (uCase(modules[i]) == uCase("#include \"../module/" + lCase(m.name) + "_M.h\"")) //if module name matches one in file
      has = true; //already has
  }
  if (has == false) //if it doesnt already have it
    modules.push_back("#include \"../module/" + lCase(m.name) + "_M.h\""); //add line cooresponding to current module
  alphabetize(modules); //alphabetize modules vector
  fstream fs3; //create new file stream
  openfile = loc + "source/resource/modules"; //resource/modules file
  fs3.open(openfile.c_str(), fstream::out); //open resource/modules file for output
  for (size_t i = 0; i < modules.size(); i++) //for each line in modules
    fs3 << modules[i] << endl; //output to file
  fs3.close(); //close file
    
  for (size_t i = 0; i < m.variables.size(); i++) { //for each variable
    //add to data/variable/ptrs
    vector<string> varptriables; //stores lines of variable/ptrs file
    string varptrnamefile = loc + "data/variable/ptrs"; //variable/ptrs file
    loadFileS(varptrnamefile, varptriables); //load file
    bool includedalready = false; //initialize bool test
    for (size_t j = 0; j < varptriables.size(); j++) { //for each line in varptriables
      if (varptriables[j] == "#include \"" + lCase(m.variables[i]) + ".variable\"") //if line cooresponds to current variable
        includedalready = true; //it is included already
    }
    if (!includedalready) //if it is not included already
      varptriables.push_back("#include \"" + lCase(m.variables[i]) + ".variable\""); //include it to list
    alphabetize(varptriables); //alphabetize varptriables
    fstream fout; //create new file stream
    fout.open(varptrnamefile, fstream::out); //open same file for output
    for (size_t j = 0; j < varptriables.size(); j++) //for each line in varptriables
      fout << varptriables[j] << endl; //print to file
    fout.close(); //close file
    
    //add to source/resource/variables
    vector<string> variables; //stores lines from resource/variables
    string varnamefile = loc + "source/resource/variables"; //resource/variables file
    loadFileS(varnamefile, variables); //load file
    includedalready = false; //initialize bool test
    for (size_t j = 0; j < variables.size(); j++) { //for each line
      if (variables[j] == "#include \"../variable/" + lCase(m.variables[i]) + "_V.h\"") //if it cooresponds to current variable
        includedalready = true; //included already
    }
    if (!includedalready) //if not included already
      variables.push_back("#include \"../variable/" + lCase(m.variables[i]) + "_V.h\""); //add to list
    alphabetize(variables); //alphabetize variables list
    fstream foutv; //create new file stream
    foutv.open(varnamefile, fstream::out); //open same file for output
    for (size_t j = 0; j < variables.size(); j++) //for each line
      foutv << variables[j] << endl; //print to file
    foutv.close(); //close file
  }

  //add to source/resource/module/resources file
  modules.clear(); //empty modules vector
  loadFileS(loc + "source/resource/module/resources", modules); //load resources/module/resource file
  for (size_t i = 0; i < modules.size(); i++) { //for each module
    if (uCase(modules[i]) == uCase("#include \"" + lCase(m.name) + ".resource\"")) //if module name matches one in file
      has = true; //already has
  }
  if (has == false) //if it doesnt already have it
    modules.push_back("#include \"" + lCase(m.name) + ".resource\""); //add line cooresponding to current module
  alphabetize(modules); //alphabetize modules vector
  fstream fs4; //create new file stream
  openfile = loc + "source/resource/module/resources"; //resource/modules file
  fs4.open(openfile.c_str(), fstream::out); //open resource/modules file for output
  for (size_t i = 0; i < modules.size(); i++) //for each line in modules
    fs4 << modules[i] << endl; //output to file
  fs4.close(); //close file

}

void removeFromDla(Module m, string loc)
{
  removeFromDla(m, loc, true); //default removeUsers set to true
}
void removeFromDla (Module m, string loc, bool removeUsers)
{
  //remove from module file
  vector<string> modules; //stores lines of data/modules
  loadFileS(loc + "data/modules", modules); //load file
  alphabetize(modules); //alphabetize modules vector
  fstream fs1; //create new file stream
  string openfile = loc + "data/modules"; //data/modules file
  fs1.open(openfile.c_str(), fstream::out); //open data modules file for output
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (uCase(modules[i]) != uCase(m.name)) //if it doesnt coorespond to the current module
      fs1 << modules[i] << endl; //print it back to the file
  }
  fs1.close(); //close file

  //remove from module/ptrs file
  modules.clear(); //empty modules vector
  loadFileS(loc + "data/module/ptrs", modules); //load module/ptrs file
  alphabetize(modules); //alphabetize modules vector
  fstream fs2; //create new file stream
  openfile = loc + "data/module/ptrs"; //module/ptrs file
  fs2.open(openfile.c_str(), fstream::out); //open module/ptrs file for output
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (modules[i] != "#include \"" + lCase(m.name) + ".functions\"") //if line doesnt correspond to current module 
      fs2 << modules[i] << endl; //print it back to file
  }
  fs2.close(); //close file

  //remove from resource/modules
  modules.clear(); //empty modules vector
  loadFileS(loc + "source/resource/modules", modules); //load resource/modules file
  alphabetize(modules); //alphabetize modules vector
  fstream fs3; //create new file stream
  openfile = loc + "source/resource/modules"; //resource/modules file
  fs3.open(openfile.c_str(), fstream::out); //open resource/modules file
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (modules[i] != "#include \"../module/" + lCase(m.name) + "_M.h\"") //if line doesnt correspond with current module
      fs3 << modules[i] << endl; //print back to file
  }
  fs3.close(); //close file
  
  //remove from users
  if (removeUsers) { //if removeUsers flag is true
    vector<string> users; //stores the list of users
    loadFileS(loc + "data/users", users); //load users file
    for (size_t i = 0; i < users.size(); i++) //for each user
      removeFromUser(m, loc, users[i]); //remove module from user
  }
  
  //remove variables
  vector<string> tmpModules; //stores list of modules
  vector<string> tmpModuleUser; //stores list of user ownerships of tmpModules
  vector<string> tmpUser; //store list of users
  loadLists(loc, tmpModules, tmpModuleUser, tmpUser); //load lists
  vector<Module> mods; //stores temp Module files
  for (size_t i = 0; i < tmpModules.size(); i++) { //for each module
    string mfile = loc + "data/module/" + tmpModules[i] + ".module"; //module file
    Module tmpMod(mfile); //create Module
    if (uCase(tmpMod.name) != uCase(m.name)) //if name is not the same as current module
      mods.push_back(tmpMod); //add to mods vector
    tmpMod.~Module(); //free Module
  }

  for (size_t v = 0; v < m.variables.size(); v++) { //for each variable
    bool stillNeeds = false; //initialize bool test

    for (size_t i = 0; i < mods.size(); i++) { //for each Module
      for (size_t j = 0; j < mods[i].variables.size(); j++) { //for each variable in mods[i]
        if (uCase(mods[i].variables[j]) == uCase(m.variables[v])) //if name matches current variable
          stillNeeds = true; //still needs it
      }
    }

    if (stillNeeds == false) { //if doesnt still need it
      //remove from data/variable/ptrs file
      vector<string> varptriables; //stores variable/ptrs
      string varptrnamefile = loc + "data/variable/ptrs"; //variable/ptrs file
      fstream dvptrs; //create new file stream
      loadFileS(varptrnamefile, varptriables); //load file
      alphabetize(varptriables); //alphbetizes varptriables vector
      dvptrs.open(varptrnamefile, fstream::out); //open same file for output
      for (size_t i = 0; i < varptriables.size(); i++) { //for each line
        if (uCase(varptriables[i]) != uCase("#include \"" + lCase(m.variables[v]) + ".variable\"")) // if it doesnt correspond to current variable
          dvptrs << varptriables[i] << endl; //print it back to file
      }
      dvptrs.close(); //close file

      //remove from source/resource/variables file
      vector<string> variables; //stores lines of resource/variables
      string varnamefile = loc + "source/resource/variables"; //resource/variables file
      fstream srvars; //create new file stream
      loadFileS(varnamefile, variables); //load file
      alphabetize(variables); //alphabetize variables vector
      srvars.open(varnamefile, fstream::out); //open same file as output
      for (size_t i = 0; i < variables.size(); i++) { //for each line
        string varStr = "#include \"../variable/" + lCase(m.variables[v]) + "_V.h\""; //determine string corresponding to current variable
        if (uCase(variables[i]) != uCase(varStr)) //if it is not that string
          srvars << variables[i] << endl; //print it back to the file
      }
      srvars.close(); //close the file
    }

    for (size_t m = 0; m < mods.size(); m++) //for each Module
      mods[m].~Module(); //free Module
  }

  //remove from source/resource/module/resources file
  modules.clear(); //empty modules vector
  loadFileS(loc + "source/resource/module/resources", modules); //load resource/modules file
  alphabetize(modules); //alphabetize modules vector
  fstream fs4; //create new file stream
  openfile = loc + "source/resource/module/resources"; //resource/modules file
  fs4.open(openfile.c_str(), fstream::out); //open resource/modules file
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (uCase(modules[i]) != uCase("#include \"" + lCase(m.name) + ".resource\"")) //if line doesnt correspond with current module
      fs4 << modules[i] << endl; //print back to file
  }
  fs4.close(); //close file
}

void addToUser(Module m, string loc, string user)
{
  //create user data directory
  makeFolder(loc + "data/user/" + user + "/resource/module/" + lCase(m.name));

  vector<string> modules; //stores user modules lines
  loadFileS(loc + "data/user/" + user + "/data/modules", modules); //load file
  bool has = false; //initialize bool test
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (uCase(modules[i]) == uCase(m.name)) //if name matches current module
      has = true; //already has
  }
  if (has == false) //if doesnt already have
    modules.push_back(lCase(m.name)); //add name to list
  alphabetize(modules); //alphabetize modules vector
  fstream fs1; //create new file stream
  string openfile = loc + "data/user/" + user + "/data/modules"; //file
  fs1.open(openfile.c_str(), fstream::out); //open same file for output
  for (size_t i = 0; i < modules.size(); i++) //for each line
    fs1 << modules[i] << endl; //print to file
  fs1.close(); //close file

  string source = loc + "data/setting/module/" + lCase(m.name) + ".settings"; //.settings file
  string dest = loc + "data/user/" + user + "/data/setting/module/" + lCase(m.name) + ".settings"; //new location for .settings file
  if (!fileExists(dest))
    copyFile(source, dest);
}

void removeFromUser(Module m, string loc, string user)
{
  vector<string> modules; //stores list of modules
  loadFileS(loc + "data/user/" + user + "/data/modules", modules); //load file
  alphabetize(modules); //alphabetize modules vector
  fstream fs1; //create new file stream
  string openfile = loc + "data/user/" + user + "/data/modules"; //file
  fs1.open(openfile.c_str(), fstream::out); //open same file for output
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (uCase(modules[i]) != uCase(m.name)) //if name doesnt match current module
      fs1 << modules[i] << endl; //print it back to file
  }
  fs1.close(); //close file

  removeUserModule(m, loc, user); //remove user module
}

void addUserModule(Module m, string loc, string user)
{
  vector<string> modules; //stores list of modules
  loadFileS(loc + "data/user/" + user + "/data/module/ptrs", modules); //load user modules
  bool has = false; //initilialize bool test
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (uCase(modules[i]) == uCase(m.name)) //if name matches current module
      has = true; //already has
  }
  if (has == false) //if doesnt already have
    modules.push_back(lCase(m.name)); //add to list
  alphabetize(modules); //alphabetize modules vector
  fstream fs1; //create new file stream
  string openfile = loc + "data/user/" + user + "/data/module/ptrs"; //module/ptrs file
  fs1.open(openfile.c_str(), fstream::out); //open file for output
  for (size_t i = 0; i < modules.size(); i++) //for each module
    fs1 << modules[i] << endl; //print to file
  fs1.close(); //close file
}

void removeUserModule(Module m, string loc, string user)
{
  vector<string> modules; //stores module list
  loadFileS(loc + "data/user/" + user + "/data/module/ptrs", modules); //load user module list
  alphabetize(modules); //alphabetize modules vector
  fstream fs1; //create new file stream
  string openfile = loc + "data/user/" + user + "/data/module/ptrs"; //file
  fs1.open(openfile.c_str(), fstream::out); //open same file for output
  for (size_t i = 0; i < modules.size(); i++) { //for each line
    if (uCase(modules[i]) != uCase(m.name)) //if name doesnt match current module
      fs1 << modules[i] << endl; //print back to file
  }
  fs1.close(); //close file
}














//Command

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
}
Command::Command(fstream& fs)
{
  readCommandFromFileStream(fs);
}

Command::~Command()
{
}

void Command::Model::dump()
{
  cout << "Match: "         << Command::Model::match        << endl
       << "Priority: "      << Command::Model::priority     << endl
       << "Command Type: "  << Command::Model::commandType  << endl
       << "Question Type: " << Command::Model::questionType << endl
       << "Question: "      << Command::Model::question     << endl;
  cout << endl;
}

void Command::dump ()
{
  cout << "Name: " << Command::name << endl;
  cout << "Description: " << Command::description << endl;
    cout << "Definition: " << Command::definition << endl;
    cout << "Return Type: " << Command::returnType << endl;
    cout << "Priority: " << Command::priority << endl;
    cout << "Model Count: " << Command::modelCount << endl;
    cout << "Output: " << Command::output << endl;
        cout << "Risk: "            << Command::risk        << endl;
  cout << endl;
}

void Command::readCommandFromFileStream (fstream& fs)
{
  if (fs.is_open()) {
    string tmp = "";

    std::getline  (fs, Command::name);
    std::getline  (fs, Command::description);
    std::getline  (fs, Command::definition);
    getint   (fs, Command::returnType);
    std::getline  (fs, Command::module);
    getdouble(fs, Command::modulePriority);
    getdouble(fs, Command::priority);

    Command::modelCount = 0;
    int nextModel = 0;
    getint(fs, nextModel);
    while (nextModel != 0) {
      Command::modelCount = nextModel;
      Model tmpModel;
      models.push_back(tmpModel);

      std::getline  (fs, models[Command::modelCount - 1].match);
      getdouble(fs, models[Command::modelCount - 1].priority);
      getint   (fs, models[Command::modelCount - 1].commandType);
      getint   (fs, models[Command::modelCount - 1].questionType);
      getint   (fs, models[Command::modelCount - 1].question);

      getint(fs, nextModel);
    }

    std::getline  (fs, Command::output);
    getdouble(fs, Command::risk);
  }
}









//Common

bool getint (istream& is, int& outVar)
{
  bool ok = false;
  string tmp;
  if (is >> outVar) {
    ok = true;
    std::getline(is, tmp);
  }
  return ok;
}

bool getdouble (istream& is, double& outVar)
{
  bool ok = false;
  string tmp;
  if (is >> outVar) {
    ok = true;
    std::getline(is, tmp);
  }
  return ok;
}

bool getbool(istream& is, bool& outVar)
{
  bool ok = false;
  string tmp;
  if (is >> outVar) {
    ok = true;
    getline(is, tmp);
  }
  return ok;
}

void cls (HANDLE hConsole)
{
  COORD coordScreen = {0, 0};
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi; 
  DWORD dwConSize;

  GetConsoleScreenBufferInfo(hConsole, &csbi);
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
  SetConsoleCursorPosition(hConsole, coordScreen);
}

string uCase (string s)
{
  boost::to_upper(s);
  return s;
}
string lCase (string s)
{
  boost::to_lower(s);
  return s;
}
string str (int i)
{
  string str;
  ostringstream oss;
  oss << i;
  str = oss.str();
  return str;
}
string str(char c)
{
  stringstream ss;
  string s;
  ss << c;
  ss >> s;
  return s;
}

void loadFileS (string file, vector<string>& data)
{
  string line = "";
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open())
    while (std::getline(fs, line)) {
      data.push_back(line);
    }
  fs.close();
}

string loadFileStr(string file)
{
  string out = "";
  vector<string> v;
  loadFileS(file, v);
  for (size_t i = 0; i < v.size(); i++)
    out += v[i] + "\n";
  return out;
}

string removePunctuation(string text)
{
  for (size_t i = 0; i < text.length(); i++) {
    if (ispunct(text[i]))
      text.erase(i--, 1);
  }
  return text;
}

void removePunctuationSoft(vector<string>& tokens)
{
  int edits;
  do {
    edits = 0;
    for (size_t i = 0; i < tokens.size(); i++) {
      string trial = tokens[i].substr(0, 1);
      bool nextIsNum = false;
      if (tokens.size() > 1) {
        string trial2 = tokens[i].substr(1, 1);
        nextIsNum = (trial2 == "0" || trial2 == "1" || trial2 == "2" || trial2 == "3" || trial2 == "4" || trial2 == "5" || trial2 == "6" || trial2 == "7" || trial2 == "8" || trial2 == "9");
      }
      if ((trial == "." && !nextIsNum) || trial == "!" || trial == "?" || trial == "," || trial == ";" || trial == ":" || trial == "\'" || trial == "\"" || trial == "(" || trial == ")" || trial == "<" || trial == ">" || trial == "{" || trial == "}" || trial == "[" || trial == "]") {
        tokens[i].erase(0, 1);
        edits++;
      }
      trial = tokens[i].substr(tokens[i].length() - 1, 1);
      if (trial == "." || trial == "!" || trial == "?" || trial == "," || trial == ";" || trial == ":" || trial == "\'" || trial == "\"" || trial == "(" || trial == ")" || trial == "<" || trial == ">" || trial == "{" || trial == "}" || trial == "[" || trial == "]") {
        tokens[i].erase(tokens[i].length() - 1, 1);
        edits++;
      }
    }
  } while (edits > 0);
}

void ReplaceStringInPlace(string& subject, string search, string replace)
{
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

void alphabetize(std::vector<std::string>& v)
{
  for (size_t i = 0; i < v.size(); i++) {
    for (size_t j = 0; j < v.size(); j++) {
      if (v[j] > v[i])
        swap(v[j], v[i]);
    }
  }
}

vector<string> tokenize(string s)
{
  return tokenize(s, false);
}
vector<string> tokenize(string s, bool punc)
{
  vector<string> tokens;
  if (!punc)
    s = removePunctuation(s);
  boost::algorithm::split(tokens, s, boost::is_any_of(" "));
  return tokens;
}

void delay(int ms)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

string generateGuid()
{
  UUID uuid;
  ::ZeroMemory(&uuid, sizeof(UUID));
  ::UuidCreate(&uuid);
  WCHAR* wszUuid = NULL;
  ::UuidToStringW(&uuid, (RPC_WSTR*)&wszUuid);
  char cUuid[37];
  char DefChar = ' ';
  WideCharToMultiByte(CP_ACP, 0, wszUuid, -1, cUuid, 37, &DefChar, NULL);
  return cUuid;
}

string getEnvVar(string name)
{
  char lpName[64];
  std::copy(name.begin(), name.end(), lpName);
  lpName[name.size()] = '\0';
  char lpBuffer[128];
  GetEnvironmentVariable(lpName, lpBuffer, 128);
  string out(lpBuffer);
  return out;
}

void runBatch(string bat)
{
  runBatch(bat, vector<string>{});
}
void runBatch(string bat, bool wait)
{
  runBatch(bat, vector<string>{}, wait);
}
void runBatch(string bat, vector<string> args)
{
  runBatch(bat, args, false);
}
void runBatch(string bat, vector<string> args, bool wait)
{
  BOOL rc = 0;
  DWORD st = 0;
  PROCESS_INFORMATION procInfo = { 0 };
  STARTUPINFO startupInfo = { 0 };
  startupInfo.cb = sizeof(startupInfo);
  string batchloc = bat;
  ReplaceStringInPlace(batchloc, "/", "\\");
  for (size_t i = 0; i < args.size(); i++)
    ReplaceStringInPlace(args[i], "/", "\\");
  string cmd = "cmd.exe /c " + batchloc;
  for (size_t i = 0; i < args.size(); i++)
    cmd += " " + args[i];
  rc = CreateProcess(NULL, (char*)(cmd.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW | CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &startupInfo, &procInfo);
  if (rc != 0 && wait)
    st = WaitForSingleObject(procInfo.hProcess, INFINITE);
  CloseHandle(procInfo.hProcess);
  CloseHandle(procInfo.hThread);
}









//Module

Module::Module ()
{
  Module::name = "";
  Module::description = "";
  Module::priority = 0.0;
  Module::commandCount = 0;
}

Module::Module (string modfile)
{
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
        Module::resourceFiles.push_back(token3);
      
      getline  (moduleFile, tmp);
      stringstream ss4(tmp);
      string token4;
      while (getline(ss4, token4, ','))
        Module::dataFiles.push_back(token4);
      
      getline  (moduleFile, tmp);
      stringstream ss5(tmp);
      string token5;
      while (getline(ss5, token5, ','))
        Module::solutionFolders.push_back(token5);

      getline  (moduleFile, tmp);
      stringstream ss6(tmp);
      string token6;
      while (getline(ss6, token6, ','))
        Module::solutionFiles.push_back(token6);

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
}

Module::~Module ()
{
  for (size_t i = 0; i < Module::commands.size(); i ++)
    Module::commands[i].~Command();
}

void Module::dump ()
{
  cout << "Name: "             << Module::name                << endl
       << "Description: "      << Module::description         << endl
       << "Priority: "         << Module::priority            << endl
       << "Dependency Count: " << Module::dependencies.size() << endl
       << "Variable Count: "   << Module::variables.size()    << endl
       << "Command Count: "    << Module::commandCount        << endl;
  cout << endl;
}










//Filesystem

bool fileExists(string name)
{
  fstream file(name.c_str());
  if (!file)
    return false;
  return true;
}


bool makeFile(string file)
{
  if (fileExists(file))
    return false;

  fstream fs;
  fs.open(file, fstream::out);
  fs.close();

  if (fileExists(file))
    return true;
  return false;
}

bool deleteFile(string file)
{
  if (!fileExists(file))
    return false;

  if (remove(file.c_str()) != 0)
    return false;
  return true;
}

bool copyFile(string file1, string file2)
{
  if (!fileExists(file1) || file1 == file2)
    return false;

  if (fileExists(file2))
    deleteFile(file2);
  makeFile(file2);

  ifstream ifs(file1.c_str(), ios::binary);
  ofstream ofs(file2.c_str(), ios::binary);
  ofs << ifs.rdbuf();
  ifs.close();
  ofs.close();

  if (fileExists(file2))
    return true;
  return false;
}

bool moveFile(string file1, string file2)
{
  return renameFile(file1, file2);
}

bool renameFile(string file1, string file2)
{
  if (!fileExists(file1) || file1 == file2)
    return false;

  if (rename(file1.c_str(), file2.c_str()) != 0)
    return false;
  return true;
}


long long fileSize(string file)
{
  if (!fileExists(file))
    return -1;

  WIN32_FILE_ATTRIBUTE_DATA fad;
  if (!GetFileAttributesEx(file.c_str(), GetFileExInfoStandard, &fad))
    return -1;
  LARGE_INTEGER size;
  size.HighPart = fad.nFileSizeHigh;
  size.LowPart = fad.nFileSizeLow;
  return size.QuadPart;
}


bool makeFolder(string source)
{
  if (CreateDirectory(source.c_str(), NULL) == 0)
    return false;
  return true;
}

bool deleteFolder(string path) {
  TCHAR* sPath = (TCHAR*)path.c_str();
  HANDLE hFind;
  WIN32_FIND_DATA FindFileData;

  TCHAR DirPath[MAX_PATH];
  TCHAR FileName[MAX_PATH];

  strcpy(DirPath, sPath);
  strcat(DirPath, "\\*");
  strcpy(FileName, sPath);
  strcat(FileName, "\\");

  hFind = FindFirstFile(DirPath,&FindFileData);
  if(hFind == INVALID_HANDLE_VALUE)
    return false;
  strcpy(DirPath,FileName);
        
  bool bSearch = true;
  while(bSearch) {
    if(FindNextFile(hFind,&FindFileData)) {
      if (!(strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")))
        continue;
      strcat(FileName,FindFileData.cFileName);
      if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        if(!deleteFolder(FileName)) { 
            FindClose(hFind); 
            return false;
        }
        RemoveDirectory(FileName);
        strcpy(FileName,DirPath);
      }
      else {
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
          _chmod(FileName, _S_IWRITE);
        if(!DeleteFile(FileName)) {
          FindClose(hFind); 
          return FALSE; 
        }                 
        strcpy(FileName,DirPath);
      }
    }
    else {
      if(GetLastError() == ERROR_NO_MORE_FILES)
      bSearch = false;
      else {
        FindClose(hFind); 
        return false;
      }
    }
  }
  FindClose(hFind);
  if (RemoveDirectory(sPath) == 0)
    return false;
  return true;
}

bool copyFolder(string source, string dest)
{
  SHFILEOPSTRUCTA sf;
  
  size_t pos = 0;
  string search = "/";
  string replace = "\\";
  while ((pos = source.find(search, pos)) != std::string::npos) {
    source.replace(pos, search.length(), replace);
    pos += replace.length();
  }
  pos = 0;
  while ((pos = dest.find(search, pos)) != std::string::npos) {
    dest.replace(pos, search.length(), replace);
    pos += replace.length();
  }

  source += '\0';
  source += '\0';
  dest += '\0';
  dest += '\0';

  sf.pFrom = source.c_str();
  sf.pTo = dest.c_str();
  sf.wFunc = FO_COPY;
  sf.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT;

  if (SHFileOperationA(&sf) == 0)
    return true;
  return false;
}

bool moveFolder(string source, string dest)
{
  SHFILEOPSTRUCTA sf;
  
  size_t pos = 0;
  string search = "/";
  string replace = "\\";
  while ((pos = source.find(search, pos)) != std::string::npos) {
    source.replace(pos, search.length(), replace);
    pos += replace.length();
  }
  pos = 0;
  while ((pos = dest.find(search, pos)) != std::string::npos) {
    dest.replace(pos, search.length(), replace);
    pos += replace.length();
  }

  source += '\0';
  source += '\0';
  dest += '\0';
  dest += '\0';

  sf.pFrom = source.c_str();
  sf.pTo = dest.c_str();
  sf.wFunc = FO_MOVE;
  sf.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT;

  if (SHFileOperationA(&sf) == 0)
    return true;
  return false;
}

bool renameFolder(string source, string dest)
{
  return moveFolder(source, dest);
}


void readDirectory(string rootstr, vector<string>& ret)
{
  readDirectory(rootstr, "", ret);
}
void readDirectory(string rootstr, string ext, vector<string>& ret)
{
  boost::filesystem::path root (rootstr);
  if (!boost::filesystem::exists(root))
    return;
  if (boost::filesystem::is_directory(root))
  {
    boost::filesystem::directory_iterator it(root);
    boost::filesystem::directory_iterator endit;
    while(it != endit)
    {
      if (ext > "") {
        if (boost::filesystem::is_regular_file(*it) && it->path().extension() == ext)
        {
          ret.push_back(it->path().filename().string());
          ret[ret.size() - 1] = ret[ret.size() - 1].substr(0, ret[ret.size() - 1].length() - ext.length());
        }
      }
      else {
        if (boost::filesystem::is_directory(it->path()))
          ret.push_back(it->path().filename().string() + "/");
        else
          ret.push_back(it->path().filename().string());
      }
      ++it;
    }
  }
  return;
}

void readDirectoryRec(string rootstr, vector<string>& ret)
{
  readDirectoryRec(rootstr, "", ret, false);
}
void readDirectoryRec(string rootstr, vector<string>& ret, bool display)
{
  readDirectoryRec(rootstr, "", ret, display);
}
void readDirectoryRec(string rootstr, string ext, vector<string>& ret)
{
  readDirectoryRec(rootstr, ext, ret, "", false);
}
void readDirectoryRec(string rootstr, string ext, vector<string>& ret, bool display)
{
  readDirectoryRec(rootstr, ext, ret, "", display);
}
void readDirectoryRec(string rootstr, string ext, vector<string>& ret, string prefix, bool display)
{
  boost::filesystem::path root (rootstr);
  if (!boost::filesystem::exists(root))
    return;
  if (boost::filesystem::is_directory(root))
  {
    boost::filesystem::directory_iterator it(root);
    boost::filesystem::directory_iterator endit;
    while(it != endit)
    {
      if (ext > "") {
        if (boost::filesystem::is_regular_file(*it) && it->path().extension() == ext)
        {
          ret.push_back(prefix + it->path().filename().string());
          ret[ret.size() - 1] = ret[ret.size() - 1].substr(0, ret[ret.size() - 1].length() - ext.length());
        }
      }
      else {
        if (boost::filesystem::is_directory(it->path()))
          ret.push_back(prefix + it->path().filename().string() + "/");
        else
          ret.push_back(prefix + it->path().filename().string());
      }
      if (boost::filesystem::is_directory(it->path())) {
        if (display)
          readDirectoryRec(rootstr + it->path().filename().string() + "/", ext, ret, prefix + "  ", display);
        else
          readDirectoryRec(rootstr + it->path().filename().string() + "/", ext, ret, prefix + it->path().filename().string() + "/", display);
      }
      ++it;
    }
  }
  return;
}







//User

User::User()
{
  User::username = "";
  User::id = "";
  User::admin = false;
  User::password = "";
  User::loc = "data/user/" + User::username + "/";
}

User::~User()
{
}

string User::getUsername() const
{
  return User::username;
}

string User::getId() const
{
  return User::id;
}

bool User::getAdmin() const
{
  return User::admin;
}

string User::getLoc() const
{
  return User::loc;
}

void User::setUsername(string username)
{
  User::username = username;
}

void User::setPassword(string password)
{
  User::password = password;
}

void User::setLoc(string loc)
{
  User::loc = loc;
}

bool User::login_simple()
{
  bool authenticated = false;
  string userStr = "";
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  cls(h_);
  cout << "Log-in" << endl << endl;
  cout << "Username: ";
  getline(cin, userStr);
  if (userStr > "") {
    User::username = userStr;
    User::loc = loc_ + "data/user/" + User::username + "/";

    bool isForeground = false;
    string tmpStr = "";
    int in;
    GUITHREADINFO gti;
    gti.cbSize = sizeof(GUITHREADINFO);
    while (INKEY()) {
      DELAY_LOOP;
    }
    do {
      DELAY_LOOP;
      COORD coordScreen = { 0, 3 };
      SetConsoleCursorPosition(h_, coordScreen);
      cout << "Password: ";
      for (size_t i = 0; i < tmpStr.length(); i++)
        cout << "*";
      GetGUIThreadInfo(NULL, &gti);
      if (gti.hwndFocus == hWnd_)
        isForeground = true;
      else
        isForeground = false;
      if (isForeground) {
        if (in = INKEY()) {
          if (in == 8) {
            if (tmpStr.length() > 0) {
              coordScreen = { (9 + (int)tmpStr.length()), 3 };
              SetConsoleCursorPosition(h_, coordScreen);
              cout << " ";
              SetConsoleCursorPosition(h_, coordScreen);
              tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
            }
          }
          else if (in != 13) {
            if (tmpStr.length() < MAX_PASSWORD_LEN)
              tmpStr += str((char)in);
          }
        }
      }
    } while (in != 13);
    if (tmpStr > "") {
      User::setPassword(tmpStr);
      authenticated = User::authenticateUser();
    }
    cin.clear();
    cin.ignore(INT_MAX, '\n');

    if (authenticated) {
      cls(h_);
      cout << "Logged in as " << User::getUsername() << "!" << endl;
      system("PAUSE");
    }
    else {
      cls(h_);
      cout << "The log-in failed!" << endl;
      system("PAUSE");
    }
  }
  return authenticated;
}

void User::loadUser_simple()
{
  fstream fs;
  stringstream ss;

  string tmpStr = "";

  fs.open(User::getLoc() + "id", fstream::in);
  getline(fs, User::id);
  fs.close();

  User::loadUserFileS("info", ss);
  Contact info(ss);
  info.~Contact();
  getbool(ss, User::admin);
  ss.clear();
}

bool User::authenticateUser()
{
  return verifyPassword(User::password, User::getLoc() + "pass");
}

void User::loadUserFileS(string file, vector<string>& data, int& dataLen)
{
  file = User::getLoc() + file;
  
  if (!fileExists(file) || fileSize(file) < 1)
    return;

  loadEncFileS(file, User::password, data, dataLen);
}
void User::loadUserFileS(string file, ostream& ss)
{
  vector<string> data;
  int dataLen = 0;
  User::loadUserFileS(file, data, dataLen);

  for (int i = 0; i < dataLen; i++)
    ss << data[i] << endl;
}

bool simpleLogin(User& user)
{
  bool succeeded = user.login_simple();

  if (succeeded)
    user.loadUser_simple();

  user.setPassword("");
  return succeeded;
}






//Keyboard

int INKEY()
{
  bool shift = (GetAsyncKeyState(VK_SHIFT)   < 0);
  bool ctrl = (GetAsyncKeyState(VK_CONTROL) < 0);
  bool alt = (GetAsyncKeyState(VK_MENU)    < 0);

  if (ctrl) {
    for (int i = 0x41; i < 0x5B; i++) {
      if (GetAsyncKeyState(i) == KEY_ACTIVE)
        return i - 0x41 + 1;
    }
  }
  if (GetAsyncKeyState(0x08) == KEY_ACTIVE)
    return 8;
  if (GetAsyncKeyState(0x09) == KEY_ACTIVE)
    return 9;
  if (GetAsyncKeyState(0x0D) == KEY_ACTIVE)
    return 13;

  if (GetAsyncKeyState(0x1B) == KEY_ACTIVE)
    return 27;
  if (GetAsyncKeyState(0x20) == KEY_ACTIVE)
    return 32;

  if (shift) {
    if (GetAsyncKeyState(0xC0) == KEY_ACTIVE)
      return 126;

    if (GetAsyncKeyState(0x31) == KEY_ACTIVE)
      return 33;
    if (GetAsyncKeyState(0x32) == KEY_ACTIVE)
      return 64;
    if (GetAsyncKeyState(0x33) == KEY_ACTIVE)
      return 35;
    if (GetAsyncKeyState(0x34) == KEY_ACTIVE)
      return 36;
    if (GetAsyncKeyState(0x35) == KEY_ACTIVE)
      return 37;
    if (GetAsyncKeyState(0x36) == KEY_ACTIVE)
      return 94;
    if (GetAsyncKeyState(0x37) == KEY_ACTIVE)
      return 38;
    if (GetAsyncKeyState(0x38) == KEY_ACTIVE)
      return 42;
    if (GetAsyncKeyState(0x39) == KEY_ACTIVE)
      return 40;
    if (GetAsyncKeyState(0x30) == KEY_ACTIVE)
      return 41;
    if (GetAsyncKeyState(0xBD) == KEY_ACTIVE)
      return 95;
    if (GetAsyncKeyState(0xBB) == KEY_ACTIVE)
      return 43;

    if (GetAsyncKeyState(0xBA) == KEY_ACTIVE)
      return 58;
    if (GetAsyncKeyState(0xBC) == KEY_ACTIVE)
      return 60;
    if (GetAsyncKeyState(0xBE) == KEY_ACTIVE)
      return 62;
    if (GetAsyncKeyState(0xBF) == KEY_ACTIVE)
      return 63;

    if (GetAsyncKeyState(0xDB) == KEY_ACTIVE)
      return 123;
    if (GetAsyncKeyState(0xDC) == KEY_ACTIVE)
      return 124;
    if (GetAsyncKeyState(0xDD) == KEY_ACTIVE)
      return 125;

    if (GetAsyncKeyState(0xDE) == KEY_ACTIVE)
      return 34;
  }
  if (GetAsyncKeyState(0xDE) == KEY_ACTIVE)
    return 39;

  if (GetAsyncKeyState(0xBC) == KEY_ACTIVE)
    return 44;
  if (GetAsyncKeyState(0xBD) == KEY_ACTIVE)
    return 45;
  if (GetAsyncKeyState(0xBE) == KEY_ACTIVE)
    return 46;
  if (GetAsyncKeyState(0xBF) == KEY_ACTIVE)
    return 47;

  if (GetAsyncKeyState(0x6A) == KEY_ACTIVE)
    return 42;
  if (GetAsyncKeyState(0x6B) == KEY_ACTIVE)
    return 43;
  if (GetAsyncKeyState(0x6D) == KEY_ACTIVE)
    return 45;
  if (GetAsyncKeyState(0x6E) == KEY_ACTIVE)
    return 46;
  if (GetAsyncKeyState(0x6F) == KEY_ACTIVE)
    return 47;

  if (GetAsyncKeyState(0xC0) == KEY_ACTIVE)
    return 96;

  if (shift == false) {
    if ((GetAsyncKeyState(0x30) == KEY_ACTIVE) || (GetAsyncKeyState(0x60) == KEY_ACTIVE))
      return 48;
    if ((GetAsyncKeyState(0x31) == KEY_ACTIVE) || (GetAsyncKeyState(0x61) == KEY_ACTIVE))
      return 49;
    if ((GetAsyncKeyState(0x32) == KEY_ACTIVE) || (GetAsyncKeyState(0x62) == KEY_ACTIVE))
      return 50;
    if ((GetAsyncKeyState(0x33) == KEY_ACTIVE) || (GetAsyncKeyState(0x63) == KEY_ACTIVE))
      return 51;
    if ((GetAsyncKeyState(0x34) == KEY_ACTIVE) || (GetAsyncKeyState(0x64) == KEY_ACTIVE))
      return 52;
    if ((GetAsyncKeyState(0x35) == KEY_ACTIVE) || (GetAsyncKeyState(0x65) == KEY_ACTIVE))
      return 53;
    if ((GetAsyncKeyState(0x36) == KEY_ACTIVE) || (GetAsyncKeyState(0x66) == KEY_ACTIVE))
      return 54;
    if ((GetAsyncKeyState(0x37) == KEY_ACTIVE) || (GetAsyncKeyState(0x67) == KEY_ACTIVE))
      return 55;
    if ((GetAsyncKeyState(0x38) == KEY_ACTIVE) || (GetAsyncKeyState(0x68) == KEY_ACTIVE))
      return 56;
    if ((GetAsyncKeyState(0x39) == KEY_ACTIVE) || (GetAsyncKeyState(0x69) == KEY_ACTIVE))
      return 57;
  }

  if (GetAsyncKeyState(0xBA) == KEY_ACTIVE)
    return 59;
  if (GetAsyncKeyState(0xBB) == KEY_ACTIVE)
    return 61;

  for (int i = 0x41; i < 0x5B; i++) {
    if (GetAsyncKeyState(i) == KEY_ACTIVE) {
      if (shift)
        return 65 + i - 0x41;
      else
        return 97 + i - 0x41;
    }
  }

  if (GetAsyncKeyState(0xDB) == KEY_ACTIVE)
    return 91;
  if (GetAsyncKeyState(0xDC) == KEY_ACTIVE)
    return 92;
  if (GetAsyncKeyState(0xDD) == KEY_ACTIVE)
    return 93;

  return 0;
}






//Crypto
string passHasher(string password, string salt, int iterations, byte purpose)
{
  if (password.length() < 1 || iterations < 1)
    return "";

  CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA512> pbkdf;

  CryptoPP::SecByteBlock derived(32);

  pbkdf.DeriveKey(derived.data(), derived.size(),
    purpose,
    (unsigned char *)password.data(), password.size(),
    (unsigned char *)salt.data(), salt.size(),
    iterations);

  CryptoPP::HexEncoder encoder;
  encoder.Put(derived.data(), derived.size());
  encoder.MessageEnd();

  string out;
  unsigned int size;

  size = (unsigned int)encoder.MaxRetrievable();
  out.resize(size);
  encoder.Get((unsigned char *)out.data(), out.size());

  return out;
}
bool verifyPassword(string password, string file)
{
  if (password.length() == 0 || !fileExists(file))
    return false;

  string truehash;
  string salt;
  int    iterations;

  fstream fs;
  fs.open(file.c_str(), fstream::in);
  getline(fs, truehash);
  getline(fs, salt);
  getint(fs, iterations);
  fs.close();

  string testhash = passHasher(password, salt, iterations, NULL);

  if (testhash == truehash)
    return true;
  return false;
}
void loadEncFileS(string file, string password, vector<string>& data, int& dataLen)
{
  if (!fileExists(file))
    return;

  password = password.substr(0, CryptoPP::AES::MAX_KEYLENGTH - 1);

  byte key[CryptoPP::AES::MAX_KEYLENGTH];
  byte iv[CryptoPP::AES::BLOCKSIZE];
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
  memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
  memcpy(key, password.c_str(), password.size());

  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
  decryptor.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, iv, CryptoPP::AES::BLOCKSIZE);

  string temp;

  CryptoPP::FileSource file_source(file.c_str(), true,
    new CryptoPP::StreamTransformationFilter(decryptor,
    new CryptoPP::StringSink(temp)));

  istringstream iss(temp);
  string line;

  data.clear();
  dataLen = 0;

  while (getline(iss, line)) {
    dataLen++;
    data.push_back(line);
  }
}






//Contact

Contact::Contact()
{
}
Contact::Contact(istream& is)
{
  readContactFromStream(is);
}

Contact::Name Contact::newName(istream& is)
{
  Contact::Name temp;
  getline(is, temp.prefix);
  getline(is, temp.first);
  getline(is, temp.middle);
  getline(is, temp.middleInitial);
  getline(is, temp.last);
  getline(is, temp.suffix);
  getline(is, temp.nickName);
  return temp;
}
Contact::DLA Contact::newDla(istream& is)
{
  Contact::DLA temp;
  getline(is, temp.name);
  getint (is, temp.gender);
  return temp;
}
Contact::Phone Contact::newPhone(istream& is)
{
  Contact::Phone temp;
  getint(is, temp.type);
  getint(is, temp.country);
  getint(is, temp.areaCode);
  getint(is, temp.number);
  getint(is, temp.ext);
  return temp;
}
Contact::Email Contact::newEmail(istream& is)
{
  Contact::Email temp;
  getint (is, temp.type);
  getline(is, temp.address);
  return temp;
}
Contact::Address Contact::newAddress(istream& is)
{
  Contact::Address temp;
  getint (is, temp.type);
  getline(is, temp.street);
  getline(is, temp.city);
  getline(is, temp.state);
  getint (is, temp.zip);
  getline(is, temp.appt);
  getint (is, temp.poBox);
  return temp;
}
Contact::Account Contact::newAccount(istream& is)
{
  Contact::Account temp;
  getline(is, temp.account);
  getline(is, temp.username);
  getline(is, temp.address);
  return temp;
}

Contact::~Contact()
{
}

void Contact::readContactFromStream(istream& is)
{
  Contact::name = Contact::newName(is);
  Contact::dla = Contact::newDla(is);
  getline(is, Contact::username);
  getline(is, Contact::id);
  getint(is, Contact::gender);
  getline(is, Contact::birthday);

  int phoneCount = 0;
  int emailCount = 0;
  int addressCount = 0;
  int accountCount = 0;

  getint(is, phoneCount);
  while (phoneCount != 0) {
    Contact::phone.push_back(Contact::newPhone(is));
    getint(is, phoneCount);
  }

  getint(is, emailCount);
  while (emailCount != 0) {
    Contact::email.push_back(Contact::newEmail(is));
    getint(is, emailCount);
  }

  getint(is, addressCount);
  while (addressCount != 0) {
    Contact::address.push_back(Contact::newAddress(is));
    getint(is, addressCount);
  }

  getint(is, accountCount);
  while (accountCount != 0) {
    Contact::account.push_back(Contact::newAccount(is));
    getint(is, accountCount);
  }
}
