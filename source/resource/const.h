////////////////////////////////////////////////////////////////////////////////
//File:   const.h
//Dir:    source/resource/
//Date:   2014-12-31
//Author: Zachary Gill
//Sets compiler definitions
////////////////////////////////////////////////////////////////////////////////


#ifndef _DLA_SR_CONST_H_
#define _DLA_SR_CONST_H_


//constants

#define AND &&
#define OR  ||
#define NOT !
#define EQU ==
#define NQU !=

#define GENDER_DEFAULT 0
#define GENDER_MALE    1
#define GENDER_FEMALE  2

#define INPUT_INPUT   0
#define INPUT_TEXT    1
#define INPUT_AUDIO   2
#define INPUT_GRAPHIC 3

#define OUTPUT_OUTPUT  0
#define OUTPUT_TEXT    1
#define OUTPUT_AUDIO   2
#define OUTPUT_GRAPHIC 3

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

#define NUM_LETTERS 26

#define REG_PMATCH_SIZE 128

#define DEFAULT_LPS 20

#define DELAY_LOOP      delay_loop(settings_.getSettingI(NULL, "METRICS", "LOOP_SPEED"))
#define RANDOMIZE_TIMER srand((unsigned int)time(NULL))


#endif