#include <iostream>
#include <vector>
#include <string>

#include "testing.h"

#ifdef __Operator__

using namespace std;

class a
{
  public:
    a() {}
    ~a() {}

    void main ()
    {
      cout << "0";
    }
} ;

class b : public a
{
  public:
    b() {}
    ~b() {}

    virtual void main ()
    {
      cout << "1";
    }
} ;

int main ()
{
  vector<a*> p;
  p.push_back(new b);

  p[0]->main();

  system("PAUSE");
  return 0;
}

#endif