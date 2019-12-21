////////////////////////////////////////////////////////////////////////////////
//File:   contact.cpp
//Dir:    source/
//Date:   2014-12-30
//Author: Zachary Gill
//Implementation of Contact
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
#include <vector>


//includes
#include "contact.h"
#include "input.h"
#include "output.h"
#include "user.h"
#include "crypto.h"


//namespaces
using namespace std;


//Constructors

Contact::Contact()
{
  Contact::name = newName("", "", "", "", "", "", "");
  Contact::dla = newDla("NULL", GENDER_DEFAULT);
  Contact::username = "";
  Contact::id = "";
  Contact::gender = GENDER_DEFAULT;
  Contact::birthday = "";
}
Contact::Contact(string file)
{
  fstream fs;
  fs.open(file);
  readContactFromStream(fs);
  fs.close();
}
Contact::Contact(istream& is)
{
  readContactFromStream(is);
}


Contact::Name Contact::newName(string prefix, string first, string middle, string middleInitial, string last, string suffix, string nickName)
{
  Contact::Name temp;
  temp.prefix = prefix;
  temp.first = first;
  temp.middle = middle;
  temp.middleInitial = middleInitial;
  temp.last = last;
  temp.suffix = suffix;
  temp.nickName = nickName;
  return temp;
}
Contact::Name Contact::newName(istream& is)
{
  Contact::Name temp;
  getline(is, temp.prefix);
  getline(is, temp.first);
  getline(is, temp.middle);
  getline(is, temp.middleInitial);
  getline(is, temp.last);
  getline(is, temp.suffix);
  getline(is, temp.nickName);
  return temp;
}

Contact::DLA Contact::newDla(string name, int gender)
{
  Contact::DLA temp;
  temp.name = name;
  temp.gender = gender;
  return temp;
}
Contact::DLA Contact::newDla(istream& is)
{
  Contact::DLA temp;
  getline(is, temp.name);
  getint (is, temp.gender);
  return temp;
}

Contact::Phone Contact::newPhone(int type, int country, int areaCode, int number, int ext)
{
  Contact::Phone temp;
  temp.type = type;
  temp.country = country;
  temp.areaCode = areaCode;
  temp.number = number;
  temp.ext = ext;
  return temp;
}
Contact::Phone Contact::newPhone(istream& is)
{
  Contact::Phone temp;
  getint(is, temp.type);
  getint(is, temp.country);
  getint(is, temp.areaCode);
  getint(is, temp.number);
  getint(is, temp.ext);
  return temp;
}

Contact::Email Contact::newEmail(int type, string address)
{
  Contact::Email temp;
  temp.type = type;
  temp.address = address;
  return temp;
}
Contact::Email Contact::newEmail(istream& is)
{
  Contact::Email temp;
  getint (is, temp.type);
  getline(is, temp.address);
  return temp;
}

Contact::Address Contact::newAddress(int type, string street, string city, string state, int zip, string appt, int poBox)
{
  Contact::Address temp;
  temp.type = type;
  temp.street = street;
  temp.city = city;
  temp.state = state;
  temp.zip = zip;
  temp.appt = appt;
  temp.poBox = poBox;
  return temp;
}
Contact::Address Contact::newAddress(istream& is)
{
  Contact::Address temp;
  getint (is, temp.type);
  getline(is, temp.street);
  getline(is, temp.city);
  getline(is, temp.state);
  getint (is, temp.zip);
  getline(is, temp.appt);
  getint (is, temp.poBox);
  return temp;
}

Contact::Account Contact::newAccount(string account, string username, string address)
{
  Contact::Account temp;
  temp.account = account;
  temp.username = username;
  temp.address = address;
  return temp;
}
Contact::Account Contact::newAccount(istream& is)
{
  Contact::Account temp;
  getline(is, temp.account);
  getline(is, temp.username);
  getline(is, temp.address);
  return temp;
}


//Destructors

Contact::~Contact()
{
}


//Accessors

Contact::Name Contact::getName() const
{
  return Contact::name;
}

Contact::DLA Contact::getDla() const
{
  return Contact::dla;
}

string Contact::getUsername() const
{
  return Contact::username;
}

string Contact::getId() const
{
  return Contact::id;
}

int Contact::getGender() const
{
  return Contact::gender;
}

string Contact::getBirthday() const
{
  return Contact::birthday;
}

Contact::Phone Contact::getPhone(int element)
{
  if (element < 0 || element > Contact::phoneCount() - 1)
    return Contact::newPhone(PHONE_TYPE_OTHER, NULL, NULL, NULL, NULL);
    
  return Contact::phone[element];
}

Contact::Email Contact::getEmail(int element)
{
  if (element < 0 || element > Contact::emailCount() - 1)
    return Contact::newEmail(EMAIL_TYPE_OTHER, "");

  return Contact::email[element];
}

Contact::Address Contact::getAddress(int element)
{
  if (element < 0 || element > Contact::addressCount() - 1)
    return Contact::newAddress(ADDRESS_TYPE_OTHER, "", "", "", NULL, "", NULL);

  return Contact::address[element];
}

string Contact::getAccount(string account)
{
  for (int i = 0; i < Contact::accountCount(); i++)
    if (uCase(Contact::account[i].account) == uCase(account))
      return Contact::account[i].username;
  return "";
}


//Modifiers

void Contact::setName(Contact::Name name)
{
  Contact::name = name;
}

void Contact::setDla(Contact::DLA dla)
{
  Contact::dla = dla;
}

void Contact::setUsername(string username)
{
  Contact::username = username;
}

void Contact::setId(string id)
{
  Contact::id = id;
}

void Contact::setGender(int gender)
{
  Contact::gender = gender;
}

void Contact::setBirthday(string birthday)
{
  Contact::birthday = birthday;
}

void Contact::addPhone(Contact::Phone phone)
{
  Contact::phone.push_back(phone);
}

void Contact::removePhone(int element)
{
  if (element > -1 && element < Contact::phoneCount())
    Contact::phone.erase(Contact::phone.begin() + element);
}

void Contact::movePhone(int initElement, int newElement)
{
  if ((initElement > -1 && initElement < Contact::phoneCount()) && (newElement > -1 && newElement < Contact::phoneCount())) {
    Contact::Phone tmp = Contact::phone[initElement];
    Contact::phone.erase(Contact::phone.begin() + initElement);
    Contact::phone.insert(Contact::phone.begin() + newElement, tmp);
  }
}

void Contact::addEmail(Contact::Email email)
{
  Contact::email.push_back(email);
}

void Contact::removeEmail(int element)
{
  if (element > -1 && element < Contact::emailCount())
    Contact::email.erase(Contact::email.begin() + element);
}

void Contact::moveEmail(int initElement, int newElement)
{
  if ((initElement > -1 && initElement < Contact::emailCount()) && (newElement > -1 && newElement < Contact::emailCount())) {
    Contact::Email tmp = Contact::email[initElement];
    Contact::email.erase(Contact::email.begin() + initElement);
    Contact::email.insert(Contact::email.begin() + newElement, tmp);
  }
}

