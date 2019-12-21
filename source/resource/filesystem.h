////////////////////////////////////////////////////////////////////////////////
//File:   filesystem.h
//Dir:    source/resource/
//Date:   2014-08-03
//Author: Zachary Gill
//Filesystem functions Interface
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SR_FILESYSTEM_H_
#define _DLA_SR_FILESYSTEM_H_


//standard includes
#include <string>
#include <vector>


//function definitions
bool                     fileExists            (std::string);

bool                     makeFile              (std::string);
bool                     deleteFile            (std::string);
bool                     copyFile              (std::string, std::string);
bool                     moveFile              (std::string, std::string);
bool                     renameFile            (std::string, std::string);

long long                fileSize              (std::string);

bool                     makeFolder            (std::string);
bool                     deleteFolder          (std::string);
bool                     copyFolder            (std::string, std::string);
bool                     moveFolder            (std::string, std::string);
bool                     renameFolder          (std::string, std::string);

void                     readDirectory         (std::string, std::vector<std::string>&);
void                     readDirectory         (std::string, std::string, std::vector<std::string>&);

void                     readDirectoryRec      (std::string, std::vector<std::string>&);
void                     readDirectoryRec      (std::string, std::vector<std::string>&, bool);
void                     readDirectoryRec      (std::string, std::string, std::vector<std::string>&);
void                     readDirectoryRec      (std::string, std::string, std::vector<std::string>&, bool);
void                     readDirectoryRec      (std::string, std::string, std::vector<std::string>&, std::string, bool);


#endif