////////////////////////////////////////////////////////////////////////////////
//File:   filesystem.cpp
//Dir:    source/resource/
//Date:   2014-12-29
//Author: Zachary Gill
//Filesystem functions Implementation
////////////////////////////////////////////////////////////////////////////////

#define  NTDDI_VERSION NTDDI_WIN7
#define  WINVER        _WIN32_WINNT_WIN7
#define _WIN32_WINNT   _WIN32_WINNT_WIN7
#define _WIN32_IE      _WIN32_IE_IE50

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include <string>
#include <vector>
#include <cctype>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <boost/filesystem.hpp>


//includes
#include "filesystem.h"


//namespaces
using namespace std;


//functions
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