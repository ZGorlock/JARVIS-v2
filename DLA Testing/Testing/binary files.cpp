#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

#include "testing.h"

#ifdef __BinaryFiles__

using namespace std;

int main()
{
  fstream file;
  file.open("test.txt", fstream::out);
  file.close();

  fstream outf;
  outf.open("test.txt", fstream::binary | fstream::out);
  string s = "testing strings in binary";
  int size = s.size() + 1;
  int a = 15423451, b = 1235576532, c = 18453451, d = 8453042, e = 1512166545, f = 3;
  outf.write((char*)&a, sizeof(int));
  outf.write((char*)&b, sizeof(int));
  outf.write((char*)&c, sizeof(int));
  outf.write((char*)&d, sizeof(int));
  outf.write((char*)&e, sizeof(int));
  outf.write((char*)&f, sizeof(int));
  outf.write((char*)&size, sizeof(int));
  outf.write(s.c_str(), size);
  outf.close();
  
  fstream inf;
  inf.open("test.txt", fstream::binary | fstream::in);
  int g, h, i, j, k, l;
  int sizeStr;
  char *get;
  inf.read((char*)&g, sizeof(int));
  inf.read((char*)&h, sizeof(int));
  inf.read((char*)&i, sizeof(int));
  inf.read((char*)&j, sizeof(int));
  inf.read((char*)&k, sizeof(int));
  inf.read((char*)&l, sizeof(int));
  inf.read((char*)&sizeStr, sizeof(int));
  get = new char[sizeStr];
  inf.read(get, sizeStr);
  inf.close();

  cout << a << " " << b << " " << c << " " << d << " " << e << " " << f << s   << endl;
  cout << g << " " << h << " " << i << " " << j << " " << k << " " << l << get << endl;
  
  system("PAUSE");
  return 0;
}

#endif