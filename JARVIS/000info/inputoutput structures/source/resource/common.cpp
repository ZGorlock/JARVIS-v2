////////////////////////////////////////////////////////////////////////////////
//File:   common.cpp
//Dir:    source/resource/
//Date:   2014-03-30
//Author: Zachary Gill
//Common functions Implementation
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <Windows.h>

#include <boost/algorithm/string.hpp>


//includes
#include "common.h"


//namespaces
using namespace std;


//functions
bool fileExists(string name)
{
  fstream file(name.c_str());
  if(!file)
    return false;
  else
    return true;
}


bool getint(fstream& fs, int& outVar)
{
  if (fs.is_open()) {
    string tmp;
    fs >> outVar;
    getline(fs, tmp);
    return true;
  }
  else
    return false;
}

bool getdouble(fstream& fs, double& outVar)
{
  if (fs.is_open()) {
    string tmp;
    fs >> outVar;
    getline(fs, tmp);
    return true;
  }
  else
    return false;
}

bool getbool (fstream& fs, bool& outVar)
{
  if (fs.is_open()) {
    string tmp;
    fs >> outVar;
    getline(fs, tmp);
    return true;
  }
  else
    return false;
}


void loadFileS(string file, vector<string>& data)
{
  string line = "";
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open())
    while (getline(fs, line)) {
      data.push_back(line);
    }
  fs.close();
}

void loadFileI(string file, vector<int>& data)
{
  int line = 0;
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open())
    while (getint(fs, line)) {
      data.push_back(line);
    }
  fs.close();
}

void loadFileD(string file, vector<double>& data)
{
  double line = 0.0;
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open())
    while (getdouble(fs, line)) {
      data.push_back(line);
    }
  fs.close();
}

void cls( HANDLE hConsole )
{
  COORD coordScreen = { 0, 0 };
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi; 
  DWORD dwConSize;

  GetConsoleScreenBufferInfo( hConsole, &csbi );
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
  GetConsoleScreenBufferInfo( hConsole, &csbi );
  FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
  SetConsoleCursorPosition( hConsole, coordScreen );
}

string uCase (string str)
{
  boost::to_upper(str);
  return str;
}

string lCase (string str)
{
  boost::to_lower(str);
  return str;
}


int val(string str)
{
  int i;
  istringstream(str) >> i;
  return i;
}

double vald(string str)
{
  double d;
  istringstream(str) >> d;
  return d;
}

string str(int i)
{
  string str;
  ostringstream oss;
  oss << i;
  str = oss.str();
  return str;
}

string str(double d)
{
  string str;
  ostringstream oss;
  oss << d;
  str = oss.str();
  return str;
}