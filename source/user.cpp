////////////////////////////////////////////////////////////////////////////////
//File:   user.cpp
//Dir:    source/
//Date:   2015-05-10
//Author: Zachary Gill
//Implementation of User
////////////////////////////////////////////////////////////////////////////////


//constants
#include "const.h"


//standard includes
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <time.h>


//includes
#include "user.h"
#include "settings.h"
#include "contact.h"
#include "input.h"
#include "output.h"
#include "module.h"

#include "input/keyboard_I.h"

#include "crypto.h"


//namespaces
using namespace std;


//Constructors

User::User()
{
  User::username = "";
  User::id = "";
  User::password = "";
  if (userList_.size() == 0)
    User::admin = true;
  else
    User::admin = false;
  User::loc = "data/user/" + User::username + "/";
  User::info = Contact::Contact();
}


void User::newLogin(string username, string password, string pin, string website)
{
  Login temp;
  temp.username = username;
  temp.password = password;
  temp.pin = pin;
  temp.website = website;
  User::addLogin(temp);
}
void User::newLogin(istream& is)
{
  Login temp;
  getline(is, temp.username);
  getline(is, temp.password);
  getline(is, temp.pin);
  getline(is, temp.website);
  User::addLogin(temp);
}


//Destructors

User::~User()
{
}

void User::free()
{
  User::~User();
}


//Accessors

string User::getUsername() const
{
  return User::username;
}

string User::getId() const
{
  return User::id;
}

bool User::getAdmin() const
{
  return User::admin;
}

Contact::DLA User::getDla() const
{
  return User::dla;
}

string User::getLoc() const
{
  return User::loc;
}

Contact User::getInfo() const
{
  return User::info;
}

Contact User::getContact(int element) const
{
  return User::contacts[element];
}


//Modifiers

void User::setUsername(string username)
{
  User::username = username;
}

void User::setId(string id)
{
  User::id = id;
}

void User::setPassword(string password)
{
  User::password = password;
}

void User::setAdmin(bool admin)
{
  pauseInputs();
  pauseOutputs();
  
  if (admin == true){
    if (User::admin == true) {
      cls(hStdout_);
      cout << "You are already an admin." << endl;
      system("PAUSE");
    }
    else {
      if (userList_.size() == 1) {
        User::admin = true;
      }
      else {
        string userStr = "";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        do {
          User tmpUser;
          cls(hStdout_);
          cout << "In order to become an admin, you need the permission of a current admin." << endl
            << "Ask a current admin to log in to allow your request." << endl;
          cout << "Enter '' to Cancel" << endl << endl;
          cout << "Admin Username: ";
          getline(cin, userStr);
          if (userStr > "") {
            tmpUser.setUsername(userStr);
            tmpUser.setLoc("data/user/" + tmpUser.getUsername() + "/");

            bool isForeground = false;
            string tmpStr = "";
            int in;
            GUITHREADINFO gti;
            gti.cbSize = sizeof(GUITHREADINFO);
            bool authenticated = false;
            while (INKEY()) {
              DELAY_LOOP;
            }
            do {
              DELAY_LOOP;
              COORD coordScreen = { 0, 5 };
              SetConsoleCursorPosition(hStdout_, coordScreen);
              cout << "Admin Password: ";
              for (size_t i = 0; i < tmpStr.length(); i++)
                cout << "*";
              GetGUIThreadInfo(NULL, &gti);
              if (gti.hwndFocus == hWnd_)
                isForeground = true;
              else
                isForeground = false;
              if (isForeground) {
                if (in = INKEY()) {
                  if (in == 8) {
                    if (tmpStr.length() > 0) {
                      coordScreen = { (15 + (int)tmpStr.length()), 5 };
                      SetConsoleCursorPosition(hStdout_, coordScreen);
                      cout << " ";
                      SetConsoleCursorPosition(hStdout_, coordScreen);
                      tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
                    }
                  }
                  else if (in != 13) {
                    if (tmpStr.length() < MAX_PASSWORD_LEN)
                      tmpStr += str((char)in);
                  }
                }
              }
            } while (in != 13);
            if (tmpStr > "") {
              tmpUser.setPassword(tmpStr);
              authenticated = tmpUser.authenticateUser();
              if (authenticated) {
                stringstream ss;
                tmpUser.loadUserFileS("info", ss);
                Contact info(ss);
                getbool(ss, tmpUser.admin);
                ss.clear();
              }
              
            }
            cin.clear();
            cin.ignore(INT_MAX, '\n');

            if (authenticated && tmpUser.getAdmin()) {
              User::admin = true;
              tmpUser.free();
              cls(hStdout_);
              cout << "You have now become an admin!" << endl;
              system("PAUSE");
            }
            else {
              cls(hStdout_);
              cout << "The admin log-in failed!" << endl;
              system("PAUSE");
            }
          }
        } while (userStr > "" && User::admin == false);
      }
    }
  }
  else {
    User::admin = admin;
    cls(hStdout_);
    cout << "You are no longer an admin." << endl;
    system("PAUSE");
  }

  startOutputs();
  startInputs();
}

