////////////////////////////////////////////////////////////////////////////////
//File:   contact.h
//Dir:    source/header/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of Contact
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_CONTACT_H_
#define _DLA_SH_CONTACT_H_


//constants
#include "const.h"

#define PHONE_TYPE_HOME    1
#define PHONE_TYPE_CELL    2
#define PHONE_TYPE_WORK    3
#define PHONE_TYPE_OTHER   4

#define EMAIL_TYPE_HOME    1
#define EMAIL_TYPE_WORK    2
#define EMAIL_TYPE_SCHOOL  3
#define EMAIL_TYPE_OTHER   4

#define ADDRESS_TYPE_HOME  1
#define ADDRESS_TYPE_WORK  2
#define ADDRESS_TYPE_OTHER 3


//includes
#include "stdafx.h"
#include <fstream>
#include <string>
#include <vector>


//class definitions
class Contact
{
  public: 
    struct Name
    {
      std::string prefix;
      std::string first;
      std::string middle;
      std::string middleInitial;
      std::string last;
      std::string suffix;
      std::string nickName;
    };
    struct DLA
    {
      std::string name;
      int         gender;
    };
    struct Phone
    {
      int type;
      int country;
      int areaCode;
      int number;
      int ext;
    };
    struct Email
    {
      int         type;
      std::string address;
    };
    struct Address
    {
      int         type;
      std::string street;
      std::string city;
      std::string state;
      int         zip;
      std::string appt;
      int         poBox;
    };
    struct Account
    {
      std::string account;
      std::string username;
      std::string address;
    };

    //Constructors
    Contact                            ();
    Contact                            (std::string);
    Contact                            (std::istream&);

    Contact::Name    newName           (std::string, std::string, std::string, std::string, std::string, std::string, std::string);
    Contact::Name    newName           (std::istream&);
    Contact::DLA     newDla            (std::string, int);
    Contact::DLA     newDla            (std::istream&);
    Contact::Phone   newPhone          (int, int, int, int, int);
    Contact::Phone   newPhone          (std::istream&);
    Contact::Email   newEmail          (int, std::string);
    Contact::Email   newEmail          (std::istream&);
    Contact::Address newAddress        (int, std::string, std::string, std::string, int, std::string, int);
    Contact::Address newAddress        (std::istream&);
    Contact::Account newAccount        (std::string, std::string, std::string);
    Contact::Account newAccount        (std::istream&);

    //Destructors
    ~Contact                           ();

    //Accessors
    Contact::Name    getName           () const;
    Contact::DLA     getDla            () const;
    std::string      getUsername       () const;
    std::string      getId             () const;
    int              getGender         () const;
    std::string      getBirthday       () const;
    Contact::Phone   getPhone          (int);
    Contact::Email   getEmail          (int);
    Contact::Address getAddress        (int);
    std::string      getAccount        (std::string);

    //Modifiers
    void             setName           (Contact::Name);
    void             setDla            (Contact::DLA);
    void             setUsername       (std::string);
    void             setId             (std::string);
    void             setGender         (int);
    void             setBirthday       (std::string);
    void             addPhone          (Contact::Phone);
    void             removePhone       (int);
    void             movePhone         (int, int);
    void             addEmail          (Contact::Email);
    void             removeEmail       (int);
    void             moveEmail         (int, int);
    void             addAddress        (Contact::Address);
    void             removeAddress     (int);
    void             moveAddress       (int, int);
    void             addAccount        (Contact::Account);
    void             removeAccount     (int);
    void             moveAccount       (int, int);

    //Other Functions
    std::string      name_toString     () const;
    std::string      dla_toString      () const;
    std::string      gender_toString   () const;
    std::string      birthday_toString () const;
    std::string      phone_toString    () const;
    std::string      phone_toString    (int) const;
    std::string      email_toString    () const;
    std::string      email_toString    (int) const;
    std::string      address_toString  () const;
    std::string      address_toString  (int) const;
    std::string      account_toString  (std::string) const;
    std::string      account_toString  (int) const;

    int              findPhone         (int) const;
    int              findEmail         (int) const;
    int              findAddress       (int) const;
    int              findAccount       (std::string) const;

    bool             hasDla            () const;

    int              emailCount        () const;
    int              addressCount      () const;
    int              phoneCount        () const;
    int              accountCount      () const;

    void             makeMainPhone     (int);
    void             makeMainEmail     (int);
    void             makeMainAddress   (int);
    void             makeMainAccount   (int);

    bool             hasAccount        (std::string) const;

    std::string      phoneType(int) const;
    std::string      emailType(int) const;
    std::string      addressType(int) const;

    std::string      phoneTypeStr(int) const;
    std::string      emailTypeStr(int) const;
    std::string      addressTypeStr(int) const;

    bool             isMale            () const;
    bool             isFemale          () const;
    bool             dlaIsMale         () const;
    bool             dlaIsFemale       () const;

    void             edit              ();

    void             save              (std::string) const;
    void             save              (std::ostream&) const;

    void             dump              () const;
    void             dump              (std::ostream&) const;

  private: 
    Contact::Name                 name;
    Contact::DLA                  dla;

    std::string                   username;
    std::string                   id;

    int                           gender;
    std::string                   birthday;

    std::vector<Contact::Phone>   phone;
    std::vector<Contact::Email>   email;
    std::vector<Contact::Address> address;
    std::vector<Contact::Account> account;

    void readContactFromStream (std::istream&);
};


//global variable definitions
extern HANDLE hStdout_;


//function definitions
Contact makeContact ();
void    editContact ();


//shared function definitions
#include "common.h"


#endif