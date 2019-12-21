////////////////////////////////////////////////////////////////////////////////
//File:   crypto.cpp
//Dir:    source/resource/
//Date:   2014-12-28
//Author: Zachary Gill
//Cryptography functions Implementation
////////////////////////////////////////////////////////////////////////////////

#define  NTDDI_VERSION NTDDI_WIN7
#define  WINVER        _WIN32_WINNT_WIN7
#define _WIN32_WINNT   _WIN32_WINNT_WIN7
#define _WIN32_IE      _WIN32_IE_IE50

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


//constants
#include "const.h"

#define DEFAULT_SALT_LEN 32

#define ITERATIONS_MIN     1
#define ITERATIONS_LOW     2
#define ITERATIONS_NORMAL  3
#define ITERATIONS_HIGH    4
#define ITERATIONS_XHIGH   5
#define ITERATIONS_EXTREME 6

#define ITERATIONS_SPREAD_MIN     16384
#define MIN_ITERATIONS_MIN        16384
#define ITERATIONS_SPREAD_LOW     32768
#define MIN_ITERATIONS_LOW        32768
#define ITERATIONS_SPREAD_NORMAL  65536
#define MIN_ITERATIONS_NORMAL     65536
#define ITERATIONS_SPREAD_HIGH    131072
#define MIN_ITERATIONS_HIGH       131072
#define ITERATIONS_SPREAD_XHIGH   262144
#define MIN_ITERATIONS_XHIGH      262144
#define ITERATIONS_SPREAD_EXTREME 524288
#define MIN_ITERATIONS_EXTREME    524288


//standard includes
#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>

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
#include "crypto.h"


//namespaces
using namespace std;


//functions

string hasher(string in)
{
  return hasher(in, 1);
}
string hasher(string in, int iterations)
{
  if (in.length() == 0 || iterations < 1)
    return "";

  string out;
  CryptoPP::SHA512 hashSlingingSlasher;

  for (int i = 0; i < iterations; i++) {
    out = "";
    CryptoPP::StringSource foo(in, true,
      new CryptoPP::HashFilter(hashSlingingSlasher,
      new CryptoPP::HexEncoder(
      new CryptoPP::StringSink(out))));
    in = out;
  }

  return out;
}

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

void savePassword(string file, string hash, string salt, int iterations)
{
  if (hash.length() < 1 || iterations < 1)
    return;

  fstream fs;
  fs.open(file.c_str(), fstream::out);
  fs << hash << endl
    << salt << endl
    << iterations;
  fs.close();
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

string generateSalt(int size)
{
  if (size < 1)
    return "";

  return randString(size);
}

int randIterations()
{
  return randIterations(ITERATIONS_NORMAL);
}
int randIterations(int difficulty)
{
  RANDOMIZE_TIMER;
  switch (difficulty) {
    case ITERATIONS_MIN:
      return rand() % ITERATIONS_SPREAD_MIN + MIN_ITERATIONS_MIN;
    case ITERATIONS_LOW:
      return rand() % ITERATIONS_SPREAD_LOW + MIN_ITERATIONS_LOW;
    case ITERATIONS_NORMAL:
      return rand() % ITERATIONS_SPREAD_NORMAL + MIN_ITERATIONS_NORMAL;
    case ITERATIONS_HIGH:
      return rand() % ITERATIONS_SPREAD_HIGH + MIN_ITERATIONS_HIGH;
    case ITERATIONS_XHIGH:
      return rand() % ITERATIONS_SPREAD_XHIGH + MIN_ITERATIONS_XHIGH;
    case ITERATIONS_EXTREME:
      return rand() % ITERATIONS_SPREAD_EXTREME + MIN_ITERATIONS_EXTREME;
    default:
      return rand() % ITERATIONS_SPREAD_NORMAL + MIN_ITERATIONS_NORMAL;
  }
}

string encryptString(string data, string password)
{
  if (data.length() < 1)
    return "";

  password = password.substr(0, CryptoPP::AES::MAX_KEYLENGTH - 1);

  string encryptedData;

  byte key[CryptoPP::AES::MAX_KEYLENGTH];
  byte iv[CryptoPP::AES::BLOCKSIZE];
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
  memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
  memcpy(key, password.c_str(), password.size());

  CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::MAX_KEYLENGTH);
  CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

  CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(encryptedData));
  stfEncryptor.Put(reinterpret_cast<const unsigned char*>(data.c_str()), data.length() + 1);
  stfEncryptor.MessageEnd();

  return encryptedData;
}