void Contact::addAddress(Contact::Address address)
{
  Contact::address.push_back(address);
}

void Contact::removeAddress(int element)
{
  if (element > -1 && element < Contact::addressCount())
    Contact::address.erase(Contact::address.begin() + element);
}

void Contact::moveAddress(int initElement, int newElement)
{
  if ((initElement > -1 && initElement < Contact::addressCount()) && (newElement > -1 && newElement < Contact::addressCount())) {
    Contact::Address tmp = Contact::address[initElement];
    Contact::address.erase(Contact::address.begin() + initElement);
    Contact::address.insert(Contact::address.begin() + newElement, tmp);
  }
}

void Contact::addAccount(Contact::Account account)
{
  Contact::account.push_back(account);
}

void Contact::removeAccount(int element)
{
  if (element > -1 && element < Contact::accountCount())
    Contact::account.erase(Contact::account.begin() + element);
}

void Contact::moveAccount(int initElement, int newElement)
{
  if ((initElement > -1 && initElement < Contact::accountCount()) && (newElement > -1 && newElement < Contact::accountCount())) {
    Contact::Account tmp = Contact::account[initElement];
    Contact::account.erase(Contact::account.begin() + initElement);
    Contact::account.insert(Contact::account.begin() + newElement, tmp);
  }
}


//Other Functions

string Contact::name_toString() const
{
  string name = "";

  if (Contact::name.first > "") {
    if (Contact::name.prefix > "")
      name += Contact::name.prefix + " ";
    name += Contact::name.first;
    if (Contact::name.last > "") {
      if (Contact::name.middle > "")
        name += " " + Contact::name.middle;
      else if (Contact::name.middleInitial > "")
        name += " " + Contact::name.middleInitial + ".";
      name += " " + Contact::name.last;
      if (Contact::name.suffix > "")
        name += " " + Contact::name.suffix;
    }
  }
  else if (Contact::name.nickName > "") {
    name = Contact::name.nickName;
  }
  else if (Contact::username > "") {
    name = Contact::username;
  }

  if (name.length() > 0) {
    if (name.substr(name.length() - 1) == " ")
      name = name.substr(0, name.length() - 1);
  }

  return name;
}

string Contact::dla_toString() const
{
  string dla = "";

  if (Contact::dla.name > "") {
    dla += Contact::dla.name;

    if (Contact::dlaIsMale())
      dla += " (M)";
    else if (Contact::dlaIsFemale())
      dla += " (F)";
  }

  return dla;
}

string Contact::gender_toString() const
{
  if (Contact::isMale())
    return "Male";
  if (Contact::isFemale())
    return "Female";
  return "";
}

string Contact::birthday_toString() const
{
  return Contact::birthday;
}

string Contact::phone_toString() const
{
  return Contact::phone_toString(0);
}
string Contact::phone_toString(int element) const
{
  if (element < 0 || element > Contact::phoneCount() - 1)
    return "";

  string phone = "";

  if (Contact::phone[element].areaCode > 0) {
    if (Contact::phone[element].country > 0) {
      phone += "+" + str(Contact::phone[element].country) + " ";
    }
    phone += "(" + zeroset(str(Contact::phone[element].areaCode), 3) + ") ";
  }
  string number = zeroset(str(Contact::phone[element].number), 7);
  phone += number.substr(0, 3) + "-" + number.substr(3);

  if (Contact::phone[element].ext > 0)
    phone += " X" + str(Contact::phone[element].ext);

  return phone;
}

string Contact::email_toString() const
{
  return Contact::email_toString(0);
}
string Contact::email_toString(int element) const
{
  if (element < 0 || element > Contact::emailCount() - 1)
    return "";

  return Contact::email[element].address;
}

string Contact::address_toString() const
{
  return Contact::address_toString(0);
}
string Contact::address_toString(int element) const
{
  if (element < 0 || element > Contact::addressCount() - 1)
    return "";

  string address = "";

  if (Contact::address[element].street > "") {
    address += Contact::address[element].street;
    if (Contact::address[element].appt > "")
      address += " apt # " + Contact::address[element].appt;
  }
  if (Contact::address[element].poBox > 0)
    address += " P.O. Box " + str(Contact::address[element].poBox);

  if (Contact::address[element].city > "")
    address += ", " + Contact::address[element].city;
  if (Contact::address[element].state > "")
    address += ", " + Contact::address[element].state + " ";
  if (Contact::address[element].zip > 0)
    address += " " + str(Contact::address[element].zip);

  return address;
}

string Contact::account_toString(string account) const
{
  return Contact::account_toString(Contact::findAccount(account));
}
string Contact::account_toString(int element) const
{
  if (element < 0 || element > Contact::accountCount() - 1)
    return "";

  string account = "";

  account += Contact::account[element].account + ": ";
  account += Contact::account[element].username;

  return account;
}


int Contact::findPhone(int type) const
{
  for (int i = 0; i < Contact::phoneCount(); i++) {
    if (Contact::phone[i].type == type)
      return i;
  }
  return -1;
}

int Contact::findEmail(int type) const
{
  for (int i = 0; i < Contact::emailCount(); i++) {
    if (Contact::email[i].type == type)
      return i;
  }
  return -1;
}

int Contact::findAddress(int type) const
{
  for (int i = 0; i < Contact::addressCount(); i++) {
    if (Contact::address[i].type == type)
      return i;
  }
  return -1;
}

int Contact::findAccount(string account) const
{
  for (int i = 0; i < Contact::accountCount(); i++)
  if (Contact::account[i].account == account)
    return i;
  return -1;
}


bool Contact::hasDla() const
{
  if (Contact::dla.name != "NULL")
    return true;
  return false;
}


int Contact::phoneCount() const
{
  return (int)Contact::phone.size();
}

int Contact::emailCount() const
{
  return (int)Contact::email.size();
}

int Contact::addressCount() const
{
  return (int)Contact::address.size();
}

int Contact::accountCount() const
{
  return (int)Contact::account.size();
}


void Contact::makeMainPhone(int element)
{
  if (element < 0 || element > Contact::phoneCount() - 1)
    return;

  Contact::movePhone(element, 0);
}

void Contact::makeMainEmail(int element)
{
  if (element < 0 || element > Contact::emailCount() - 1)
    return;

  Contact::moveEmail(element, 0);
}

void Contact::makeMainAddress(int element)
{
  if (element < 0 || element > Contact::addressCount() - 1)
    return;

  Contact::moveAccount(element, 0);
}

void Contact::makeMainAccount(int element)
{
  if (element < 0 || element > Contact::accountCount() - 1)
    return;

  Contact::moveAccount(element, 0);
}


bool Contact::hasAccount(string account) const
{
  for (int i = 0; i < Contact::accountCount(); i++) {
    if (uCase(Contact::account[i].account) == uCase(account))
      return true;
  }
  return false;
}


