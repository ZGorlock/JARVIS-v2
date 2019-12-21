////////////////////////////////////////////////////////////////////////////////
//File:   common.cpp
//Dir:    source/resource/
//Date:   2015-01-04
//Author: Zachary Gill
//Common functions Implementation
////////////////////////////////////////////////////////////////////////////////

#define  NTDDI_VERSION NTDDI_WIN7
#define  WINVER        _WIN32_WINNT_WIN7
#define _WIN32_WINNT   _WIN32_WINNT_WIN7
#define _WIN32_IE      _WIN32_IE_IE50

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include <string>
#include <array>
#include <vector>
#include <cctype>
#include <algorithm>
#include <time.h>
#include <rpc.h>
#include <cstdio>

#include <boost/algorithm/string.hpp>
#include <boost/thread/thread.hpp>


//includes
#include "common.h"
#include "Double-Metaphone/double_metaphone.h"


//libraries
#pragma comment(lib, "Rpcrt4.lib")


//namespaces
using namespace std;


//functions

bool getint(istream& is, int& outVar)
{
  return getint(is, outVar, true);
}
bool getint(istream& is, int& outVar, bool newline)
{
  bool ok = false;
  string tmp;
  if (is >> outVar) {
    ok = true;
    if (newline)
      getline(is, tmp);
  }
  return ok;
}

bool getdouble(istream& is, double& outVar)
{
  return getdouble(is, outVar, true);
}
bool getdouble(istream& is, double& outVar, bool newline)
{
  bool ok = false;
  string tmp;
  if (is >> outVar) {
    ok = true;
    if (newline)
      getline(is, tmp);
  }
  return ok;
}

bool getbool(istream& is, bool& outVar)
{
  return getbool(is, outVar, true);
}
bool getbool(istream& is, bool& outVar, bool newline)
{
  bool ok = false;
  string tmp;
  if (is >> outVar) {
    ok = true;
    if (newline)
      getline(is, tmp);
  }
  return ok;
}


void loadFileS(string file, vector<string>& data)
{
  string line = "";
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open()) {
    while (getline(fs, line)) {
      data.push_back(line);
    }
  }
  fs.close();
}

void loadFileI(string file, vector<int>& data)
{
  int line = 0;
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open()) {
    while (getint(fs, line)) {
      data.push_back(line);
    }
  }
  fs.close();
}

