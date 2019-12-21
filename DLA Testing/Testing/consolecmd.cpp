#include <iostream>
#include <Windows.h>

#include "testing.h"

#ifdef __ConsoleCmd__

using namespace std;

int main ()
{
  HWND hWnd_ = GetConsoleWindow();
  RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL | MOD_NOREPEAT, VK_HOME);
  
  ShowWindow(hWnd_, SW_HIDE);
  Sleep(3000);
  ShowWindow(hWnd_, SW_RESTORE);
  Sleep(3000);

  ShowWindow(hWnd_, SW_HIDE);

  MSG msg = { 0 };
  while (GetMessage(&msg, NULL, 0, 0) != 0)
    if (msg.message == WM_HOTKEY)
      ShowWindow(hWnd_, SW_RESTORE);


  return 0;
}

#endif