string Contact::phoneType(int element) const
{
  if (element < 0 || element > Contact::phoneCount() - 1)
    return "";

  switch (Contact::phone[element].type) {
    case PHONE_TYPE_HOME:
      return "Home";
    case PHONE_TYPE_CELL:
      return "Cell";
    case PHONE_TYPE_WORK:
      return "Work";
    case PHONE_TYPE_OTHER:
      return "Other";
    default:
      return "";
  }
}

string Contact::emailType(int element) const
{
  if (element < 0 || element > Contact::emailCount() - 1)
    return "";

  switch (Contact::email[element].type) {
    case EMAIL_TYPE_HOME:
      return "Home";
    case EMAIL_TYPE_WORK:
      return "Work";
    case EMAIL_TYPE_SCHOOL:
      return "Cell";
    case EMAIL_TYPE_OTHER:
      return "Other";
    default:
      return "";
  }
}

string Contact::addressType(int element) const
{
  if (element < 0 || element > Contact::addressCount() - 1)
    return "";

  switch (Contact::address[element].type) {
    case ADDRESS_TYPE_HOME:
      return "Home";
    case ADDRESS_TYPE_WORK:
      return "Work";
    case ADDRESS_TYPE_OTHER:
      return "Other";
    default:
      return "";
  }
}


string Contact::phoneTypeStr(int element) const
{
  if (element < 0 || element > Contact::phoneCount() - 1)
    return "";

  switch (Contact::phone[element].type) {
    case PHONE_TYPE_HOME:
      return "  Home : ";
    case PHONE_TYPE_CELL:
      return "  Cell : ";
    case PHONE_TYPE_WORK:
      return "  Work : ";
    case PHONE_TYPE_OTHER:
      return "  Other: ";
    default:
      return "         ";
  }
}

string Contact::emailTypeStr(int element) const
{
  if (element < 0 || element > Contact::emailCount() - 1)
    return "";

  switch (Contact::email[element].type) {
    case EMAIL_TYPE_HOME:
      return "  Home  : ";
    case EMAIL_TYPE_WORK:
      return "  Work  : ";
    case EMAIL_TYPE_SCHOOL:
      return "  School: ";
    case EMAIL_TYPE_OTHER:
      return "  Other : ";
    default:
      return "         ";
  }
}

string Contact::addressTypeStr(int element) const
{
  if (element < 0 || element > Contact::addressCount() - 1)
    return "";

  switch (Contact::address[element].type) {
    case ADDRESS_TYPE_HOME:
      return "  Home : ";
    case ADDRESS_TYPE_WORK:
      return "  Work : ";
    case ADDRESS_TYPE_OTHER:
      return "  Other: ";
    default:
      return "         ";
  }
}


bool Contact::isMale() const
{
  if (Contact::gender == GENDER_MALE)
    return true;
  return false;
}

bool Contact::isFemale() const
{
  if (Contact::gender == GENDER_FEMALE)
    return true;
  return false;
}

bool Contact::dlaIsMale() const
{
  if (Contact::dla.gender == GENDER_MALE)
    return true;
  return false;
}

bool Contact::dlaIsFemale() const
{
  if (Contact::dla.gender == GENDER_FEMALE)
    return true;
  return false;
}