void loadFileD(string file, vector<double>& data)
{
  double line = 0.0;
  fstream fs;

  fs.open(file.c_str(), fstream::in);
  if (fs.is_open()) {
    while (getdouble(fs, line)) {
      data.push_back(line);
    }
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


void printFileS(string file, vector<string>& data)
{
  fstream fs;

  fs.open(file.c_str(), fstream::out);
  if (fs.is_open()) {
    for (size_t i = 0; i < data.size(); i++)
      fs << data[i] << endl;
  }

  fs.close();
}

void printFileI(string file, vector<int>& data)
{
  fstream fs;

  fs.open(file.c_str(), fstream::out);
  if (fs.is_open()) {
    for (size_t i = 0; i < data.size(); i++)
      fs << data[i] << endl;
  }

  fs.close();
}

void printFileD(string file, vector<double>& data)
{
  fstream fs;
  
  fs.open(file.c_str(), fstream::out);
  if (fs.is_open()) {
    for (size_t i = 0; i < data.size(); i++)
      fs << data[i] << endl;
  }

  fs.close();
}


void cls(HANDLE hConsole)
{
  COORD coordScreen = { 0, 0 };
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


void clipboardOut(string s)
{
  HGLOBAL hText;
  char *pText;
  hText = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, 100);
  pText = (char*)GlobalLock(hText);
  strcpy(pText, s.c_str());
  GlobalUnlock(hText);

  OpenClipboard(NULL);
  EmptyClipboard();
  SetClipboardData(CF_TEXT, hText);
  CloseClipboard();
}

string clipboardIn()
{
  OpenClipboard(NULL);
  HANDLE foo = GetClipboardData(CF_TEXT);
  CloseClipboard();
  LPVOID lptstr = GlobalLock(foo);
  return (char *)lptstr;
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


string removeSpace(string text)
{
  for (size_t i = 0; i < text.length(); i++)
  if (text[i] == ' ')
    text.erase(i--, 1);
  return text;
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

void ReplaceStringInPlace(string& subject, const string& search, const string& replace)
{
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

wstring stringToWideString(const std::string& s)
{
  int len;
  int slength = (int)s.length() + 1;
  len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
  wchar_t* buf = new wchar_t[len];
  MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
  std::wstring r(buf);
  delete[] buf;
  return r;
}

bool str_is_0(string s)
{
  int    a = 0;
  double b = 0.0;
  for (size_t i = 0; i < s.length(); i++)
    a += (int)s.at(i);
  b = (double)a / s.length();
  if (b == 48.0)
    return true;
  return false;
}


string randString(int size)
{
  return randString(size, 1, 256);
}
string randString(int size, int lowerLimit, int upperLimit)
{
  RANDOMIZE_TIMER;
  string r = "";
  for (int i = 0; i < size; i++) {
    int n = rand() % (upperLimit - lowerLimit) + lowerLimit;
    r += str((char)n);
  }
  return r;
}


string uCase(string s)
{
  boost::to_upper(s);
  return s;
}

string lCase(string s)
{
  boost::to_lower(s);
  return s;
}


void trim(string& s)
{
  lTrim(s);
  rTrim(s);
}

void lTrim(string& s)
{
  string left = s.substr(0, 1);
  if (left == " " || left == "\0")
    s = s.substr(1);
}

void rTrim(string& s)
{
  string right = s.substr(s.length() - 1, 1);
  if (right == " " || right == "\0")
    s = s.substr(0, s.length() - 1);
}


int val(string s)
{
  int i = 0;
  size_t j;
  for (j = 0; j < s.length(); j++)
  if (s.at(j) != '0')
    break;
  s = s.substr(j);
  istringstream(s) >> i;
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
string str(char c)
{
  stringstream ss;
  string s;
  ss << c;
  ss >> s;
  return s;
}


string zeroset(string& s, int i)
{
  while (s.length() < (size_t)i)
    s = "0" + s;
  return s;
}


bool inRange(int n, int lb, int ub)
{
  return inRange((double)n, (double)lb, (double)ub);
}
bool inRange(int n, int lb, int ub, bool lbInc,  bool ubInc)
{
  return inRange((double)n, (double)lb, (double)ub, lbInc, ubInc);
}
bool inRange(double n, double lb, double ub)
{
  return inRange(n, lb, ub, true, true);
}
bool inRange(double n, double lb, double ub, bool lbInc, bool ubInc)
{
  bool isInRange = true;
  if (lbInc) {
    if (n < lb)
      isInRange = false;
  }
  else {
    if (n <= lb)
      isInRange = false;
  }
  if (ubInc) {
    if (n > ub)
      isInRange = false;
  }
  else {
    if (n >= ub)
      isInRange = false;
  }
  return isInRange;
}


double stringDistance(string x, string y)
{
  return stringDistance(x, y, false);
}
double stringDistance(string x, string y, bool typoCostSpecification)
{
  int cost, defaultCost;
  int leftCell, topCell, cornerCell;

  if (typoCostSpecification)
    defaultCost = 2;
  else
    defaultCost = 1;

  int m = x.length() + 1;
  int n = y.length() + 1;

  int *T = (int *)malloc(m * n * sizeof(int));
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++)
      *(T + i * n + j) = -1;
  }
  for (int i = 0; i < m; i++)
    *(T + i * n) = i;
  for (int j = 0; j < n; j++)
    *(T + j) = j;

  for (int i = 1; i < m; i++)
  {
    for (int j = 1; j < n; j++)
    {
      if (x[i - 1] == y[j - 1] || x[i - 1] == '*') {
        cost = 0;
      }
      else {
        if (typoCostSpecification) {
          if (keyboard_touch_['A' - toupper(x[i - 1])]['A' - toupper(y[j - 1])])
            cost = 1;
          else
            cost = 2;
        }
        else {
          cost = 1;
        }
      }
      leftCell = *(T + i * n + j - 1) + defaultCost; //deletion
      topCell = *(T + (i - 1) * n + j) + defaultCost; //insertion
      cornerCell = *(T + (i - 1) * n + j - 1) + cost;
      *(T + i * n + j) = min(min(leftCell, topCell), cornerCell);
      if (i > 1 && j > 1 && x[i - 1] == y[j - 2] && x[i - 2] == y[j - 1]) //transposition
        *(T + i * n + j) = min(*(T + i * n + j), *(T + (i - 2) * n + (j - 2)) + defaultCost);
    }
  }

  cost = *(T + m * n - 1);
  free(T);

  if (typoCostSpecification)
    return cost / 2.0;
  return cost;
}


double compareString(string s1, string s2)
{
  return 1.0 - (stringDistance(s1, s2, true) / max(s1.length(), s2.length()));
}

double compareStringPhonetic(string s1, string s2)
{
  s1 = DoubleMetaphone(s1);
  s2 = DoubleMetaphone(s2);
  return compareString(s1, s2);
}


int num_phrase_to_num(string s)
{
  s = uCase(removePunctuation(removeSpace(s)));
  if (s == "ZERO" || s == "0" || s == "ZEROTH" || s == "0TH")
    return 0;
  if (s == "ONE" || s == "1" || s == "FIRST" || s == "1ST")
    return 1;
  if (s == "TWO" || s == "2" || s == "SECOND" || s == "2ND")
    return 2;
  if (s == "THREE" || s == "3" || s == "THIRD" || s == "3RD")
    return 3;
  if (s == "FOUR" || s == "4" || s == "FOURTH" || s == "4TH")
    return 4;
  if (s == "FIVE" || s == "5" || s == "FIFTH" || s == "5TH")
    return 5;
  if (s == "SIX" || s == "6" || s == "SIXTH" || s == "6TH")
    return 6;
  if (s == "SEVEN" || s == "7" || s == "SEVENTH" || s == "7TH")
    return 7;
  if (s == "EIGHT" || s == "8" || s == "EIGHTH" || s == "8TH")
    return 8;
  if (s == "NINE" || s == "9" || s == "NINTH" || s == "9TH")
    return 9;
  if (s == "TEN" || s == "10" || s == "TENTH" || s == "10TH")
    return 10;
  if (s == "ELEVEN" || s == "11" || s == "ELEVENTH" || s == "11TH")
    return 11;
  if (s == "TWELVE" || s == "12" || s == "TWELFTH" || s == "TWELVETH" || s == "12TH")
    return 12;
  if (s == "THIRTEEN" || s == "13" || s == "THIRTEENTH" || s == "13TH")
    return 13;
  if (s == "FOURTEEN" || s == "14" || s == "FOURTEENTH" || s == "14TH")
    return 14;
  if (s == "FIFTEEN" || s == "15" || s == "FIFTEENTH" || s == "15TH")
    return 15;
  if (s == "SIXTEEN" || s == "16" || s == "SIXTEENTH" || s == "16TH")
    return 16;
  if (s == "SEVENTEEN" || s == "17" || s == "SEVENTEENTH" || s == "17TH")
    return 17;
  if (s == "EIGHTEEN" || s == "18" || s == "EIGHTEENTH" || s == "18TH")
    return 18;
  if (s == "NINETEEN" || s == "19" || s == "NINTEENTH" || s == "19TH")
    return 19;
  if (s == "TWENTY" || s == "20" || s == "TWENTIETH" || s == "20TH")
    return 20;
  if (s == "TWENTYONE" || s == "21" || s == "TWENTYFIRST" || s == "21ST")
    return 21;
  if (s == "TWENTYTWO" || s == "22" || s == "TWENTYSECOND" || s == "22ND")
    return 22;
  if (s == "TWENTYTHREE" || s == "23" || s == "TWENTYTHIRD" || s == "23RD")
    return 23;
  if (s == "TWENTYFOUR" || s == "24" || s == "TWENTYFOURTH" || s == "24TH")
    return 24;
  if (s == "TWENTYFIVE" || s == "25" || s == "TWENTYFIFTH" || s == "25TH")
    return 25;
  if (s == "TWENTYSIX" || s == "26" || s == "TWENTYSIXTH" || s == "26TH")
    return 26;
  if (s == "TWENTYSEVEN" || s == "27" || s == "TWENTYSEVENTH" || s == "27TH")
    return 27;
  if (s == "TWENTYEIGHT" || s == "28" || s == "TWENTYEIGHTH" || s == "28TH")
    return 28;
  if (s == "TWENTYNINE" || s == "29" || s == "TWENTYNINTH" || s == "29TH")
    return 29;
  if (s == "THIRTY" || s == "30" || s == "THIRTIETH" || s == "30TH")
    return 30;
  if (s == "THIRYONE" || s == "31" || s == "THIRYFIRST" || s == "31ST")
    return 31;
  return -1;
}


bool isNo(string s)
{
  s = lCase(s);
  for (size_t i = 0; i < words_no_.size(); i++) {
    if (s == words_no_[i])
      return true;
  }
  return false;
}

bool isYes(string s)
{
  s = lCase(s);
  for (size_t i = 0; i < words_yes_.size(); i++) {
    if (s == words_yes_[i])
      return true;
  }
  return false;
}


int daysInMonth(int m, int y)
{
  if (m == 2)
    return 28 + isLeapYear(y);
  if (m == 4 || m == 6 || m == 9 || m == 11)
    return 30;
  return 31;
}

int daysInYear(int y)
{
  return 365 + isLeapYear(y);
}

bool isLeapYear(int y)
{
  return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}


void delay_loop(int lps)
{
  if (lps < 1)
    lps = DEFAULT_LPS;
  delay((int)(1000 / lps));
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