////////////////////////////////////////////////////////////////////////////////
//File:   main.cpp
//Dir:    source/
//Date:   2014-03-19
//Author: Zachary Gill
//Main Program
////////////////////////////////////////////////////////////////////////////////


//constants
#include "resource/const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>


//includes
#include "header/main.h"


//namespaces
using namespace std;


//functions
int main()
{
  load();
  init();


  //loadModules();

  //for (size_t i = 0; i < modules_.size(); i ++) {
  //  modules_[i].dump();
  //  cout << endl;
  //}

  //system("PAUSE");
  //return 0;


  string request = "";
  string response = "";

  //open log
  fstream log;
  log.open("data/log.log", fstream::app);

  //change color to green
  SetConsoleTextAttribute(h_, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  
  cout << "JARVIS" << endl << endl;

  //main loop
  do {
    //get input
    request = "";
    cout << "<<";
    getline (cin, request);
    log << "<<" << request << endl;


    if (request == "exit")
      CORE::terminate();
    if (request == "time")
      response = TIME::time();
    if (request == "date")
      response = TIME::date();

    //analyze input
    //parse(request);
    //response = respond(request);

    //produce output
    if (response > "") {
      cout << ">>" << response << endl;
      log << ">>" << response << endl;
      response = "";
    }
  } while (terminate_ == false);

  //backup settings
  //clean up

  return 0;
}

void load()
{
  loadResources();
  //loadUser();
  //loadModules();
}

void init()
{
  //turn modules
}