void Contact::edit()
{
  pauseInputs();
  pauseOutputs();

  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
  int e = 0;
  
  int    tmpInt = 0;
  string tmpStr = "";

  do {
    cls(hStdout_);
    cout << "Contact:: " << endl;
    Contact::dump();
    cout << endl << endl
      << "0 - Done" << endl
      << "1 - Edit Name" << endl
      //<< "2 - Edit Username" << endl
      << "2 - Edit Gender" << endl
      << "3 - Edit Birthday" << endl
      << "4 - Edit Phones" << endl
      << "5 - Edit Emails" << endl
      << "6 - Edit Addresses" << endl
      << "7 - Edit Accounts" << endl << endl
      << ": ";
    getline(cin, tmpStr);
    a = val(tmpStr);
    switch (a) {
      case 0:
        break;
      case 1:
        do {
          cls(hStdout_);
          cout << "Contact::Name: " << Contact::name_toString()    << endl
               << "  Prefix:   "    << Contact::name.prefix        << endl
               << "  First:    "    << Contact::name.first         << endl
               << "  Middle:   "    << Contact::name.middle        << endl
               << "  MI:       "    << Contact::name.middleInitial << endl
               << "  Last:     "    << Contact::name.last          << endl
               << "  Suffix:   "    << Contact::name.suffix        << endl
               << "  Nickname: "    << Contact::name.nickName      << endl << endl;
          cout << "0 - Done"                                       << endl
               << "1 - Change Name Prefix"                         << endl
               << "2 - Change First Name"                          << endl
               << "3 - Change Middle Name"                         << endl
               << "4 - Change Middle Initial"                      << endl
               << "5 - Change Last Name"                           << endl
               << "6 - Change Name Suffix"                         << endl
               << "7 - Change Nick Name"                           << endl << endl
               << ": ";
          getline(cin, tmpStr);
          b = val(tmpStr);
          switch (b) {
            case 0:
              break;
            case 1:
              cls(hStdout_);
              cout << "Contact::Name::Prefix: " << Contact::name.prefix << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New Name Prefix" << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0")
                Contact::name.prefix = tmpStr;
              break;
            case 2:
              cls(hStdout_);
              cout << "Contact::Name::First: " << Contact::name.first << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New First Name" << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0")
                Contact::name.first = tmpStr;
              break;
            case 3:
              cls(hStdout_);
              cout << "Contact::Name::Middle: " << Contact::name.middle << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New Middle Name" << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0") {
                Contact::name.middle = tmpStr;
                if (Contact::name.middle > "")
                  Contact::name.middleInitial = uCase(Contact::name.middle.substr(0, 1));
              }
              break;
            case 4:
              if (Contact::name.middle > "")
                break;
              cls(hStdout_);
              cout << "Contact::Name::MI: " << Contact::name.middleInitial << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New Middle Initial" << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0")
                Contact::name.middleInitial = uCase(tmpStr.substr(0, 1));
              break;
            case 5:
              cls(hStdout_);
              cout << "Contact::Name::Last: " << Contact::name.last << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New Last Name" << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0")
                Contact::name.last = tmpStr;
              break;
            case 6:
              cls(hStdout_);
              cout << "Contact::Name::Suffix: " << Contact::name.suffix << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New Name Suffix " << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0")
                Contact::name.suffix = tmpStr;
              break;
            case 7:
              cls(hStdout_);
              cout << "Contact::Name::Nickname: " << Contact::name.nickName << endl << endl;
              cout << "0 - Return" << endl
                   << "* - Set New Nick Name" << endl << endl
                   << ": ";
              getline(cin, tmpStr);
              if (tmpStr != "0")
                Contact::name.nickName = tmpStr;
              break;
          }
        } while (b > 0);
        break;
      //case 2:
      //  cls(hStdout_);
      //  cout << "Contact::Username: " << Contact::username << endl << endl;
      //  cout << "0 - Return" << endl
      //       << "* - Set New Username" << endl << endl
      //       << ": ";
      //  getline(cin, tmpStr);
      //  if (tmpStr != "0")
      //    Contact::username = tmpStr;
      //  break;
      case 2:
        do {
          cls(hStdout_);
          cout << "Contact::Gender: " << Contact::gender_toString() << endl << endl;
          cout << "0 - NULL" << endl
               << "1 - Male" << endl
               << "2 - Female" << endl << endl
               << ": ";
          getline(cin, tmpStr);
          Contact::gender = val(tmpStr);
        } while (Contact::gender < 0 || Contact::gender > 2);
        break;
      case 3:
        b = 1;
        do {
          cls(hStdout_);
          cout << "Contact::Birthday: " << Contact::birthday_toString() << endl << endl;
          cout << "0          - Return"                                 << endl
               << "MM-DD-YYYY - Set Birthday"                           << endl << endl;
          getline(cin, tmpStr);
          if (tmpStr == "0" || tmpStr == "") {
            b = 0;
          }
          else {
            if (val(tmpStr.substr(0, 2)) > 0 && val(tmpStr.substr(0, 2)) < 13 &&
                val(tmpStr.substr(3, 2)) > 0 && val(tmpStr.substr(3, 2)) < daysInMonth(val(tmpStr.substr(0, 2)), val(tmpStr.substr(6))) + 1 &&
                val(tmpStr.substr(6)) > 0 && tmpStr.substr(6).length() == 4 &&
                tmpStr.substr(2, 1) == "-" && tmpStr.substr(5, 1) == "-") {
              Contact::birthday = tmpStr;
              b = 0;
            }
          }
        } while (b > 0);
        break;
      case 4:
        do {
          cls(hStdout_);
          cout << "Contact::Phone: " << endl;
          for (int i = 0; i < Contact::phoneCount(); i++)
            cout << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
          cout                          << endl
               << "0 - Done"            << endl
               << "1 - Edit Phones"     << endl
               << "2 - Add Phones"      << endl
               << "3 - Remove Phones"   << endl
               << "4 - Move Phones"     << endl
               << "5 - Swap Phones"     << endl
               << "6 - Organize Phones" << endl << endl
               << ": ";
          getline(cin, tmpStr);
          b = val(tmpStr);
          switch (b) {
            case 0:
              break;
            case 1:
              do {
                cls(hStdout_);
                cout << "Contact::Phone::Edit: " << endl;
                for (int i = 0; i < Contact::phoneCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                cout << endl
                  << "0 - Done" << endl
                  << "X - Edit Phone [X]" << endl << endl;
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::phoneCount() + 1) {
                      do {
                        cls(hStdout_);
                        cout << "Contact::Phone::Edit: " << Contact::phone_toString(c - 1) << endl
                             << "  Type:         " << Contact::phone[c - 1].type     << endl
                             << "  Country Code: " << Contact::phone[c - 1].country  << endl
                             << "  Area Code:    " << Contact::phone[c - 1].areaCode << endl
                             << "  Number:       " << Contact::phone[c - 1].number   << endl
                             << "  Extension:    " << Contact::phone[c - 1].ext      << endl << endl;
                        cout << "0 - Done"              << endl
                             << "1 - Edit Type"         << endl
                             << "2 - Edit Country Code" << endl
                             << "3 - Edit Area Code"    << endl
                             << "4 - Edit Number"       << endl
                             << "5 - Edit Extension"    << endl
                             << "6 - Make Main Phone"   << endl << endl
                             << ": ";
                        getline(cin, tmpStr);
                        d = val(tmpStr);
                        switch (d) {
                          case 0:
                            break;
                          case 1:
                            cls(hStdout_);
                            cout << "Contact::Phone::Edit::Type: " << Contact::phoneType(c - 1) << " (" << Contact::phone[c - 1].type << ")" << endl << endl;
                            cout << "0 - Return" << endl
                                 << "1 - Home" << endl
                                 << "2 - Cell" << endl
                                 << "3 - Work" << endl
                                 << "4 - Other" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            tmpInt = val(tmpStr);
                            switch (tmpInt) {
                              case 0:
                                break;
                              default:
                                if (tmpInt > 0 && tmpInt < 5)
                                  Contact::phone[c - 1].type = tmpInt;
                                break;
                            }
                            break;
                          case 2:
                            cls(hStdout_);
                            cout << "Contact::Phone::Edit::CountryCode: " << "+" << str(Contact::phone[c - 1].country) << endl << endl;
                            cout << "0 - Return" << endl
                                 << "X - Set Country Code " << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            tmpInt = val(tmpStr);
                            if (tmpInt == 0)
                              break;
                            Contact::phone[c - 1].country = tmpInt;
                            break;
                          case 3:
                            e = 1;
                            do {
                              cls(hStdout_);
                              cout << "Contact::Phone::Edit::AreaCode: " << "(" << zeroset(str(Contact::phone[c - 1].areaCode), 3) << ")" << endl << endl;
                              cout << "0   - Return"         << endl
                                   << "XXX - Set Area Code " << endl << endl
                                   << ": ";
                              getline(cin, tmpStr);
                              if (tmpStr == "0" || tmpStr == "") {
                                break;
                                e = 0;
                              }
                              if (tmpStr.length() == 3) {
                                Contact::phone[c - 1].areaCode = val(tmpStr);
                                e = 0;
                              }
                            } while (e > 0);
                            break;
                          case 4:
                            e = 0;
                              do {
                              cls(hStdout_);
                              tmpStr = zeroset(str(Contact::phone[c - 1].number), 7);
                              cout << "Contact::Phone::Edit::Number: " << tmpStr.substr(0, 3) << "-" << tmpStr.substr(3) << endl << endl;
                              cout << "0       - Return" << endl
                                   << "XXXXXXX - Set Number " << endl << endl
                                   << ": ";
                              getline(cin, tmpStr);
                              if (tmpStr == "0" || tmpStr == "") {
                                break;
                                e = 0;
                              }
                              if (tmpStr.length() == 7) {
                                Contact::phone[c - 1].number = val(tmpStr);
                                e = 0;
                              }
                              } while (e > 0);
                            break;
                          case 5:
                            cls(hStdout_);
                            cout << "Contact::Phone::Edit::Extension: " << "X" << Contact::phone[c - 1].ext << endl << endl;
                            cout << "0 - Return" << endl
                                 << "X - Set Extension" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            tmpInt = val(tmpStr);
                            if (tmpInt == 0)
                              break;
                            Contact::phone[c - 1].ext = tmpInt;
                            break;
                          case 6:
                            Contact::makeMainPhone(c - 1);
                            c = 1;
                            break;
                        }
                      } while (d > 0);
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 2:
              do {
                cls(hStdout_);
                cout << "Contact::Phone::Add " << endl;
                for (int i = 0; i < Contact::phoneCount(); i++)
                  cout << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                cout                   << endl
                  << "0 - Done"        << endl
                  << "1 - Add A Phone" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    Contact::phone.push_back(Contact::newPhone(NULL, NULL, NULL, NULL, NULL));
                }
              } while (c > 0);
              break;
            case 3:
              do {
                cls(hStdout_);
                cout << "Contact::Phone::Remove " << endl;
                for (int i = 0; i < Contact::phoneCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                cout                        << endl
                  << "0 - Done"             << endl
                  << "X - Remove Phone [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::phoneCount() + 1)
                      Contact::phone.erase(Contact::phone.begin() + c - 1);
                    break;
                }
              } while (c > 0);
              break;
            case 4:
              do {
                cls(hStdout_);
                cout << "Contact::Phone::Move " << endl;
                for (int i = 0; i < Contact::phoneCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Move Phone [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::phoneCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Phone::Move " << endl;
                      for (int i = 0; i < Contact::phoneCount(); i++)
                        cout << "[" << i + 1 << "]" << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Move Phone [" << c << "] to Position [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::phoneCount() + 1)
                            Contact::movePhone(c - 1, d - 1);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 5:
              do {
                cls(hStdout_);
                cout << "Contact::Phone::Swap " << endl;
                for (int i = 0; i < Contact::phoneCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Swap Phone [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::phoneCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Phone::Swap " << endl;
                      for (int i = 0; i < Contact::phoneCount(); i++)
                        cout << "[" << i + 1 << "]" << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Swap Phone [" << c << "] with Phone [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::phoneCount() + 1)
                            swap(Contact::phone[c - 1], Contact::phone[d - 1]);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 6:
              do {
                cls(hStdout_);
                cout << "Contact::Phone::Organize " << endl;
                for (int i = 0; i < Contact::phoneCount(); i++)
                  cout << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
                cout                                  << endl
                  << "0 - Done"                       << endl
                  << "1 - Organize by Type"           << endl
                  << "1 - Organize by Type (Reverse)" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    for (int i = 0; i < Contact::phoneCount(); i++) {
                      for (int j = 0; j < Contact::phoneCount(); j++) {
                        if (Contact::phone[j].type > Contact::phone[i].type)
                          swap(Contact::phone[j], Contact::phone[i]);
                      }
                    }
                    break;
                  case 2:
                    for (int i = 0; i < Contact::phoneCount(); i++) {
                      for (int j = 0; j < Contact::phoneCount(); j++) {
                        if (Contact::phone[j].type < Contact::phone[i].type)
                          swap(Contact::phone[j], Contact::phone[i]);
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
          }
        } while (b > 0);
        break;
      case 5:
        do {
          cls(hStdout_);
          cout << "Contact::Email: " << endl;
          for (int i = 0; i < Contact::emailCount(); i++)
            cout << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
          cout                          << endl
               << "0 - Done"            << endl
               << "1 - Edit Emails"     << endl
               << "2 - Add Emails"      << endl
               << "3 - Remove Emails"   << endl
               << "4 - Move Emails"     << endl
               << "5 - Swap Emails"     << endl
               << "6 - Organize Emails" << endl << endl
               << ": ";
          getline(cin, tmpStr);
          b = val(tmpStr);
          switch (b) {
            case 0:
              break;
            case 1:
              do {
                cls(hStdout_);
                cout << "Contact::Email::Edit: " << endl;
                for (int i = 0; i < Contact::emailCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                cout << endl
                  << "0 - Done" << endl
                  << "X - Edit Email [X]" << endl << endl;
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::emailCount() + 1) {
                      do {
                        cls(hStdout_);
                        cout << "Contact::Email::Edit: " << Contact::email_toString(c - 1) << endl
                             << "  Type:    " << Contact::email[c - 1].type    << endl
                             << "  Address: " << Contact::email[c - 1].address << endl << endl;
                        cout << "0 - Done"            << endl
                             << "1 - Edit Type"       << endl
                             << "2 - Edit Address"    << endl
                             << "3 - Make Main Email" << endl << endl
                             << ": ";
                        getline(cin, tmpStr);
                        d = val(tmpStr);
                        switch (d) {
                          case 0:
                            break;
                          case 1:
                            cls(hStdout_);
                            cout << "Contact::Email::Edit::Type: " << Contact::emailType(c - 1) << " (" << Contact::email[c - 1].type << ")" << endl << endl;
                            cout << "0 - Return" << endl
                                 << "1 - Home"   << endl
                                 << "2 - Work"   << endl
                                 << "3 - School" << endl
                                 << "4 - Other"  << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            tmpInt = val(tmpStr);
                            switch (tmpInt) {
                              case 0:
                                break;
                              default:
                                if (tmpInt > 0 && tmpInt < 5)
                                  Contact::email[c - 1].type = tmpInt;
                                break;
                            }
                            break;
                          case 2:
                            cls(hStdout_);
                            cout << "Contact::Email::Edit::Address: " << Contact::email[c - 1].address << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New Address" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::email[c - 1].address = tmpStr;
                            break;
                          case 3:
                            Contact::makeMainEmail(c - 1);
                            c = 1;
                            break;
                        }
                      } while (d > 0);
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 2:
              do {
                cls(hStdout_);
                cout << "Contact::Email::Add " << endl;
                for (int i = 0; i < Contact::emailCount(); i++)
                  cout << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                cout                   << endl
                  << "0 - Done"        << endl
                  << "1 - Add An Email" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    Contact::email.push_back(Contact::newEmail(NULL, ""));
                }
              } while (c > 0);
              break;
            case 3:
              do {
                cls(hStdout_);
                cout << "Contact::Email::Remove " << endl;
                for (int i = 0; i < Contact::emailCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                cout                        << endl
                  << "0 - Done"             << endl
                  << "X - Remove Email [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::emailCount() + 1)
                      Contact::email.erase(Contact::email.begin() + c - 1);
                    break;
                }
              } while (c > 0);
              break;
            case 4:
              do {
                cls(hStdout_);
                cout << "Contact::Email::Move " << endl;
                for (int i = 0; i < Contact::emailCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Move Email [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::emailCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Email::Move " << endl;
                      for (int i = 0; i < Contact::emailCount(); i++)
                        cout << "[" << i + 1 << "]" << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Move Email [" << c << "] to Position [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::emailCount() + 1)
                            Contact::moveEmail(c - 1, d - 1);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 5:
              do {
                cls(hStdout_);
                cout << "Contact::Email::Swap " << endl;
                for (int i = 0; i < Contact::emailCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Swap Email [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::emailCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Email::Swap " << endl;
                      for (int i = 0; i < Contact::emailCount(); i++)
                        cout << "[" << i + 1 << "]" << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Swap Email [" << c << "] with Email [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::emailCount() + 1)
                            swap(Contact::email[c - 1], Contact::email[d - 1]);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 6:
              do {
                cls(hStdout_);
                cout << "Contact::Email::Organize " << endl;
                for (int i = 0; i < Contact::emailCount(); i++)
                  cout << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
                cout                                         << endl
                  << "0 - Done"                              << endl
                  << "1 - Organize by Type"                  << endl
                  << "2 - Organize by Type (Reverse)"        << endl
                  << "3 - Organize Alphabetically"           << endl
                  << "4 - Organize Alphabetically (Reverse)" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    for (int i = 0; i < Contact::emailCount(); i++) {
                      for (int j = 0; j < Contact::emailCount(); j++) {
                        if (Contact::email[j].type > Contact::email[i].type)
                          swap(Contact::email[j], Contact::email[i]);
                      }
                    }
                    break;
                  case 2:
                    for (int i = 0; i < Contact::emailCount(); i++) {
                      for (int j = 0; j < Contact::emailCount(); j++) {
                        if (Contact::email[j].type < Contact::email[i].type)
                          swap(Contact::email[j], Contact::email[i]);
                      }
                    }
                    break;
                  case 3:
                    for (int i = 0; i < Contact::emailCount(); i++) {
                      for (int j = 0; j < Contact::emailCount(); j++) {
                        if (Contact::email[j].address > Contact::email[i].address)
                          swap(Contact::email[j], Contact::email[i]);
                      }
                    }
                    break;
                  case 4:
                    for (int i = 0; i < Contact::emailCount(); i++) {
                      for (int j = 0; j < Contact::emailCount(); j++) {
                        if (Contact::email[j].address < Contact::email[i].address)
                          swap(Contact::email[j], Contact::email[i]);
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
          }
        } while (b > 0);
        break;
      case 6:
        do {
          cls(hStdout_);
          cout << "Contact::Address: " << endl;
          for (int i = 0; i < Contact::addressCount(); i++)
            cout << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
          cout                          << endl
               << "0 - Done"            << endl
               << "1 - Edit Addresss"     << endl
               << "2 - Add Addresss"      << endl
               << "3 - Remove Addresss"   << endl
               << "4 - Move Addresss"     << endl
               << "5 - Swap Addresss"     << endl
               << "6 - Organize Addresss" << endl << endl
               << ": ";
          getline(cin, tmpStr);
          b = val(tmpStr);
          switch (b) {
            case 0:
              break;
            case 1:
              do {
                cls(hStdout_);
                cout << "Contact::Address::Edit: " << endl;
                for (int i = 0; i < Contact::addressCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                cout << endl
                  << "0 - Done" << endl
                  << "X - Edit Address [X]" << endl << endl;
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::addressCount() + 1) {
                      do {
                        cls(hStdout_);
                        cout << "Contact::Address::Edit: " << Contact::address_toString(c - 1) << endl
                             << "  Type:       " << Contact::address[c - 1].type   << endl
                             << "  Street:     " << Contact::address[c - 1].street << endl
                             << "  City:       " << Contact::address[c - 1].city   << endl
                             << "  State:      " << Contact::address[c - 1].state  << endl
                             << "  Zip:        " << Contact::address[c - 1].zip    << endl
                             << "  Appartment: " << Contact::address[c - 1].appt   << endl
                             << "  PO Box:     " << Contact::address[c - 1].poBox  << endl << endl;
                        cout << "0 - Done"              << endl
                             << "1 - Edit Type"         << endl
                             << "2 - Edit Street"       << endl
                             << "3 - Edit City"         << endl
                             << "4 - Edit State"        << endl
                             << "5 - Edit Zip"          << endl
                             << "6 - Edit Appartment"   << endl
                             << "7 - Edit PO Box"       << endl
                             << "8 - Make Main Address" << endl << endl
                             << ": ";
                        getline(cin, tmpStr);
                        d = val(tmpStr);
                        switch (d) {
                          case 0:
                            break;
                          case 1:
                            cls(hStdout_);
                            cout << "Contact::Address::Edit::Type: " << Contact::addressType(c - 1) << " (" << Contact::address[c - 1].type << ")" << endl << endl;
                            cout << "0 - Return" << endl
                                 << "1 - Home" << endl
                                 << "2 - Work" << endl
                                 << "3 - Other" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            tmpInt = val(tmpStr);
                            switch (tmpInt) {
                              case 0:
                                break;
                              default:
                                if (tmpInt > 0 && tmpInt < 4)
                                  Contact::address[c - 1].type = tmpInt;
                                break;
                            }
                            break;
                          case 2:
                            cls(hStdout_);
                            cout << "Contact::Address::Edit::Street: " << Contact::address[c - 1].street << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New Street" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::address[c - 1].street = tmpStr;
                            break;
                          case 3:
                            cls(hStdout_);
                            cout << "Contact::Address::Edit::City: " << Contact::address[c - 1].city << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New City" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::address[c - 1].city = tmpStr;
                            break;
                          case 4:
                            cls(hStdout_);
                            cout << "Contact::Address::Edit::State: " << Contact::address[c - 1].state << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New State" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::address[c - 1].state = tmpStr;
                            break;
                          case 5:
                            e = 1;
                            do {
                              cls(hStdout_);
                              cout << "Contact::Address::Edit::Zip: " << zeroset(str(Contact::address[c - 1].zip), 5) << endl << endl;
                              cout << "0     - Return"         << endl
                                   << "XXXXX - Set Area Code " << endl << endl
                                   << ": ";
                              getline(cin, tmpStr);
                              if (tmpStr == "0" || tmpStr == "") {
                                break;
                                e = 0;
                              }
                              if (tmpStr.length() == 5) {
                                Contact::address[c - 1].zip = val(tmpStr);
                                e = 0;
                              }
                            } while (e > 0);
                            break;
                          case 6:
                            cls(hStdout_);
                            cout << "Contact::Address::Edit::Appartment: " << Contact::address[c - 1].appt << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New Appartment" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::address[c - 1].appt = tmpStr;
                            break;
                          case 7:
                            cls(hStdout_);
                            cout << "Contact::Address::Edit::POBox: " << zeroset(str(Contact::address[c - 1].zip), 5) << endl << endl;
                            cout << "0 - Return"      << endl
                                 << "X - Set PO Box " << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0" && tmpStr != "") {
                              Contact::address[c - 1].poBox = val(tmpStr);
                            }
                            break;
                          case 8:
                            Contact::makeMainAddress(c - 1);
                            c = 1;
                            break;
                        }
                      } while (d > 0);
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 2:
              do {
                cls(hStdout_);
                cout << "Contact::Address::Add " << endl;
                for (int i = 0; i < Contact::addressCount(); i++)
                  cout << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                cout                   << endl
                  << "0 - Done"        << endl
                  << "1 - Add An Address" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    Contact::address.push_back(Contact::newAddress(NULL, "", "", "", NULL, "", NULL));
                }
              } while (c > 0);
              break;
            case 3:
              do {
                cls(hStdout_);
                cout << "Contact::Address::Remove " << endl;
                for (int i = 0; i < Contact::addressCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                cout                        << endl
                  << "0 - Done"             << endl
                  << "X - Remove Address [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::addressCount() + 1)
                      Contact::address.erase(Contact::address.begin() + c - 1);
                    break;
                }
              } while (c > 0);
              break;
            case 4:
              do {
                cls(hStdout_);
                cout << "Contact::Address::Move " << endl;
                for (int i = 0; i < Contact::addressCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Move Address [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::addressCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Address::Move " << endl;
                      for (int i = 0; i < Contact::addressCount(); i++)
                        cout << "[" << i + 1 << "]" << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Move Address [" << c << "] to Position [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::addressCount() + 1)
                            Contact::moveAddress(c - 1, d - 1);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 5:
              do {
                cls(hStdout_);
                cout << "Contact::Address::Swap " << endl;
                for (int i = 0; i < Contact::addressCount(); i++)
                  cout << "[" << i + 1 << "]" << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Swap Address [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::addressCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Address::Swap " << endl;
                      for (int i = 0; i < Contact::addressCount(); i++)
                        cout << "[" << i + 1 << "]" << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Swap Address [" << c << "] with Address [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::addressCount() + 1)
                            swap(Contact::address[c - 1], Contact::address[d - 1]);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 6:
              do {
                cls(hStdout_);
                cout << "Contact::Address::Organize " << endl;
                for (int i = 0; i < Contact::addressCount(); i++)
                  cout << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
                cout                                  << endl
                  << "0 - Done"                       << endl
                  << "1 - Organize by Type"           << endl
                  << "2 - Organize by Type (Reverse)" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    for (int i = 0; i < Contact::addressCount(); i++) {
                      for (int j = 0; j < Contact::addressCount(); j++) {
                        if (Contact::address[j].type > Contact::address[i].type)
                          swap(Contact::address[j], Contact::address[i]);
                      }
                    }
                    break;
                  case 2:
                    for (int i = 0; i < Contact::addressCount(); i++) {
                      for (int j = 0; j < Contact::addressCount(); j++) {
                        if (Contact::address[j].type < Contact::address[i].type)
                          swap(Contact::address[j], Contact::address[i]);
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
          }
        } while (b > 0);
        break;
      case 7:
        do {
          cls(hStdout_);
          cout << "Contact::Account: " << endl;
          for (int i = 0; i < Contact::accountCount(); i++)
            cout << "  " << Contact::account_toString(i) << endl;
          cout                          << endl
               << "0 - Done"            << endl
               << "1 - Edit Accounts"     << endl
               << "2 - Add Accounts"      << endl
               << "3 - Remove Accounts"   << endl
               << "4 - Move Accounts"     << endl
               << "5 - Swap Accounts"     << endl
               << "6 - Organize Accounts" << endl << endl
               << ": ";
          getline(cin, tmpStr);
          b = val(tmpStr);
          switch (b) {
            case 0:
              break;
            case 1:
              do {
                cls(hStdout_);
                cout << "Contact::Account::Edit: " << endl;
                for (int i = 0; i < Contact::accountCount(); i++)
                  cout << "[" << i + 1 << "]  " << Contact::account_toString(i) << endl;
                cout << endl
                  << "0 - Done" << endl
                  << "X - Edit Account [X]" << endl << endl;
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::accountCount() + 1) {
                      do {
                        cls(hStdout_);
                        cout << "Contact::Account::Edit: " << Contact::account_toString(c - 1) << endl
                             << "  Account:  " << Contact::account[c - 1].account   << endl
                             << "  Username: " << Contact::account[c - 1].username << endl
                             << "  Address:  " << Contact::account[c - 1].address   << endl;
                        cout << "0 - Done"              << endl
                             << "1 - Edit Account"      << endl
                             << "2 - Edit Username"     << endl
                             << "3 - Edit Address"      << endl
                             << "4 - Make Main Account" << endl << endl
                             << ": ";
                        getline(cin, tmpStr);
                        d = val(tmpStr);
                        switch (d) {
                          case 0:
                            break;
                          case 1:
                            cls(hStdout_);
                            cout << "Contact::Account::Edit::Account: " << Contact::account[c - 1].account << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New Account" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::account[c - 1].account = tmpStr;
                            break;
                          case 2:
                            cls(hStdout_);
                            cout << "Contact::Account::Edit::Username: " << Contact::account[c - 1].username << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New Username" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::account[c - 1].username = tmpStr;
                            break;
                          case 3:
                            cls(hStdout_);
                            cout << "Contact::Account::Edit::Address: " << Contact::account[c - 1].address << endl << endl;
                            cout << "0 - Return" << endl
                                 << "* - Set New Address" << endl << endl
                                 << ": ";
                            getline(cin, tmpStr);
                            if (tmpStr != "0")
                              Contact::account[c - 1].address = tmpStr;
                            break;
                          case 4:
                            Contact::makeMainAccount(c - 1);
                            c = 1;
                            break;
                        }
                      } while (d > 0);
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 2:
              do {
                cls(hStdout_);
                cout << "Contact::Account::Add " << endl;
                for (int i = 0; i < Contact::accountCount(); i++)
                  cout << "  " << Contact::account_toString(i) << endl;
                cout                   << endl
                  << "0 - Done"        << endl
                  << "1 - Add An Account" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    Contact::account.push_back(Contact::newAccount("", "", ""));
                }
              } while (c > 0);
              break;
            case 3:
              do {
                cls(hStdout_);
                cout << "Contact::Account::Remove " << endl;
                for (int i = 0; i < Contact::accountCount(); i++)
                  cout << "[" << i + 1 << "]  " << Contact::account_toString(i) << endl;
                cout                        << endl
                  << "0 - Done"             << endl
                  << "X - Remove Account [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::accountCount() + 1)
                      Contact::account.erase(Contact::account.begin() + c - 1);
                    break;
                }
              } while (c > 0);
              break;
            case 4:
              do {
                cls(hStdout_);
                cout << "Contact::Account::Move " << endl;
                for (int i = 0; i < Contact::accountCount(); i++)
                  cout << "[" << i + 1 << "]  " << Contact::account_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Move Account [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::accountCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Account::Move " << endl;
                      for (int i = 0; i < Contact::accountCount(); i++)
                        cout << "[" << i + 1 << "]  " << Contact::account_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Move Account [" << c << "] to Position [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::accountCount() + 1)
                            Contact::moveAccount(c - 1, d - 1);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 5:
              do {
                cls(hStdout_);
                cout << "Contact::Account::Swap " << endl;
                for (int i = 0; i < Contact::accountCount(); i++)
                  cout << "[" << i + 1 << "]  " << Contact::account_toString(i) << endl;
                cout                      << endl
                  << "0 - Done"           << endl
                  << "X - Swap Account [X]" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  default:
                    if (c > 0 && c < Contact::accountCount() + 1) {
                      cls(hStdout_);
                      cout << "Contact::Account::Swap " << endl;
                      for (int i = 0; i < Contact::accountCount(); i++)
                        cout << "[" << i + 1 << "]  " << Contact::account_toString(i) << endl;
                      cout << endl
                        << "0 - Cancel" << endl
                        << "X - Swap Account [" << c << "] with Account [X]" << endl << endl
                        << ": ";
                      getline(cin, tmpStr);
                      d = val(tmpStr);
                      switch (d) {
                        case 0:
                          break;
                        default:
                          if (d > 0 && d < Contact::accountCount() + 1)
                            swap(Contact::account[c - 1], Contact::account[d - 1]);
                          break;
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
            case 6:
              do {
                cls(hStdout_);
                cout << "Contact::Account::Organize " << endl;
                for (int i = 0; i < Contact::accountCount(); i++)
                  cout << "  " << Contact::account_toString(i) << endl;
                cout                        << endl
                  << "0 - Done"             << endl
                  << "1 - Organize Alphabetically" << endl
                  << "2 - Organize Alphabetically (Reverse)" << endl << endl
                  << ": ";
                getline(cin, tmpStr);
                c = val(tmpStr);
                switch (c) {
                  case 0:
                    break;
                  case 1:
                    for (int i = 0; i < Contact::accountCount(); i++) {
                      for (int j = 0; j < Contact::accountCount(); j++) {
                        if (Contact::account[j].account > Contact::account[i].account) {
                          swap(Contact::account[j], Contact::account[i]);
                        }
                        else if (Contact::account[j].account == Contact::account[i].account) {
                          if (Contact::account[j].username > Contact::account[i].username)
                            swap(Contact::account[j], Contact::account[i]);
                        }
                      }
                    }
                    break;
                  case 2:
                    for (int i = 0; i < Contact::accountCount(); i++) {
                      for (int j = 0; j < Contact::accountCount(); j++) {
                        if (Contact::account[j].account < Contact::account[i].account) {
                          swap(Contact::account[j], Contact::account[i]);
                        }
                        else if (Contact::account[j].account == Contact::account[i].account) {
                          if (Contact::account[j].username < Contact::account[i].username)
                            swap(Contact::account[j], Contact::account[i]);
                        }
                      }
                    }
                    break;
                }
              } while (c > 0);
              break;
          }
        } while (b > 0);
        break;
    }
  } while (a > 0);

  startOutputs();
  startInputs();
}


void Contact::save(string file) const
{
  fstream fs;
  fs.open(file, fstream::out);
  Contact::save(fs);
  fs.close();
}
void Contact::save(ostream& os) const
{
  os << Contact::name.prefix        << endl
     << Contact::name.first         << endl
     << Contact::name.middle        << endl
     << Contact::name.middleInitial << endl
     << Contact::name.last          << endl
     << Contact::name.suffix        << endl
     << Contact::name.nickName      << endl
     << Contact::dla.name           << endl
     << Contact::dla.gender         << endl
     << Contact::username           << endl
     << Contact::id                 << endl
     << Contact::gender             << endl
     << Contact::birthday           << endl;

  for (int i = 0; i < Contact::phoneCount(); i++) {
    os << str(i)                     << endl
       << Contact::phone[i].type     << endl
       << Contact::phone[i].country  << endl
       << Contact::phone[i].areaCode << endl
       << Contact::phone[i].number   << endl
       << Contact::phone[i].ext      << endl;
  }
  os << "0" << endl;

  for (int i = 0; i < Contact::emailCount(); i++) {
    os << str(i)                    << endl
       << Contact::email[i].type    << endl
       << Contact::email[i].address << endl;
  }
  os << "0" << endl;

  for (int i = 0; i < Contact::addressCount(); i++) {
    os << str(i)                     << endl
       << Contact::address[i].type   << endl
       << Contact::address[i].street << endl
       << Contact::address[i].city   << endl
       << Contact::address[i].state  << endl
       << Contact::address[i].zip    << endl
       << Contact::address[i].appt   << endl
       << Contact::address[i].poBox  << endl;
  }
  os << "0" << endl;

  for (int i = 0; i < Contact::accountCount(); i++) {
    os << str(i)                       << endl
       << Contact::account[i].account  << endl
       << Contact::account[i].username << endl
       << Contact::account[i].address  << endl;
  }
  os << "0" << endl;
}


void Contact::dump() const
{
  Contact::dump(cout);
}
void Contact::dump(ostream& stream) const
{
  stream << "Name: "     << Contact::name_toString()     << endl
         << "DLA: "      << Contact::dla_toString()      << endl
         << "Username: " << Contact::username            << endl
         << "Id: "       << Contact::id                  << endl
         << "Gender: "   << Contact::gender_toString()   << endl
         << "Birthday: " << Contact::birthday_toString() << endl;

  stream << "Phones: " << endl;
  if (Contact::phoneCount() == 0) {
    stream << "  None" << endl;
  }
  else {
    for (int i = 0; i < Contact::phoneCount(); i++)
      stream << Contact::phoneTypeStr(i) << Contact::phone_toString(i) << endl;
  }

  stream << "Emails: " << endl;
  if (Contact::emailCount() == 0) {
    stream << "  None" << endl;
  }
  else {
    for (int i = 0; i < Contact::emailCount(); i++)
      stream << Contact::emailTypeStr(i) << Contact::email_toString(i) << endl;
  }

  stream << "Addresses: " << endl;
  if (Contact::addressCount() == 0) {
    stream << "  None" << endl;
  }
  else {
    for (int i = 0; i < Contact::addressCount(); i++)
      stream << Contact::addressTypeStr(i) << Contact::address_toString(i) << endl;
  }

  stream << "Accounts: " << endl;
  if (Contact::accountCount() == 0) {
    stream << "  None" << endl;
  }
  else {
    for (int i = 0; i < Contact::accountCount(); i++) {
      stream << "  " << Contact::account_toString(i) << endl;
    }
  }
}


//Private Functions

void Contact::readContactFromStream(istream& is)
{
  Contact::name = Contact::newName(is);
  Contact::dla = Contact::newDla(is);
  getline(is, Contact::username);
  getline(is, Contact::id);
  getint(is, Contact::gender);
  getline(is, Contact::birthday);

  int phoneCount = 0;
  int emailCount = 0;
  int addressCount = 0;
  int accountCount = 0;

  getint(is, phoneCount);
  while (phoneCount != 0) {
    Contact::phone.push_back(Contact::newPhone(is));
    getint(is, phoneCount);
  }

  getint(is, emailCount);
  while (emailCount != 0) {
    Contact::email.push_back(Contact::newEmail(is));
    getint(is, emailCount);
  }

  getint(is, addressCount);
  while (addressCount != 0) {
    Contact::address.push_back(Contact::newAddress(is));
    getint(is, addressCount);
  }

  getint(is, accountCount);
  while (accountCount != 0) {
    Contact::account.push_back(Contact::newAccount(is));
    getint(is, accountCount);
  }
}
