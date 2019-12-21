////////////////////////////////////////////////////////////////////////////////
//File:   input.h
//Dir:    source/header/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of Output
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_OUTPUT_H_
#define _DLA_SH_OUTPUT_H_


//constants
#include "const.h"


//includes
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>


//class definitions
class Output
{
  public:
    Output                              ();

    ~Output                             ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectOutput   ();
    virtual void        start           ();
    virtual void        pause           ();
    virtual inline void free            ();

    int                 getType         () const;
    std::string         getName         () const;
    bool                getActive       () const;
    bool                getConsole      () const;

    void                setType         (int);
    void                setName         (std::string);

    bool        done;

  protected:
    int         type;
    std::string name;
    bool        active;
    bool        console;
};

class TextOutput : public Output
{
  public:
    TextOutput              ();

    std::string getBuffer   () const;
    std::string getLast     () const;

    void        setBuffer   (std::string);
    void        clearBuffer ();
    void        setLast     (std::string);

  protected:
    std::string  buffer;
    std::string  last;

    std::fstream file;
};

class AudioOutput : public TextOutput
{
  public:
    AudioOutput      ();

    bool isSpeaking  ();

    int  getLocation () const;
    
    void setLocation (int);

  protected : 
    int  currentOutput;
    bool speaking;
    int  location;
};

class GraphicOutput : public Output
{
  public:
    GraphicOutput ();
};


//global variable definitions
extern bool                     terminate_;
extern std::string              response_;
extern std::vector<std::string> oQueue_;
extern std::vector<double>      oPriority_;
extern int                      oCount_;
extern int                      outputCount_;
extern std::vector<Output*>     outputs_;
extern std::vector<std::string> outputList_;


//resources
#include "outputs"


//function definitions
void        loadOutputs  ();
void        loadOutputs  (std::string);
bool        isOutput     (std::string, std::vector<std::string>&);
bool        hasOutput    (std::string);

void        addOutput    (std::string);
void        addOutput    (std::string, double);

void        startOutputs ();
void        pauseOutputs ();


//shared function definitions
#include "common.h"


#endif