#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "pro.pb.h"

using namespace std;

int main(void)
{
  cout << "Start Protocol buffers reader sample" << endl;
  tutorial::Person getperson;

  std::fstream input("stream.dat", ios::in | ios::binary);
  // fstream input("../python/pbdata_py.dat", ios::in | ios::binary);
  //fstream input("../julia/pbdata_julia.dat", ios::in | ios::binary);
  for (size_t i = 0; i < 40; i++)
  {
    std::this_thread::sleep_for(1000ms);
    input.open("stream.dat", ios::in | ios::binary);
    getperson.ParseFromIstream(&input);
    input.close();
    //std::cout << getperson.id() << std::endl;
    // Human readable print
    cout << getperson.DebugString() << endl;
  }

  cout << "Done" << endl;
}