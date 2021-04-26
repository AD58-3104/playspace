#include <fstream>
#include <iostream>
#include <string>

#include "pro.pb.h"

using namespace std;

int main(void) {
  cout << "Start Protocol buffers reader sample" << endl;
  tutorial::Person person;

  std::fstream input("../stream.txt", ios::in | ios::binary);
  // fstream input("../python/pbdata_py.dat", ios::in | ios::binary);
  //fstream input("../julia/pbdata_julia.dat", ios::in | ios::binary);
  person.ParseFromIstream(&input);

  // Human readable print
  cout << person.DebugString() << endl;

  cout << "Done" << endl;
}