void User::setDla(Contact::DLA dla)
{
  User::dla = dla;
}

void User::setLoc(string loc)
{
  User::loc = loc;
}

void User::setInfo(Contact info)
{
  User::info = info;
}

void User::addContact(Contact contact)
{
  User::contacts.push_back(contact);
}

void User::addLogin(Login login)
{
  User::logins.push_back(login);
}


//Other Functions

void User::loadUser()
{
  loadFileS("data/users", userList_);
  
  User::login();
  userLoc_ = User::getLoc();
  
  cls(hStdout_);

  fstream fs;
  stringstream ss;

  Contact::DLA dla;
  string tmpStr = "";

  fs.open(User::getLoc() + "id", fstream::in);
  getline(fs, tmpStr);
  User::setId(tmpStr);
  fs.close();

  fs.open(User::getLoc() + "dla", fstream::in);
  getline(fs, dla.name);
  getint (fs, dla.gender);
  User::setDla(dla);
  name_ = dla.name;
  gender_ = dla.gender;
  fs.close();

  User::loadUserFileS("info", ss);
  Contact info(ss);
  User::setInfo(info);
  getbool(ss, User::admin);
  ss.clear();

  if (User::isEncrypted("log")) {
    decryptFile(User::getLoc() + "log.log", User::password);
    fs.open(User::getLoc() + "log.crypt", fstream::out);
    fs << "0";
    fs.close();
  }
  
  User::loadUserFileS("contacts", ss);
  int contacts = 0;
  User::contacts.clear();
  getint(ss, contacts);
  while (contacts > 0) {
    Contact temp(ss);
    User::addContact(temp);
    getint(ss, contacts);
  }
  ss.clear();
  
  User::loadUserFileS("logins", ss);
  int logins = 0;
  User::logins.clear();
  getint(ss, logins);
  while (logins > 0) {
    User::newLogin(ss);
    getint(ss, logins);
  }
}

