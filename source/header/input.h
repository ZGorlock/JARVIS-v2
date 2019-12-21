////////////////////////////////////////////////////////////////////////////////
//File:   input.h
//Dir:    source/header/
//Date:   2014-12-30
//Author: Zachary Gill
//Interface of Input
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SH_INPUT_H_
#define _DLA_SH_INPUT_H_


//constants
#include "const.h"

#define KEY_ACTIVE -32767


//standard includes
#include "stdafx.h"
#include <string>
#include <vector>
#include <map>


//class definitions
class Input
{
  public:
    Input                               ();

    ~Input                              ();

    virtual void        main            ();
    virtual inline void init            ();
    virtual inline void defaultSettings ();
    virtual inline bool collectInput    ();
    virtual void        start           ();
    virtual void        pause           ();
    virtual inline void free            ();

    int                 getType         () const;
    std::string         getName         () const;
    bool                getActive       () const;
    bool                getConsole      () const;

    void                setType         (int);
    void                setName         (std::string);

    bool done;

  protected:
    int         type;
    std::string name;
    bool        active;
    bool        console;
} ;

class TextInput : public Input
{
  public:
    TextInput  ();

    void        updateBuffer (std::string);
    void        clearBuffer  ();
    std::string getBuffer    () const;
    void        setBuffer    (std::string);

  protected:
    std::string buffer;
} ;

class AudioInput : public TextInput
{
  public:
    AudioInput  ();
} ;

class GraphicInput : public Input
{
  public:
    GraphicInput  ();

    int* getGraphic () const;
    void setGraphic (int*);

  protected:
    int* graphic;
} ;


//global variable definitions
extern bool                          terminate_;
extern bool                          help_;
extern bool                          foreground_;
extern std::vector<std::string>      iQueue_;
extern std::vector<double>           iPriority_;
extern int                           iCount_;
extern int                           inputCount_;
extern HWND                          hWnd_;
extern std::vector<Input*>           inputs_;
extern std::vector<std::string>      inputList_;


//resources
#include "inputs"


//function definitions
void        loadInputs  ();
void        loadInputs  (std::string);
bool        isInput     (std::string, std::vector<std::string>&);
bool        hasInput    (std::string);
void        hotkeys     ();

void        addInput    (std::string);
void        addInput    (std::string, double);

void        startInputs ();
void        pauseInputs ();

int			    INKEY       ();
char		    INKEYC      ();
std::string INKEYS      ();


//shared function definitions
#include "common.h"


#endif