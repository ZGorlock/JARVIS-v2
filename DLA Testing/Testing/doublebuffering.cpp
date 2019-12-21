#include <windows.h> 
#include <stdio.h>

#include "testing.h"

#ifdef __DubBuf__


using namespace std;

void display();
void cls(HANDLE);


HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

SMALL_RECT srctReadRect;
SMALL_RECT srctWriteRect;
CHAR_INFO chiBuffer[2000];
COORD coordBufSize;
COORD coordBufCoord;


int main(void) 
{
  SetConsoleActiveScreenBuffer(hScreenBuffer);
  
  srctReadRect.Top = 0;
  srctReadRect.Left = 0;
  srctReadRect.Bottom = 24;
  srctReadRect.Right = 79;
  coordBufSize.Y = 25;
  coordBufSize.X = 80;
  coordBufCoord.X = 0;
  coordBufCoord.Y = 0;
  srctWriteRect.Top = 0;
  srctWriteRect.Left = 0;
  srctWriteRect.Bottom = 24;
  srctWriteRect.Right = 79;


  while (true) {
    for (int i = 0; i < 20; i ++)
      cout << "testing" << endl;
    display();
  }



  SetConsoleActiveScreenBuffer(hStdout);

  return 0;
}

void display()
{
  ReadConsoleOutput(hStdout, chiBuffer, coordBufSize, coordBufCoord, &srctReadRect);
  WriteConsoleOutput(hScreenBuffer, chiBuffer, coordBufSize, coordBufCoord, &srctWriteRect);
  cls(hStdout);
}

void cls(HANDLE hConsole)
{
  COORD coordScreen = { 0, 0 };
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;

  GetConsoleScreenBufferInfo(hConsole, &csbi);
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
  SetConsoleCursorPosition(hConsole, coordScreen);
}


#endif