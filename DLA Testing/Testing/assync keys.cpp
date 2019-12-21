#include <iostream>
#include <windows.h>

#include "testing.h"

#ifdef __AssyncKeys__

using namespace std;

int main()
  {
  HANDLE hstdin;
  DWORD  mode;

  hstdin = GetStdHandle( STD_INPUT_HANDLE );
  GetConsoleMode( hstdin, &mode );
  SetConsoleMode( hstdin, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT );  // see note below 

  cout << "Press any key..." << flush;
  int ch = cin.get();

  if (ch == 13) cout << "\nYou pressed ENTER\n";
  else          cout << "\n\nYou did not press ENTER\n";

  SetConsoleMode( hstdin, mode );
  return 0;
  }

#endif