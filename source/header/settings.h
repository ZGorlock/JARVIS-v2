////////////////////////////////////////////////////////////////////////////////
//File:   settings.h
//Dir:    source/header/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of Settings
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_SETTINGS_H_
#define _DLA_SH_SETTINGS_H_


//constants
#include "const.h"

#define SETTING_METRIC 0
#define SETTING_MODULE 1
#define SETTING_INPUT  2
#define SETTING_OUTPUT 3
#define SETTING_NULL   4


//standard includes
#include <string>
#include <vector>


//class definitions
class Setting
{
  public:
    //Constructors
    Setting              ();
    Setting              (std::string, bool);
    Setting              (std::string, int);
    Setting              (std::string, double);
    Setting              (std::string, std::string);

    //Destructors
    ~Setting             ();

    //Accessors
    std::string getName  ();
    std::string getValue ();

    //Modifiers
    void        setName  (std::string);
    void        setValue (bool);
    void        setValue (int);
    void        setValue (double);
    void        setValue (std::string);

    //Other Functions
    void        dump     ();
    void        dump     (std::ostream&);
    void        dump     (std::ostream&, std::string);

  private:
    std::string name;
    std::string value;
    
};
class SettingGroup
{
  public:
    //Constructors
    SettingGroup                     ();
    SettingGroup                     (int, std::string);

    //Destructors
    ~SettingGroup                    ();

    //Accessors
    std::string          getName     ();
    int                  getType     ();
    std::vector<Setting> getSettings ();
    std::string          getFile     ();

    //Modifiers
    void                 setName     (std::string);
    void                 setType     (int);
    void                 setFile     (std::string);

    //Other Functions
    Setting              getSetting  (std::string);
    void                 setSetting  (std::string, bool);
    void                 setSetting  (std::string, int);
    void                 setSetting  (std::string, double);
    void                 setSetting  (std::string, std::string);
    void                 addSetting  (Setting);
    
    void                 dump        ();
    void                 dump        (std::ostream&);
    void                 dump        (std::ostream&, std::string);
    void                 save        ();

  private:
    std::string          name;
    int                  type;
    std::vector<Setting> settings;
    std::string          file;
};
class Settings
{
  public:
    //Constructors
    Settings                                   ();
    
    //Destructors
    ~Settings                                  ();

    //Accessors
    std::vector<SettingGroup> getSettingGroups ();

    //Other Functions
    SettingGroup              getSettingGroup  (int, std::string);
    std::string               getSetting       (int, std::string, std::string);
    void                      setSetting       (int, std::string, std::string, bool);
    void                      setSetting       (int, std::string, std::string, int);
    void                      setSetting       (int, std::string, std::string, double);
    void                      setSetting       (int, std::string, std::string, std::string);
    
    bool                      getSettingB      (int, std::string, std::string);
    int                       getSettingI      (int, std::string, std::string);
    double                    getSettingD      (int, std::string, std::string);
    std::string               getSettingS      (int, std::string, std::string);
 
    void                      addSettingGroup  (SettingGroup);

    void                      clear            ();

    void                      dump             ();
    void                      dump             (std::ostream&);
    void                      dump             (std::ostream&, std::string);
    void                      save             ();
    void                      load             ();
    void                      loadDefault      ();

    bool                      hasSettingGroup  (int, std::string);
    bool                      hasSetting       (int, std::string, std::string);

  private:
    std::vector<SettingGroup> settingGroups;
};


//global variable definitions
extern std::string              userLoc_;
extern int                      moduleCount_;
extern int                      inputCount_;
extern int                      outputCount_;
extern std::vector<std::string> moduleList_;
extern std::vector<std::string> inputList_;
extern std::vector<std::string> outputList_;


//shared function definitions
#include "common.h"


#endif