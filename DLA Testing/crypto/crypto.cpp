#define _CRT_SECURE_NO_WARNINGS

#define MAX_PASSWORD_LEN 128
#define DEFAULT_SALT_LEN 32

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

#define RANDOMIZE_TIMER srand((unsigned int)time(NULL))


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include "Windows.h"

#include "config.h"
#include "default.h"
#include "sha.h"
#include "aes.h"
#include "files.h"
#include "modes.h"
#include "filters.h"
#include "hex.h"
#include "pwdbased.h"


using namespace std;


string hasher(string);
string hasher(string, int);

string passHasher(string, string, int, byte);

void   savePassword(string, string, string, int);
bool   verifyPassword(string, string);

string generateSalt(int);
int    randIterations();

string encryptString(string, string);
string decryptString(string, string);

void   encryptFile(string, string);
void   encryptFile(string, string, string);
void   decryptFile(string, string);
void   decryptFile(string, string, string);

void   loadEncFileS(string, string, vector<string>&, int&);
void   printEncFileS(string, string, vector<string>&);

bool   fileExists(string);
string str(char);
string randString(int);
bool   getint(fstream&, int&);
void   loadFileS(string, vector<string>&);


int main()
{
  //test hasher
  //string in = "a test string";
  //int iterations = 1;

  //string hash = hasher(in, iterations);
  //cout << hash << endl;
  



  //test password hashing, saving, and verifying
  //string password = "testing";
  //string salt = generateSalt(DEFAULT_SALT_LEN);
  //int iterations = randIterations();

  //string truehash = passHasher(password, salt, iterations, NULL);

  //savePassword("pass.txt", truehash, salt, iterations);

  //cout << truehash << endl << endl;

  //bool verified = false;
  //while (true) {
  //  string testpass = "";
  //  cout << ": ";
  //  getline(cin, testpass);
  //  if (verifyPassword(testpass, "pass.txt")) {
  //    cout << "password verified" << endl;
  //    break;
  //  }
  //  else {
  //    cout << "password incorrect, try again" << endl;
  //  }
  //}




  //test en/de crypt strings
  //string password = "testing";
  //string message = "just a simple test string";
  //cout << message << endl;
  //message = encryptString(message, password);
  //cout << message << endl;
  //message = decryptString(message, password);
  //cout << message << endl;





  //test en/de crypt files 1
  //string password = "testing";
  //encryptFile("log.log", password, "log2.log");
  //system("PAUSE");
  //decryptFile("log2.log", password, "log3.log");




  //test read/write encrypted files
  //string password = "testing";
  //vector<string> data;
  //int dataLen;

  //loadEncFileS("log2.log", password, data, dataLen);

  //for (int i = 0; i < dataLen; i++)
  //  cout << data[i] << endl;

  //printEncFileS("log4.log", password, data);



  system("PAUSE");
  return 0;
}


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
  getint (fs, iterations);
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
  RANDOMIZE_TIMER;
  return rand() % ITERATIONS_SPREAD_EXTREME + MIN_ITERATIONS_EXTREME;
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

void encryptFile(string fileIn, string password, string fileOut)
{
  if (!fileExists(fileIn) || fileIn == fileOut)
    return;

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
}

void decryptFile(string fileIn, string password, string fileOut)
{
  if (!fileExists(fileIn) || fileIn == fileOut)
    return;

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








bool fileExists(string name)
{
  fstream file(name.c_str());
  if (!file)
    return false;
  else
    return true;
}

string str(char c)
{
  stringstream ss;
  string s;
  ss << c;
  ss >> s;
  return s;
}

string randString(int size)
{
  RANDOMIZE_TIMER;
  string r = "";
  for (int i = 0; i < size; i++) {
    int n = rand() % 255 + 1;
    r += str((char)n);
  }
  return r;
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