void User::login()
{
  vector<string> data;
  loadFileS("data/users", data);
  int in = 0;
  string tmpStr;
  int choice = 0;
  int a = 0;
  int b = 0;
  bool authenticated = false;

  GUITHREADINFO gti;
  gti.cbSize = sizeof(GUITHREADINFO);
  bool isForeground;

  if (data.size() == 1) {
    do {
      User::setUsername(data[0]);
      User::setLoc("data/user/" + User::getUsername() + "/");
      
      tmpStr = "";
      isForeground = false;
      cls(hStdout_);
      cout << "Log-in. Enter '' to go to the users' screen." << endl << endl;
      cout << "Username: " << User::getUsername() << endl;
      while (INKEY()) {
        DELAY_LOOP;
      }
      do {
        DELAY_LOOP;

        COORD coordScreen = { 0, 3 };
        SetConsoleCursorPosition(hStdout_, coordScreen);

        cout << "Password: ";
        if (showPassword_) {
          cout << tmpStr;
        }
        else {
          for (size_t i = 0; i < tmpStr.length(); i++)
            cout << "*";
        }
        
        GetGUIThreadInfo(NULL, &gti);
        if (gti.hwndFocus == hWnd_)
          isForeground = true;
        else
          isForeground = false;

        if (isForeground) {
          if (in = INKEY()) {
            if (in == 8) {
              if (tmpStr.length() > 0) {
                coordScreen = { (9 + (int)tmpStr.length()), 3 };
                SetConsoleCursorPosition(hStdout_, coordScreen);
                cout << " ";
                SetConsoleCursorPosition(hStdout_, coordScreen);
                tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
              }
            }
            else if (in != 13) {
              if (tmpStr.length() < MAX_PASSWORD_LEN)
                tmpStr += str((char)in);
            }
          }
        }
      } while (in != 13);
      if (tmpStr > "") {
        User::setPassword(tmpStr);
        authenticated = User::authenticateUser();
      }
      cin.clear();
      cin.ignore(INT_MAX, '\n');
    } while (tmpStr > "" && !authenticated);
  }

  if (!authenticated) {
    do {
      do {
        while (INKEY()) {
          DELAY_LOOP;
        }
        cls(hStdout_);
        cout << "0 - Other Options" << endl;
        for (size_t i = 0; i < data.size(); i++)
          cout << i + 1 << " - " << data[i] << endl;
        cout << ": ";
        getline(cin, tmpStr);
        choice = val(tmpStr);
      } while (choice >(int)data.size() && choice != 0);

      switch (choice) {
        case 0:
          do {
            while (INKEY()) {
              DELAY_LOOP;
            }
            cls(hStdout_);
            cout << "0 - Return" << endl
                 << "1 - New User" << endl
                 << "2 - Delete User" << endl
                 << "3 - Backup User" << endl
                 << ": ";
            getline(cin, tmpStr);
            a = val(tmpStr);
            switch (a) {
              case 0:
                break;
              case 1:
                createUser();
                data.clear();
                loadFileS("data/users", data);
                break;
              case 2:
                do {
                  while (INKEY()) {
                    DELAY_LOOP;
                  }
                  cls(hStdout_);
                  cout << "Delete User:" << endl;
                  for (size_t i = 0; i < data.size(); i++)
                    cout << "[" << i + 1 << "] - " << data[i] << endl;
                  cout << endl
                    << "0 - Return" << endl
                    << "X - Delete User [X]" << endl << endl;
                  cout << ": ";
                  getline(cin, tmpStr);
                  b = val(tmpStr);
                  switch (b) {
                    case 0:
                      break;
                    default:
                      if (b > 0 && b < (int)data.size() + 1) {
                        User::setUsername(data[b - 1]);
                        User::setLoc("data/user/" + User::getUsername() + "/");
                        tmpStr = "";
                        isForeground = false;
                        cls(hStdout_);
                        cout << "Username: " << User::getUsername() << endl;
                  
                        while (INKEY()) {
                          DELAY_LOOP;
                        }
                        do {
                          DELAY_LOOP;

                          COORD coordScreen = { 0, 1 };
                          SetConsoleCursorPosition(hStdout_, coordScreen);

                          cout << "Password: ";
                          if (showPassword_) {
                            cout << tmpStr;
                          }
                          else {
                            for (size_t i = 0; i < tmpStr.length(); i++)
                              cout << "*";
                          }
        
                          GetGUIThreadInfo(NULL, &gti);
                          if (gti.hwndFocus == hWnd_)
                            isForeground = true;
                          else
                            isForeground = false;

                          if (isForeground) {
                            if (in = INKEY()) {
                              if (in == 8) {
                                if (tmpStr.length() > 0) {
                                  coordScreen = { (9 + (int)tmpStr.length()), 1 };
                                  SetConsoleCursorPosition(hStdout_, coordScreen);
                                  cout << " ";
                                  SetConsoleCursorPosition(hStdout_, coordScreen);
                                  tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
                                }
                              }
                              else if (in != 13) {
                                if (tmpStr.length() < MAX_PASSWORD_LEN)
                                  tmpStr += str((char)in);
                              }
                            }
                          }
                        } while (in != 13);
                        if (tmpStr > "") {
                          User::setPassword(tmpStr);
                          if (User::authenticateUser()) {
                            cin.clear();
                            cin.ignore(INT_MAX, '\n');
                            User::deleteUser();
                            data.clear();
                            loadFileS("data/users", data);
                          }
                          else {
                            cin.clear();
                            cin.ignore(INT_MAX, '\n');
                          }
                          tmpStr = "";
                        }
                      }
                  }
                } while (b > 0);
                break;
              case 3:
                do {
                  while (INKEY()) {
                    DELAY_LOOP;
                  }
                  cls(hStdout_);
                  cout << "Backup User:" << endl;
                  for (size_t i = 0; i < data.size(); i++)
                    cout << "[" << i + 1 << "] - " << data[i] << endl;
                  cout << endl
                    << "0 - Return" << endl
                    << "X - Backup User [X]" << endl << endl;
                  cout << ": ";
                  getline(cin, tmpStr);
                  b = val(tmpStr);
                  switch (b) {
                    case 0:
                      break;
                    default:
                      if (b > 0 && b < (int)data.size() + 1) {
                        User::setUsername(data[b - 1]);
                        User::setLoc("data/user/" + User::getUsername() + "/");
                        tmpStr = "";
                        isForeground = false;
                        cls(hStdout_);
                        cout << "Username: " << User::getUsername() << endl;
                        while (INKEY()) {}
                        do {
                          DELAY_LOOP;

                          COORD coordScreen = { 0, 1 };
                          SetConsoleCursorPosition(hStdout_, coordScreen);

                          cout << "Password: ";
                          if (showPassword_) {
                            cout << tmpStr;
                          }
                          else {
                            for (size_t i = 0; i < tmpStr.length(); i++)
                              cout << "*";
                          }
        
                          GetGUIThreadInfo(NULL, &gti);
                          if (gti.hwndFocus == hWnd_)
                            isForeground = true;
                          else
                            isForeground = false;

                          if (isForeground) {
                            if (in = INKEY()) {
                              if (in == 8) {
                                if (tmpStr.length() > 0) {
                                  coordScreen = { (9 + (int)tmpStr.length()), 1 };
                                  SetConsoleCursorPosition(hStdout_, coordScreen);
                                  cout << " ";
                                  SetConsoleCursorPosition(hStdout_, coordScreen);
                                  tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
                                }
                              }
                              else if (in != 13) {
                                if (tmpStr.length() < MAX_PASSWORD_LEN)
                                  tmpStr += str((char)in);
                              }
                            }
                          }
                        } while (in != 13);
                        if (tmpStr > "") {
                          User::setPassword(tmpStr);
                          if (User::authenticateUser()) {
                            User::backupUser();
                            cls(hStdout_);
                            cout << "A backup of " << User::getUsername() << " has been made." << endl;
                            system("PAUSE");
                          }
                        }
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                      }
                    }
                } while (b > 0);
                break;
            }
          } while (a > 0);
          break;
        default:
          User::setUsername(data[choice - 1]);
          User::setLoc("data/user/" + User::getUsername() + "/");

          tmpStr = "";
          bool isForeground = false;
          cls(hStdout_);
          cout << "Log-in. Enter '' to go to the users' screen." << endl << endl;
          cout << "Username: " << User::getUsername() << endl;
          while (INKEY()) {
            DELAY_LOOP;
          }
          do {
            DELAY_LOOP;

            COORD coordScreen = { 0, 3 };
            SetConsoleCursorPosition(hStdout_, coordScreen);

            cout << "Password: ";
            if (showPassword_) {
              cout << tmpStr;
            }
            else {
              for (size_t i = 0; i < tmpStr.length(); i++)
                cout << "*";
            }

            GetGUIThreadInfo(NULL, &gti);
            if (gti.hwndFocus == hWnd_)
              isForeground = true;
            else
              isForeground = false;

            if (isForeground) {
              if (in = INKEY()) {
                if (in == 8) {
                  if (tmpStr.length() > 0) {
                    coordScreen = { (9 + (int)tmpStr.length()), 3 };
                    SetConsoleCursorPosition(hStdout_, coordScreen);
                    cout << " ";
                    SetConsoleCursorPosition(hStdout_, coordScreen);
                    tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
                  }
                }
                else if (in != 13) {
                  if (tmpStr.length() < MAX_PASSWORD_LEN)
                    tmpStr += str((char)in);
                }
              }
            }
          } while (in != 13);
          if (tmpStr > "") {
            User::setPassword(tmpStr);
            authenticated = User::authenticateUser();
          }
          cin.clear();
          cin.ignore(INT_MAX, '\n');
      }
    } while (!authenticated);
  }
  authenticated = false;

  User::setPassword(tmpStr);
  tmpStr = "";
}


