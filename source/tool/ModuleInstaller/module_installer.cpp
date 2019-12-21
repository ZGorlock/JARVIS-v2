////////////////////////////////////////////////////////////////////////////////
//File:   module_installer.cpp
//Dir:    *
//Date:   2015-01-04
//Author: Zachary Gill
//Implementation of Module Installer
////////////////////////////////////////////////////////////////////////////////


//standard includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <commdlg.h>


//includes
#include "module_installer.h"


//namespaces
using namespace std;


//functions

int main()
{
  //load location of DLA
  string loc = "";

  if (!fileExists("C:/ProgramData/DLA/loc")) //if loc file doesnt exist
    return 1;

  fstream fs;
  fs.open("C:/ProgramData/DLA/loc", fstream::in);
  getline(fs, loc); //get loc
  fs.close();

  cout << "DLA loc: " << loc << endl;
  


  //get .dmi file
  cout << "Getting .dmi File from User..." << endl;

  string dmiFile;
  
  OPENFILENAMEA ofn = { 0 }; //create OPENFILENAMEA structure
  char Buffer[512];
  fill(Buffer, Buffer + 512, '\0'); //initialize buffer

  ofn.lStructSize = sizeof(OPENFILENAMEA);
  ofn.hwndOwner = GetConsoleWindow(); //current window
  ofn.lpstrFilter = "DLA Module Installer (.dmi)\0*.dmi\0"; //only .dmi files
  ofn.lpstrCustomFilter = NULL;
  ofn.nFilterIndex = 0;
  ofn.lpstrFile = Buffer; //output string
  ofn.nMaxFile = 512;
  ofn.lpstrFileTitle = NULL;
  ofn.lpstrInitialDir = NULL;
  ofn.lpstrTitle = NULL;
  ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (!GetOpenFileNameA(&ofn)) //if failed to get file name from user
    return 1;

  dmiFile = ofn.lpstrFile; //set filename
  cout << "File: " << dmiFile << endl;
  cout << "Extracting..." << endl;



  //extract .dmi file
  vector<string> files;
  vector<string> variables;

  fstream release;
  release.open(dmiFile, fstream::binary | fstream::in); //open dmi file as a binary for input

  int size;
  release.read((char*)&size, sizeof(int)); //get size of module name

  char *name;
  name = new char[size];
  release.read(name, size); //get module name
  
  //make temporary folder
  string folder = loc + "/temp/" + lCase(name);
  makeFolder(folder);

  //set files vector
  files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(name) + ".module");
  files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(name) + ".functions");
  files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(name) + "_M.cpp");
  files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(name) + "_M.h");
  files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(name) + ".settings");
  
  int start, end;
  for (int i = 0; i < 5; i++) { //includes 5 files
    release.read((char*)&start, sizeof(int)); //get start index of file i
    release.read((char*)&end, sizeof(int)); //get end index of file i
    readOutFile(release, start, end, files[i]); //read out file
    cout << "Extracted: " << files[i] << endl;
  }

  int count;
  release.read((char*)&count, sizeof(int)); //get number of variables

  for (int i = 0; i < count; i++) { //for each variable
    int varSize;
    release.read((char*)&varSize, sizeof(int)); //get size of variable name
    char *varName;
    varName = new char[varSize];
    release.read(varName, varSize); //get variable name
    variables.push_back(varName); //add to variables vector
  }
  
  for (int i = 0; i < count; i++) { //for each variable
    //set files vector
    files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(variables[i]) + "_V.cpp");
    files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(variables[i]) + "_V.h");
    files.push_back(loc + "temp/" + lCase(name) + "/" + lCase(variables[i]) + ".variable");
    
    int filesStart = 5 + (3 * i);
    for (int j = filesStart; j < filesStart + 3; j++) { //includes 3 files
      release.read((char*)&start, sizeof(int)); //get start index of file j of variable i
      release.read((char*)&end, sizeof(int)); //get end index of file j of variable i
      readOutFile(release, start, end, files[j]); //read out file
      cout << "Extracted: " << files[j] << endl;
    }
  }

  release.close(); //close file

  cout << "Extraction Complete!" << endl;
  cout << "Moving Files..." << endl;


  
  //move files
  string source;
  string dest;

  //.module file
  source = loc + "temp/" + lCase(name) + "/" + lCase(name) + ".module";
  dest   = loc + "data/module/" + lCase(name) + ".module";
  if (!fileExists(dest)) {
    copyFile(source, dest);
    cout << "Moved: " << lCase(name) << ".module" << " to " << "DLA/data/module/" << endl;
  }
  else {
    cout << "Error: " << lCase(name) << ".module" << " already exists" << endl;
  }

  //.functions file
  source = loc + "temp/" + lCase(name) + "/" + lCase(name) + ".functions";
  dest   = loc + "data/module/" + lCase(name) + ".functions";
  if (!fileExists(dest)) {
    copyFile(source, dest);
    cout << "Moved: " << lCase(name) << ".functions" << " to " << "DLA/data/module/" << endl;
  }
  else {
    cout << "Error: " << lCase(name) << ".functions" << " already exists" << endl;
  }

  //.cpp file
  source = loc + "temp/" + lCase(name) + "/" + lCase(name) + "_M.cpp";
  dest   = loc + "source/module/" + lCase(name) + "_M.cpp";
  if (!fileExists(dest)) {
    copyFile(source, dest);
    cout << "Moved: " << lCase(name) << "_M.cpp" << " to " << "DLA/source/module/" << endl;
  }
  else {
    cout << "Error: " << lCase(name) << "_M.cpp" << " already exists" << endl;
  }
  
  //.h file
  source = loc + "temp/" + lCase(name) + "/" + lCase(name) + "_M.h";
  dest   = loc + "source/module/" + lCase(name) + "_M.h";
  if (!fileExists(dest)) {
    copyFile(source, dest);
    cout << "Moved: " << lCase(name) << "_M.h" << " to " << "DLA/source/module/" << endl;
  }
  else {
    cout << "Error: " << lCase(name) << "_M.h" << " already exists" << endl;
  }
  
  //variables
  for (size_t i = 0; i < variables.size(); i++) {
    cout << "Moving Variables: " << lCase(variables[i]) << endl;

    //.cpp file
    source = loc + "temp/" + lCase(name) + "/" + lCase(variables[i]) + "_V.cpp";
    dest = loc + "source/variable/" + lCase(variables[i]) + "_V.cpp";
    if (!fileExists(dest)) {
      copyFile(source, dest);
      cout << "  Moved: " << lCase(variables[i]) << "_V.cpp" << " to " << "DLA/source/variable/" << endl;
    }
    else {
      cout << "  Error: " << lCase(variables[i]) << "_V.cpp" << " already exists" << endl;
    }

    //.h file
    source = loc + "temp/" + lCase(name) + "/" + lCase(variables[i]) + "_V.h";
    dest = loc + "source/variable/" + lCase(variables[i]) + "_V.h";
    if (!fileExists(dest)) {
      copyFile(source, dest);
      cout << "  Moved: " << lCase(variables[i]) << "_V.h" << " to " << "DLA/source/variable/" << endl;
    }
    else {
      cout << "  Error: " << lCase(variables[i]) << "_V.h" << " already exists" << endl;
    }
    
    //.variable file
    source = loc + "temp/" + lCase(name) + "/" + lCase(variables[i]) + ".variable";
    dest = loc + "data/variable/" + lCase(variables[i]) + ".variable";
    if (!fileExists(dest)) {
      copyFile(source, dest);
      cout << "  Moved: " << lCase(variables[i]) << ".variable" << " to " << "DLA/data/variable/" << endl;
    }
    else {
      cout << "  Error: " << lCase(variables[i]) << ".variable" << " already exists" << endl;
    }

    cout << "Moved: " << "Variable: " << lCase(variables[i]) << endl;
  }

  cout << "Moved Files!" << endl;
  cout << "Removing Temporary Files..." << endl;



  //remove temporary files
  deleteFolder(folder);
  cout << "Removed Temporary Files!" << endl;
  cout << "Updating Lists..." << endl;



  //update lists
  fstream updList;
  vector<string> data;
  string newData;
  bool has;

  //data/modules
  cout << "Updating: DLA/data/modules" << endl;
  data.clear();
  loadFileS(loc + "data/modules", data);
  newData = lCase(name);
  has = false;
  for (size_t i = 0; i < data.size(); i++) {
    if (uCase(data[i]) == uCase(newData))
      has = true;
  }
  if (!has)
    data.push_back(newData);
  alphabetize(data);
  updList.open(loc + "data/modules", fstream::out);
  for (size_t i = 0; i < data.size(); i++)
    updList << data[i] << endl;
  updList.close();
  
  //data/module/ptrs
  cout << "Updating: DLA/data/module/ptrs" << endl;
  data.clear();
  loadFileS(loc + "data/module/ptrs", data);
  newData = "#include \"" + lCase(name) + ".functions\"";
  has = false;
  for (size_t i = 0; i < data.size(); i++) {
    if (uCase(data[i]) == uCase(newData))
      has = true;
  }
  if (!has)
    data.push_back(newData);
  alphabetize(data);
  updList.open(loc + "data/module/ptrs", fstream::out);
  for (size_t i = 0; i < data.size(); i++)
    updList << data[i] << endl;
  updList.close();
  
  //data/variable/ptrs
  cout << "Updating: DLA/data/variable/ptrs" << endl;
  data.clear();
  loadFileS(loc + "data/variable/ptrs", data);
  for (size_t v = 0; v < variables.size(); v++) {
    newData = "#include \"" + lCase(variables[v]) + ".variable\"";
    has = false;
    for (size_t i = 0; i < data.size(); i++) {
      if (uCase(data[i]) == uCase(newData))
        has = true;
    }
    if (!has)
      data.push_back(newData);
  }
  alphabetize(data);
  updList.open(loc + "data/variable/ptrs", fstream::out);
  for (size_t i = 0; i < data.size(); i++)
    updList << data[i] << endl;
  updList.close();
  
  //source/resource/variables
  cout << "Updating: DLA/source/resource/variables" << endl;
  data.clear();
  loadFileS(loc + "source/resource/variables", data);
  for (size_t v = 0; v < variables.size(); v++) {
    newData = "#include \"../variable/" + lCase(variables[v]) + "_V.h\"";
    has = false;
    for (size_t i = 0; i < data.size(); i++) {
      if (uCase(data[i]) == uCase(newData))
        has = true;
    }
    if (!has)
      data.push_back(newData);
  }
  alphabetize(data);
  updList.open(loc + "source/resource/variables", fstream::out);
  for (size_t i = 0; i < data.size(); i++)
    updList << data[i] << endl;
  updList.close();
  
  //source/resource/modules
  cout << "Updating: DLA/source/resource/modules" << endl;
  data.clear();
  loadFileS(loc + "source/resource/modules", data);
  newData = "#include \"../module/" + lCase(name) + "_M.h\"";
  has = false;
  for (size_t i = 0; i < data.size(); i++) {
    if (uCase(data[i]) == uCase(newData))
      has = true;
  }
  if (!has)
    data.push_back(newData);
  alphabetize(data);
  updList.open(loc + "source/resource/modules", fstream::out);
  for (size_t i = 0; i < data.size(); i++)
    updList << data[i] << endl;
  updList.close();

  cout << "Updated Lists!" << endl;
  cout << "Installation Complete!" << endl;


  system("PAUSE");
  return 0;
}

void readOutFile(fstream& bin, int start, int end, string file)
{
  int saveIndex = (int)bin.tellg(); //save index
  bin.seekg(start); //move to start of file in bin

  char *get;
  int size = end - start; //size of file
  get = new char[size];

  bin.read(get, size); //read file out of bin

  string got;
  got.assign(get, get + size - 2); //remove trailing chars

  fstream fs;
  fs.open(file, fstream::out); //open file for output
  fs << got; //put file from bin in file
  fs.close();

  bin.seekg(saveIndex); //return to previous location in bin
}