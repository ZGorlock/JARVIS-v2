#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

#include <commdlg.h>

#include "testing.h"

#ifdef __ComDialogBox__

using namespace std;

int main()
{
  HWND hWnd = GetConsoleWindow();

  OPENFILENAMEA ofn = { 0 };
  char Buffer[512];
  fill(Buffer, Buffer + 512, '\0');

  ofn.lStructSize = sizeof(OPENFILENAMEA);
  ofn.hwndOwner = hWnd;
  ofn.lpstrFilter = NULL;
  ofn.lpstrCustomFilter = NULL;
  ofn.nFilterIndex = 0;
  ofn.lpstrFile = Buffer;
  ofn.nMaxFile = 512;
  ofn.lpstrFileTitle = NULL;
  ofn.lpstrInitialDir = NULL;
  ofn.lpstrTitle = NULL;
  ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  GetOpenFileNameA(&ofn);
  cout << ofn.lpstrFile << endl;
  //cout << CommDlgExtendedError() << endl;
  
  system("PAUSE");
  return 0;
}

#endif