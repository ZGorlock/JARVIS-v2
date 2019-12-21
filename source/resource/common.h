////////////////////////////////////////////////////////////////////////////////
//File:   common.h
//Dir:    source/resource/
//Date:   2014-12-28
//Author: Zachary Gill
//Common functions Interface
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SR_COMMON_H_
#define _DLA_SR_COMMON_H_


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>


//includes
#include "filesystem.h"
#include "timer.h"


//function definitions
bool                     getint                  (std::istream&, int&);
bool                     getint                  (std::istream&, int&, bool);
bool                     getdouble               (std::istream&, double&);
bool                     getdouble               (std::istream&, double&, bool);
bool                     getbool                 (std::istream&, bool&);
bool                     getbool                 (std::istream&, bool&, bool);

void                     loadFileS               (std::string, std::vector<std::string>&);
void                     loadFileI               (std::string, std::vector<int>&);
void                     loadFileD               (std::string, std::vector<double>&);
std::string              loadFileStr             (std::string);

void                     printFileS              (std::string, std::vector<std::string>&);
void                     printFileI              (std::string, std::vector<int>&);
void                     printFileD              (std::string, std::vector<double>&);

void                     cls                     (HANDLE);

void                     clipboardOut            (std::string);
std::string              clipboardIn             ();

void                     alphabetize             (std::vector<std::string>&);
std::vector<std::string> tokenize                (std::string);
std::vector<std::string> tokenize                (std::string, bool);

std::string              removeSpace             (std::string);
std::string              removePunctuation       (std::string);
void                     removePunctuationSoft   (std::vector<std::string>&);
void                     ReplaceStringInPlace    (std::string&, const std::string&, const std::string&);
std::wstring             stringToWideString      (const std::string&);
bool                     str_is_0                (std::string);

std::string              randString              (int);
std::string              randString              (int, int, int);

std::string              uCase                   (std::string);
std::string              lCase                   (std::string);

void                     trim                    (std::string&);
void                     lTrim                   (std::string&);
void                     rTrim                   (std::string&);

int                      val                     (std::string);
double                   vald                    (std::string);
std::string              str                     (int);
std::string              str                     (double);
std::string              str                     (char);

std::string              zeroset                 (std::string&, int);

bool                     inRange                 (int, int, int);
bool                     inRange                 (int, int, int, bool, bool);
bool                     inRange                 (double, double, double);
bool                     inRange                 (double, double, double, bool, bool);

double                   stringDistance          (std::string, std::string);
double                   stringDistance          (std::string, std::string, bool);

double                   compareString           (std::string, std::string);
double                   compareStringPhonetic   (std::string, std::string);

int                      num_phrase_to_num       (std::string);

bool                     isNo                    (std::string);
bool                     isYes                   (std::string);

int                      daysInMonth             (int, int);
int                      daysInYear              (int);
bool                     isLeapYear              (int);

void                     delay_loop              (int);
void                     delay                   (int);

std::string              generateGuid            ();

std::string              getEnvVar               (std::string);

void                     runBatch                (std::string);
void                     runBatch                (std::string, bool);
void                     runBatch                (std::string, std::vector<std::string>);
void                     runBatch                (std::string, std::vector<std::string>, bool);


//resrources
extern std::string              loc_;
extern bool                     keyboard_touch_[NUM_LETTERS][NUM_LETTERS];
extern std::vector<std::string> words_no_;
extern std::vector<std::string> words_yes_;


#endif