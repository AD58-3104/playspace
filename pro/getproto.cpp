#include <fstream>
#include <iostream>
#include <string>

#include "pro.pb.h"

using namespace std;

int main(void) {
  cout << "Start Protocol buffers reader sample" << endl;
  tutorial::AddressBook address_book;

     fstream input("./pbdata_cpp.dat", ios::in | ios::binary);
  // fstream input("../python/pbdata_py.dat", ios::in | ios::binary);
  //fstream input("../julia/pbdata_julia.dat", ios::in | ios::binary);
  address_book.ParseFromIstream(&input);

  // Human readable print
  cout << address_book.DebugString() << endl;

  cout << "Done" << endl;
}