////////////////////////////////////////////////////////////////////////////////
//File:   crypto.h
//Dir:    source/resource/
//Date:   2014-06-26
//Author: Zachary Gill
//Cryptography functions Interface
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SR_CRYPTO_H_
#define _DLA_SR_CRYPTO_H_


//constants
#include "const.h"


//standard includes
#include <string>
#include <vector>


//function definitions
std::string hasher         (std::string);
std::string hasher         (std::string, int);

std::string passHasher     (std::string, std::string, int, byte);

void        savePassword   (std::string, std::string, std::string, int);
bool        verifyPassword (std::string, std::string);

std::string generateSalt   (int);
int         randIterations ();
int         randIterations (int);

std::string encryptString  (std::string, std::string);
std::string decryptString  (std::string, std::string);

bool        encryptFile    (std::string, std::string);
bool        encryptFile    (std::string, std::string, std::string);
bool        decryptFile    (std::string, std::string);
bool        decryptFile    (std::string, std::string, std::string);

void        loadEncFileS   (std::string, std::string, std::vector<std::string>&, int&);
void        printEncFileS  (std::string, std::string, std::vector<std::string>&);


//shared function definitions
#include "common.h"


#endif