bool User::login_simple()
{
  bool authenticated = false;
  string userStr = "";
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  cls(hStdout_);
  cout << "Log-in" << endl << endl;
  cout << "Username: ";
  getline(cin, userStr);
  if (userStr > "") {
    User::username = userStr;
    User::loc = "data/user/" + User::username + "/";

    bool isForeground = false;
    string tmpStr = "";
    int in;
    GUITHREADINFO gti;
    gti.cbSize = sizeof(GUITHREADINFO);
    while (INKEY()) {
      DELAY_LOOP;
    }
    do {
      DELAY_LOOP;
      COORD coordScreen = { 0, 3 };
      SetConsoleCursorPosition(hStdout_, coordScreen);
      cout << "Password: ";
      for (size_t i = 0; i < tmpStr.length(); i++)
        cout << "*";
      GetGUIThreadInfo(NULL, &gti);
      if (gti.hwndFocus == hWnd_)
        isForeground = true;
      else
        isForeground = false;
      if (isForeground) {
        if (in = INKEY()) {
          if (in == 8) {
            if (tmpStr.length() > 0) {
              coordScreen = { (9 + (int)tmpStr.length()), 3 };
              SetConsoleCursorPosition(hStdout_, coordScreen);
              cout << " ";
              SetConsoleCursorPosition(hStdout_, coordScreen);
              tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
            }
          }
          else if (in != 13) {
            if (tmpStr.length() < MAX_PASSWORD_LEN)
              tmpStr += str((char)in);
          }
        }
      }
    } while (in != 13);
    if (tmpStr > "") {
      User::setPassword(tmpStr);
      authenticated = User::authenticateUser();
    }
    cin.clear();
    cin.ignore(INT_MAX, '\n');

    if (authenticated) {
      cls(hStdout_);
      cout << "Logged in as " << User::getUsername() << "!" << endl;
      system("PAUSE");
    }
    else {
      cls(hStdout_);
      cout << "The log-in failed!" << endl;
      system("PAUSE");
    }
  }
  return authenticated;
}

void User::loadUser_simple()
{
  fstream fs;
  stringstream ss;

  string tmpStr = "";

  fs.open(User::getLoc() + "id", fstream::in);
  getline(fs, User::id);
  fs.close();

  User::loadUserFileS("info", ss);
  Contact info(ss);
  getbool(ss, User::admin);
  ss.clear();
}


