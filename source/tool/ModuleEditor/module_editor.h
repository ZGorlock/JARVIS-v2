////////////////////////////////////////////////////////////////////////////////
//File:   module_editor.h
//Dir:    *
//Date:   2015-02-16
//Author: Zachary Gill
//Interface of Module Editor
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_ME_H
#define _DLA_ME_H


//constants
#define COMMAND_TYPE_NULL      0
#define COMMAND_TYPE_COMMAND   1
#define COMMAND_TYPE_QUESTION  2
#define COMMAND_TYPE_STATEMENT 3

#define QUESTION_TYPE_NULL    0
#define QUESTION_TYPE_BASIC   1
#define QUESTION_TYPE_COMPLEX 2

#define RETURN_TYPE_DECLTYPE   0
#define RETURN_TYPE_VOID       1
#define RETURN_TYPE_BOOL       2
#define RETURN_TYPE_SCHAR      3
#define RETURN_TYPE_UCHAR      4
#define RETURN_TYPE_SHORT      5
#define RETURN_TYPE_USHORT     6
#define RETURN_TYPE_INT        7
#define RETURN_TYPE_UINT       8
#define RETURN_TYPE_LONG       9
#define RETURN_TYPE_ULONG      10
#define RETURN_TYPE_LONGLONG   11
#define RETURN_TYPE_ULONGLONG  12
#define RETURN_TYPE_FLOAT      13
#define RETURN_TYPE_DOUBLE     14
#define RETURN_TYPE_LONGDOUBLE 15
#define RETURN_TYPE_CHAR       16
#define RETURN_TYPE_CHAR16_T   17
#define RETURN_TYPE_CHAR32_T   18
#define RETURN_TYPE_WCHAR_T    19
#define RETURN_TYPE_STRING     20
#define RETURN_TYPE_UDEF       21

#define GENDER_DEFAULT 0
#define GENDER_MALE    1
#define GENDER_FEMALE  2

#define MAX_PASSWORD_LEN 128

#define KEY_ACTIVE -32767

#define DEFAULT_LPS 20
#define DELAY_LOOP      delay((int)(1000 / DEFAULT_LPS))


//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>


//class definitions
class Command
{  
  public:
    class Model
    {
      public:
        std::string              match;
        double                   priority;
        int                      commandType;
        int                      questionType;
        int                      question;

        void dump ();
    } ;

    //Constructors
    Command ();
    Command (std::fstream&);

    //Destructors
    ~Command  ();

    //Other Functions
    void   dump           ();

    std::string              name;
    std::string              description;
    std::string              definition;
    int                      returnType;
    std::string              module;
    double                   modulePriority;
    double                   priority;
    int                      modelCount;
    std::vector<Model>       models;
    std::string              output;
    double                   risk;
    
    void        readCommandFromFileStream (std::fstream&);
} ;

class Module
{
  public:
    //Constructors
    Module ();
    Module(std::string);

    //Destructors
    ~Module();

    //Other Functions
    void        dump       ();
    
    std::string                            name;
    std::string                            author;
    std::string                            description;
    double                                 priority;
    std::vector<std::string>               keywords;
    std::vector<std::string>               dependencies;
    std::vector<std::string>               variables;
    std::vector<std::string>               resourceFiles;
    std::vector<std::string>               dataFiles;
    std::vector<std::string>               solutionFolders;
    std::vector<std::string>               solutionFiles;
    std::vector<int>                       solutionPropertyCounts;
    std::vector<std::vector<std::string> > solutionProperties;
    int                                    commandCount;
    std::vector<Command>                   commands;
} ;

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
    Contact                            (std::istream&);

    Contact::Name    newName           (std::istream&);
    Contact::DLA     newDla            (std::istream&);
    Contact::Phone   newPhone          (std::istream&);
    Contact::Email   newEmail          (std::istream&);
    Contact::Address newAddress        (std::istream&);
    Contact::Account newAccount        (std::istream&);

    //Destructors
    ~Contact                           ();

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

class User
{
  public: 

    //Constructors
    User                              ();

    //Destructors
    ~User                             ();

    //Accessors
    std::string  getUsername           () const;
    std::string  getId                 () const;
    bool         getAdmin              () const;
    std::string  getLoc                () const;

    //Modifiers
    void         setUsername           (std::string);
    void         setPassword           (std::string);
    void         setAdmin              (bool);
    void         setLoc                (std::string);

    //Other Functions
    bool         login_simple          ();
    void         loadUser_simple       ();

  private:
    std::string              username;
    std::string              id;
    std::string              password;
    bool                     admin;

    std::string              loc;

    bool  authenticateUser      ();

    void  loadUserFileS         (std::string, std::vector<std::string>&, int& dataLen);
    void  loadUserFileS         (std::string, std::ostream&);
} ;


//global variable definitions
HANDLE      h_    = GetStdHandle(STD_OUTPUT_HANDLE);
HWND        hWnd_ = GetConsoleWindow();
std::string loc_;


//function definitions
int         main                 ();

void        loadLists            (std::string, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&);

void        backupModule         (Module, std::string, std::string);
void        backupModule         (Module, std::string, std::string, bool);
void        saveModule           (Module, std::string, std::string, std::string);

void        loadSettings         (std::string, std::vector<std::string>&, std::vector<std::string>&, std::string);
void        saveSettings         (Module, std::string, std::vector<std::string>&, std::vector<std::string>&, std::string);

void        makeFunctions        (Module, std::string);
void        makeCpp              (Module, std::string);
void        makeH                (Module, std::string);
void        makeSettings         (Module, std::string);
void        makeVariables        (Module, std::string);
void        makeResource         (Module, std::string);

void        addToDla             (Module, std::string);
void        removeFromDla        (Module, std::string);
void        removeFromDla        (Module, std::string, bool);
void        addToUser            (Module, std::string, std::string);
void        removeFromUser       (Module, std::string, std::string);
void        addUserModule        (Module, std::string, std::string);
void        removeUserModule     (Module, std::string, std::string);


//Common
bool        getint           (std::istream&, int&);
bool        getdouble        (std::istream&, double&);
bool        getbool          (std::istream&, bool&);
void        cls              (HANDLE);
std::string uCase            (std::string);
std::string lCase            (std::string);
std::string str              (int);
std::string str              (char);
void        loadFileS        (std::string, std::vector<std::string>&);
std::string              loadFileStr             (std::string);

std::string              removePunctuation       (std::string);
void                     removePunctuationSoft   (std::vector<std::string>&);
void                     ReplaceStringInPlace    (std::string&, std::string, std::string);

void                     alphabetize             (std::vector<std::string>&);
std::vector<std::string> tokenize                (std::string);
std::vector<std::string> tokenize                (std::string, bool);
void                     delay                   (int);
std::string              generateGuid            ();
std::string              getEnvVar               (std::string);
void                     runBatch                (std::string);
void                     runBatch                (std::string, bool);
void                     runBatch                (std::string, std::vector<std::string>);
void                     runBatch                (std::string, std::vector<std::string>, bool);



//Filesystem
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



//User
bool        simpleLogin (User&);



//Keyboard
int INKEY();



//Crypto
std::string passHasher     (std::string, std::string, int, byte);
bool        verifyPassword (std::string, std::string);
void        loadEncFileS   (std::string, std::string, std::vector<std::string>&, int&);


#endif