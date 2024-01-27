#include <fast_io.h>
#include <fast_io_device.h>
#include <chrono>
#include <fstream>
#include <iostream>

int main(int argc, char const *argv[])
{
    fast_io::io::println("Get println in my C++!!!");
    fast_io::io::print("Hello World from fast_io\n");
	fast_io::obuf_file obf(fast_io::mnp::os_c_str("outputfile.txt"));
	// transmit(fast_io::c_stdout(),obf);//Transmit all bytes to FILE* stdout
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 100'000; i++)
    {
        // fast_io::io::println(obf," sdfoijasdfoijawoiejfo jawefiojwaefojawoi jsoidajfoisadjfjsadfa asodifjsaodijfwaeloop is ",i);
        fast_io::io::println(" sdfoijasdfoijawoiejfo jawefiojwaefojawoi jsoidajfoisadjfjsadfa asodifjsaodijfwaeloop is ",i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    uint64_t time_fast = elapsed.count();
    std::ofstream ofs("ofsoutput.txt");
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 100'000; i++)
    {
        // ofs << " sdfoijasdfoijawoiejfo jawefiojwaefojawoi jsoidajfoisadjfjsadfa asodifjsaodijfwaeloop is " << i << "\n";
        std::cout << " sdfoijasdfoijawoiejfo jawefiojwaefojawoi jsoidajfoisadjfjsadfa asodifjsaodijfwaeloop is " << i << "\n";
    }
    end = std::chrono::high_resolution_clock::now();
    fast_io::io::println("obf Elapsed time: ",time_fast,"us");
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    fast_io::io::println("Ofstream Elapsed time: ",elapsed.count(),"us");

    return 0;
}