void User::backupUser()
{
  string source = loc_ + "data/user/" + User::getUsername();
  string timeStr = str((int)time(0));
  string dest = loc_ + "data/backup/user/" + lCase(User::getUsername()) + "/" + timeStr;
  copyFolder(source, dest);
  runBatch("bin/batch/zip.bat", vector<string>{"data/backup/user/" + lCase(User::getUsername()), "../../../../", timeStr + ".zip", timeStr, "1"});
}


void User::changePassword()
{
  pauseInputs();
  pauseOutputs();

  string tmpStr = "";
  string password = "";
  string verify = "";
  string hash = "";
  bool verified1 = true;
  bool verified2 = true;
  int iterations;

  do {
    cls(hStdout_);
    if (!verified1)
      cout << "This password is incorrect, please try again." << endl;
    cout << "Enter your current password. Enter '' to cancel." << endl
         << "Current Password: ";
    getline(cin, tmpStr);
    if (tmpStr == "") {
      startOutputs();
      startInputs();
      return;
    }
    verified1 = verifyPassword(tmpStr, User::getLoc() + "pass");
  } while (!verified1);
  
  do {
    do {
      cls(hStdout_);
      if (!verified2)
        cout << "The passwords you entered did not match, please try again." << endl;
      cout << "Create a New Password." << endl
           << "New Password: ";
      getline(cin, password);
    } while (password == "");
    iterations = randIterations();
    do {
      cls(hStdout_);
      cout << "Reenter your Password to Verify." << endl
           << "Password: ";
      getline(cin, verify);
    } while (verify == "");
    verified2 = (verify == password);
  } while (!verified2);

  User::backupUser();

  hash = passHasher(password, User::getUsername() + User::getId(), iterations, NULL);
  savePassword(User::getLoc() + "pass", hash, User::getUsername() + User::getId(), iterations);
  User::registerPassword(hash, iterations);

  cls(hStdout_);
  cout << "Your password has been changed." << endl;
  system("PAUSE");
  cls(hStdout_);

  startOutputs();
  startInputs();
}

void User::resetPassword()
{
  //reset user password
}


void User::loadUserInputs() const
{
  loadInputs(User::getUsername());
}

void User::loadUserOutputs() const
{
  loadOutputs(User::getUsername());
}

void User::loadUserModules() const
{
  loadModules(User::getUsername());
}


void User::save()
{
  fstream fs;
  stringstream ss;

  User::info.save(ss);
  ss << User::getAdmin() << endl;
  User::printUserFileS("info", ss);
  ss.clear();
  
  if (User::isDecrypted("log")) {
    encryptFile(User::getLoc() + "log.log", User::password);
    fs.open(User::getLoc() + "log.crypt", fstream::out);
    fs << "1";
    fs.close();
  }
  
  for (size_t i = 0; i < User::contacts.size(); i++) {
    ss << i << endl;
    User::contacts[i].save(ss);
  }
  ss << "0";
  User::printUserFileS("contacts", ss);
  ss.clear();
  
  for (size_t i = 0; i < User::logins.size(); i++) {
    ss << i << endl;
    ss << User::logins[i].username << endl
       << User::logins[i].password << endl
       << User::logins[i].pin      << endl
       << User::logins[i].website  << endl;
  }
  ss << "0";
  User::printUserFileS("logins", ss);
  ss.clear();
}

void User::dump() const
{
  User::dump(cout);
}
void User::dump(ostream& stream) const
{
  stream << "Username: " << User::getUsername() << endl
         << "Id: "       << User::getId()       << endl
         << "Admin: "    << User::getAdmin()    << endl
         << "Loc: "      << User::getLoc()      << endl << endl;
  User::getInfo().dump();
}


//Private Functions

User::Login User::getLogin(int element)
{
  return User::logins[element];
}


