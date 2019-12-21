#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/any.hpp>

#include "testing.h"

#ifdef __VectoVector_

using namespace std;


int main ()
{
  vector<boost::any> vars_;

  vector<int>    n;
  vector<double> m;
  vector<string> o;

  n.push_back(5);
  n.push_back(1);
  
  m.push_back(7.15);

  o.push_back("hello");
  o.push_back("world");
  o.push_back("test");

  vars_.push_back(n);
  vars_.push_back(m);
  vars_.push_back(o);

  cout << boost::any_cast< vector<int> >(vars_[0])[0] << endl
       << boost::any_cast< vector<string> >(vars_[2])[1] << endl;

  system("PAUSE");
  return 0;
}

#endif