string decryptString(string data, string password)
{
  if (data.length() < 1)
    return "";

  password = password.substr(0, CryptoPP::AES::MAX_KEYLENGTH - 1);

  string decryptedData;

  byte key[CryptoPP::AES::MAX_KEYLENGTH];
  byte iv[CryptoPP::AES::BLOCKSIZE];
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
  memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
  memcpy(key, password.c_str(), password.size());

  CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::MAX_KEYLENGTH);
  CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

  CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedData));
  stfDecryptor.Put(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
  stfDecryptor.MessageEnd();

  return decryptedData;
}

bool encryptFile(string file, string password)
{
  if (encryptFile(file, password, file + "~")) {
    if (deleteFile(file)) {
      if (renameFile(file + "~", file))
        return true;
      return false;
    }
    return false;
  }
  return false;
}
bool encryptFile(string fileIn, string password, string fileOut)
{
  if (!fileExists(fileIn) || fileIn == fileOut || fileSize(fileIn) == 0)
    return false;

  password = password.substr(0, CryptoPP::AES::MAX_KEYLENGTH - 1);

  byte key[CryptoPP::AES::MAX_KEYLENGTH];
  byte iv[CryptoPP::AES::BLOCKSIZE];
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
  memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
  memcpy(key, password.c_str(), password.size());

  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
  encryptor.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, iv, CryptoPP::AES::BLOCKSIZE);

  CryptoPP::FileSource file_source(fileIn.c_str(), true,
    new CryptoPP::StreamTransformationFilter(encryptor,
    new CryptoPP::FileSink(fileOut.c_str())));

  return true;
}

bool decryptFile(string file, string password)
{
  if (decryptFile(file, password, file + "~")) {
    if (deleteFile(file)) {
      if (renameFile(file + "~", file))
        return true;
      return false;
    }
    return false;
  }
  return false;
}
bool decryptFile(string fileIn, string password, string fileOut)
{
  if (!fileExists(fileIn) || fileIn == fileOut || fileSize(fileIn) == 0)
    return false;

  password = password.substr(0, CryptoPP::AES::MAX_KEYLENGTH - 1);

  byte key[CryptoPP::AES::MAX_KEYLENGTH];
  byte iv[CryptoPP::AES::BLOCKSIZE];
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
  memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
  memcpy(key, password.c_str(), password.size());

  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
  decryptor.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, iv, CryptoPP::AES::BLOCKSIZE);

  CryptoPP::FileSource file_source(fileIn.c_str(), true,
    new CryptoPP::StreamTransformationFilter(decryptor,
    new CryptoPP::FileSink(fileOut.c_str())));

  return true;
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

void printEncFileS(string file, string password, vector<string>& data)
{
  if (data.size() < 1)
    return;

  password = password.substr(0, CryptoPP::AES::MAX_KEYLENGTH - 1);

  byte key[CryptoPP::AES::MAX_KEYLENGTH];
  byte iv[CryptoPP::AES::BLOCKSIZE];
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
  memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
  memcpy(key, password.c_str(), password.size());

  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
  encryptor.SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, iv, CryptoPP::AES::BLOCKSIZE);

  stringstream ss;
  for (size_t i = 0; i < data.size(); i++) {
    ss << data[i] << endl;
  }

  string temp = ss.str();

  CryptoPP::StringSource css(temp, true,
    new CryptoPP::StreamTransformationFilter(encryptor,
    new CryptoPP::FileSink(file.c_str())));
}