void User::createUser()
{
  string username = "";
  string id = "";
  string password = "";
  string hash = "";
  string verify = "";
  int iterations = 0;
  bool verifiedUsername = true;
  bool verifiedPassword = true;
  bool registered = true;
  
  do {
    cls(hStdout_);
    if (!verifiedUsername)
      cout << "The username you entered is unusable or taken, please try another." << endl;
    cout << "Create a Username." << endl
         << "Username: ";
    getline(cin, username);
    User::setUsername(username);
    verifiedUsername = User::verifyUsername();
  } while (!verifiedUsername);
  User::setLoc("data/user/" + username + "/");
  User::populateUserDirectory();

  id = generateGuid();
  fstream saveId;
  saveId.open(User::getLoc() + "id", fstream::out);
  saveId << id;
  saveId.close();
  
  do {
    do {
      cls(hStdout_);
      if (!verifiedPassword)
        cout << "The passwords you entered did not match, please try again." << endl;
      cout << "Create a Password." << endl
           << "Password: ";
      getline(cin, password);
    } while (password == "");
    iterations = randIterations();
    do {
      cls(hStdout_);
      cout << "Reenter your Password to Verify." << endl
           << "Password: ";
      getline(cin, verify);
    } while (verify == "");
    verifiedPassword = (verify == password);
  } while (!verifiedPassword);

  hash = passHasher(password, username + id, iterations, NULL);
  savePassword(User::getLoc() + "pass", hash, username + id, iterations);
  User::setPassword(password);

  fstream fs;
  stringstream ss;
  
  User::createDla();
  fs.open(User::getLoc() + "dla", fstream::out);
  fs << User::getDla().name << endl
     << User::getDla().gender << endl;
  fs.close();
  
  vector<string> modules;
  User::selectModules(modules);
  fs.open(User::getLoc() + "data/modules", fstream::out);
  for (size_t i = 0; i < modules.size(); i++)
    fs << modules[i] << endl;
  fs.close();

  User::populateSettings();

  Contact info;
  info.setUsername(User::getUsername());
  info.setId(id);
  info.setDla(User::getDla());
  info.edit();
  info.save(ss);
  if (userList_.size() == 0)
    User::admin = true;
  else
    User::admin = false;
  ss << User::admin << endl;
  User::printUserFileS("info", ss);

  vector<string> data;
  data.push_back("0");
  User::printUserFileS("contacts", data);
  User::printUserFileS("logins", data);

  encryptFile(User::getLoc() + "log.log", password);
  fs.open(User::getLoc() + "log.crypt", fstream::out);
  fs << "1";
  fs.close();

  User::registerUser(hash, iterations);

  fs.open("data/users", fstream::app);
  fs << username << endl;
  fs.close();
}

void User::deleteUser()
{
  int a = 0;
  string tmpStr;
  
  while (INKEY()) {
    DELAY_LOOP;
  }
  cls(hStdout_);
  cout << "Are you sure you want to permanently delete the account [" << User::getUsername() << "]" << endl
       << "This action cannot easily be undone!" << endl << endl
       << "0 - Cancel" << endl
       << "1 - Yes, I understand. Delete this account." << endl
       << ": ";
  getline(cin, tmpStr);
  a = val(tmpStr);

  if (a == 1) {
    User::backupUser();

    if (deleteFolder(loc_ + "data/user/" + User::getUsername())) {
      vector<string> users;
      loadFileS("data/users", users);
      for (size_t i = 0; i < users.size(); i++) {
        if (uCase(users[i]) == uCase(User::getUsername()))
          users.erase(users.begin() + i);
      }
      printFileS("data/users", users);
    }

  }
}


void User::populateUserDirectory()
{
  makeFolder(User::getLoc());
  makeFolder(User::getLoc() + "backup");
  makeFolder(User::getLoc() + "backup/input");
  makeFolder(User::getLoc() + "backup/module");
  makeFolder(User::getLoc() + "backup/output");
  makeFolder(User::getLoc() + "data");
  makeFolder(User::getLoc() + "data/input");
  makeFolder(User::getLoc() + "data/module");
  makeFolder(User::getLoc() + "data/output");
  makeFolder(User::getLoc() + "data/setting");
  makeFolder(User::getLoc() + "data/setting/input");
  makeFolder(User::getLoc() + "data/setting/module");
  makeFolder(User::getLoc() + "data/setting/output");
  makeFolder(User::getLoc() + "resource");
  makeFolder(User::getLoc() + "resource/input");
  makeFolder(User::getLoc() + "resource/module");
  makeFolder(User::getLoc() + "resource/output");

  makeFile(User::getLoc() + "log.log");
  makeFile(User::getLoc() + "log.crypt");
  makeFile(User::getLoc() + "contacts");
  makeFile(User::getLoc() + "dla");
  makeFile(User::getLoc() + "id");
  makeFile(User::getLoc() + "info");
  makeFile(User::getLoc() + "logins");
  makeFile(User::getLoc() + "data/input/ptrs");
  makeFile(User::getLoc() + "data/module/ptrs");
  makeFile(User::getLoc() + "data/output/ptrs");
  
  copyFile("data/backup/defaultinputs",  User::getLoc() + "data/inputs");
  copyFile("data/backup/defaultmodules", User::getLoc() + "data/modules");
  copyFile("data/backup/defaultoutputs", User::getLoc() + "data/outputs");
}

