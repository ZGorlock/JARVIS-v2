////////////////////////////////////////////////////////////////////////////////
//File:   user.h
//Dir:    source/header/
//Date:   2015-01-05
//Author: Zachary Gill
//Interface of User
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_USER_H_
#define _DLA_SH_USER_H_


//constants
#include "const.h"

#define MAX_PASSWORD_LEN 128


//standard includes
#include <string>
#include <vector>


//includes
#include "contact.h"
#include "settings.h"


//class definitions
class User
{
  public: 
    struct Login
    {
      std::string username;
      std::string password;
      std::string pin;
      std::string website;
    };

    //Constructors
    User                              ();

    void        newLogin              (std::string, std::string, std::string, std::string);
    void        newLogin              (std::istream&);

    //Destructors
    ~User                             ();
    void        free                  ();

    //Accessors
    std::string  getUsername           () const;
    std::string  getId                 () const;
    bool         getAdmin              () const;
    Contact::DLA getDla                () const;
    std::string  getLoc                () const;
    Contact      getInfo               () const;
    Contact      getContact            (int) const;

    //Modifiers
    void         setUsername           (std::string);
    void         setId                 (std::string);
    void         setPassword           (std::string);
    void         setAdmin              (bool);
    void         setDla                (Contact::DLA);
    void         setLoc                (std::string);
    void         setInfo               (Contact);
    void         addContact            (Contact);
    void         addLogin              (Login);

    //Other Functions    
    void         loadUser              ();
    void         login                 ();

    bool         login_simple          ();
    void         loadUser_simple       ();

    void         backupUser            ();
    
    void         changePassword        ();
    void         resetPassword         ();

    void         loadUserInputs        () const;
    void         loadUserOutputs       () const;
    void         loadUserModules       () const;

    void         save                  ();
    void         dump                  () const;
    void         dump                  (std::ostream&) const;

  private:
    std::string              username;
    std::string              id;
    std::string              password;
    bool                     admin;

    Contact::DLA             dla;

    std::string              loc;

    Contact                  info;

    std::vector<Contact>     contacts;
    std::vector<User::Login> logins;

    Login getLogin              (int);
    
    void  createUser            ();
    void  deleteUser            ();

    void  populateUserDirectory ();
    void  populateSettings      ();

    bool  authenticateUser      ();
    bool  verifyUsername        ();
    void  registerUser          (std::string, int);
    void  registerPassword      (std::string, int);

    void  createDla             ();
    void  selectModules         (std::vector<std::string>&);

    void  loadUserFileS         (std::string, std::vector<std::string>&, int& dataLen);
    void  loadUserFileS         (std::string, std::ostream&);
    void  printUserFileS(std::string, std::vector<std::string>&);// const;
    void  printUserFileS(std::string, std::istream&);// const;

    bool  isEncrypted            (std::string) const;
    bool  isDecrypted            (std::string) const;
} ;


//functions
std::string getAlias    ();
bool        simpleLogin (User&);


//global variable definitions
extern std::string              name_;
extern int                      gender_;
extern HANDLE                   hStdout_;
extern std::string              loc_;
extern std::string              userLoc_;
extern bool                     showPassword_;
extern HWND                     hWnd_;
extern std::vector<std::string> userList_;
extern Settings                 settings_;
extern User                     user_;


//resources
extern std::vector<std::string> names_boy_;
extern std::vector<std::string> names_girl_;


//shared function definitions
#include "common.h"


#endif