void User::populateSettings()
{
  copyFile("data/setting/settings.settings", User::getLoc() + "data/setting/settings.settings");
  
  vector<string> modules;
  loadFileS(User::getLoc() + "data/modules", modules);
  for (size_t i = 0; i < modules.size(); i++)
    copyFile("data/setting/module/" + lCase(modules[i]) + ".settings", User::getLoc() + "data/setting/module/" + lCase(modules[i]) + ".settings");

  vector<string> inputs;
  loadFileS(User::getLoc() + "data/inputs", inputs);
  for (size_t i = 0; i < inputs.size(); i++)
    copyFile("data/setting/input/" + lCase(inputs[i]) + ".settings", User::getLoc() + "data/setting/input/" + lCase(inputs[i]) + ".settings");

  vector<string> outputs;
  loadFileS(User::getLoc() + "data/outputs", outputs);
  for (size_t i = 0; i < outputs.size(); i++)
    copyFile("data/setting/output/" + lCase(outputs[i]) + ".settings", User::getLoc() + "data/setting/output/" + lCase(outputs[i]) + ".settings");
}


bool User::authenticateUser()
{
  return verifyPassword(User::password, User::getLoc() + "pass");
}

bool User::verifyUsername()
{
  if (User::getUsername() == "")
    return false;
  
  //test against server

  vector<string> users;
  loadFileS("data/users", users);
  for (size_t i = 0; i < users.size(); i++) {
    if (uCase(users[i]) == uCase(User::getUsername()))
      return false;
  }

  return true;
}

void User::registerUser(std::string hash, int iterations)
{
  string username = User::getUsername();
  string id = User::getId();

  //send username / id to server

  User::registerPassword(hash, iterations);
}

void User::registerPassword(std::string hash, int iterations)
{
  //send hash / iterations to server
}


void User::createDla ()
{
  Contact::DLA dla;
  dla.name = "NULL";
  dla.gender = GENDER_DEFAULT;

  string tmpStr = "";
  string tmpName = "";
  int tmpInt = 0;
  int a = 0;
  int b = 0;

  do {
    cls(hStdout_);
    cout << "Now you will create your specific DLA." << endl << endl
         << "Would you like me to be male or female?" << endl
         << "1 - Male" << endl
         << "2 - Female" << endl << endl
         << ": ";
    getline(cin, tmpStr);
    tmpInt = val(tmpStr);
    switch (tmpInt) {
      case 1:
        dla.gender = GENDER_MALE;
        break;
      case 2:
        dla.gender = GENDER_FEMALE;
        break;
    }
  } while (dla.gender == GENDER_DEFAULT);

  switch (dla.gender) {
    case 1:
      tmpName = names_boy_[rand() % (int)names_boy_.size()];
      break;
    case 2:
      tmpName = names_girl_[rand() % (int)names_girl_.size()];
      break;
  }

  do {
    cls(hStdout_);
    cout << "Now you will create your specific DLA." << endl << endl
         << "I have chosen the name " << tmpName << "." << endl
         << "Are you ok with that name?" << endl
         << "0 - Yes, your name will be " << tmpName << endl
         << "1 - No, select another name" << endl
         << "2 - No, let me enter a name for you" << endl
         << ": ";
    getline(cin, tmpStr);
    a = val(tmpStr);

    switch (a) {
      case 0:
        dla.name = tmpName;
        break;
      case 1:
        switch (dla.gender) {
          case 1:
            tmpName = names_boy_[rand() % (int)names_boy_.size()];
            break;
          case 2:
            tmpName = names_girl_[rand() % (int)names_girl_.size()];
            break;
        }
        break;
      case 2:
        do {
          cls(hStdout_);
          cout << "Enter a name for me. Enter '' to return." << endl
               << ": ";
          getline(cin, tmpStr);
          if (tmpStr == "")
            break;
          tmpName = tmpStr;
          cls(hStdout_);
          cout << "Are you sure you want my name to be " << tmpName << "?" << endl
            << "0 - Yes, your name will be " << tmpName << endl
            << "1 - No, let me enter another name" << endl
            << "2 - No, return" << endl
            << ": ";
          cin >> b;
          getline(cin, tmpStr);
          if (b == 2)
            break;
        } while (b > 0);
        if (b == 2)
          break;
        dla.name = tmpName;
        break;
    }
  } while (uCase(dla.name) == "NULL");

  User::setDla(dla);
}

void User::selectModules(vector<string>& modules)
{
  vector<string> availableModules;
  loadFileS("data/modules", availableModules);

  modules.clear();
  loadFileS("data/modules", modules);
  
  vector<string> moduleOptions;
  string tmpStr = "";
  bool has = false;
  int a = 0;
  int b = 0;

  do {
    cls(hStdout_);
    cout << "Now you will select which modules to add to your DLA." << endl
         << "This is for advanced users, we recommend you just enter '0' to select all modules." << endl << endl;
    cout << "0 - Done selecting modules" << endl
         << "1 - View Selected Modules" << endl
         << "2 - Select a Module" << endl
         << "3 - Deselect a Module" << endl
         << "4 - Select all Modules" << endl
         << "5 - Deselect all Modules" << endl
         << ": ";
    getline(cin, tmpStr);
    a = val(tmpStr);
    switch (a) {
      case 0:
        break;
      case 1:
        cls(hStdout_);
        cout << "Modules you have selected:" << endl;
        for (size_t i = 0; i < modules.size(); i++)
          cout << "  " << modules[i] << endl;
        system("PAUSE");
        break;
      case 2:
        do { 
          moduleOptions.clear();
          for (size_t i = 0; i < availableModules.size(); i++) {
            has = false;
            for (size_t j = 0; j < modules.size(); j++) {
              if (uCase(modules[j]) == uCase(availableModules[i]))
                has = true;
            }
            if (!has)
              moduleOptions.push_back(availableModules[i]);
          }
          cls(hStdout_);
          cout << "Available Modules:" << endl;
          if (moduleOptions.size() == 0) {
            cout << "All modules are already selected, enter '0'." << endl;
          }
          else {
            for (size_t i = 0; i < moduleOptions.size(); i++)
              cout << "[" << i + 1 << "] - " << moduleOptions[i] << endl;
          }
          cout << endl
               << "0 - Done Selecting Modules" << endl
               << "X - Select Module [X]" << endl;
          cin >> b;
          getline(cin, tmpStr);
          switch (b) {
            case 0:
              break;
            default:
              if (inRange(b, 0, (int)moduleOptions.size() + 1, false, false)) {
                modules.push_back(moduleOptions[b - 1]);
                cls(hStdout_);
                cout << "The " << moduleOptions[b - 1] << " module has been selected." << endl;
                system("PAUSE");
              }
          }
        } while (b > 0);
        break;
      case 3:
        do {
          cls(hStdout_);
          cout << "Selected Modules:" << endl;
          if (modules.size() == 0) {
            cout << "All modules are already deselected, enter '0'." << endl;
          }
          else {
            for (size_t i = 0; i < modules.size(); i++)
              cout << "[" << i + 1 << "] - " << modules[i] << endl;
          }
          cout << endl
               << "0 - Done Deselecting Modules" << endl
               << "X - Deselect Module [X]" << endl;
          cin >> b;
          getline(cin, tmpStr);
          switch (b) {
            case 0:
              break;
            default:
              if (inRange(b, 0, (int)modules.size() + 1, false, false)) {
                tmpStr = modules[b - 1];
                modules.erase(modules.begin() + b - 1);
                cls(hStdout_);
                cout << "The " << tmpStr << " module has been deselected." << endl;
                system("PAUSE");
              }
          }
        } while (b > 0);
        break;
      case 4:
        modules.clear();
        for (size_t i = 0; i < availableModules.size(); i++)
          modules.push_back(availableModules[i]);
        cls(hStdout_);
        cout << "All modules have been selected." << endl;
        system("PAUSE");
        break;
      case 5:
        modules.clear();
        cls(hStdout_);
        cout << "All modules have been deselected." << endl;
        system("PAUSE");
        break;
    }
  } while (a > 0);
}


void User::loadUserFileS(string file, vector<string>& data, int& dataLen)
{
  file = User::getLoc() + file;
  
  if (!fileExists(file) || fileSize(file) < 1)
    return;

  loadEncFileS(file, User::password, data, dataLen);
}
void User::loadUserFileS(string file, ostream& ss)
{
  vector<string> data;
  int dataLen = 0;
  User::loadUserFileS(file, data, dataLen);

  for (int i = 0; i < dataLen; i++)
    ss << data[i] << endl;
}

void User::printUserFileS(string file, vector<string>& data)// const
{
  file = User::getLoc() + file;

  if (!fileExists(file))
    makeFile(file);

  printEncFileS(file, User::password, data);
}
void User::printUserFileS(string file, istream& ss)// const
{
  string tmpStr = "";
  vector<string> data;
  while (getline(ss, tmpStr))
    data.push_back(tmpStr);

  User::printUserFileS(file, data);
}


bool User::isEncrypted(string file) const
{
  int result;
  fstream fs;
  fs.open(User::getLoc() + file + ".crypt");
  getint(fs, result);
  if (result == 1)
    return true;
  return false;
}

bool User::isDecrypted(string file) const
{
  int result;
  fstream fs;
  fs.open(User::getLoc() + file + ".crypt");
  getint(fs, result);
  if (result == 0)
    return true;
  return false;
}


//functions
string getAlias()
{
  if (user_.getInfo().getName().nickName > "")
    return user_.getInfo().getName().nickName;
  else
    return user_.getInfo().name_toString();
}

bool simpleLogin(User& user)
{
  bool succeeded = user.login_simple();

  if (succeeded)
    user.loadUser_simple();

  user.setPassword("");
  return succeeded;
}