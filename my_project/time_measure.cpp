#include <chrono>
#include <deque>
#include <cstdint>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <syslog.h>
#include <sstream>
#include <fstream>
using namespace std::literals::chrono_literals;

void fileOpen()
{
    for (size_t i = 0; i < 5; i++)
    {
        FILE *fp = fopen("/var/tmp/error.txt", "a");
        if (fp != NULL)
        {
            time_t now = time(NULL);
            struct tm *pnow = localtime(&now);
            fprintf(fp, "%d:%d:%d WALKING CONTROL OVERTIME: %dms\r\n",
                    pnow->tm_hour, pnow->tm_min, pnow->tm_sec,
                    1000);
            fclose(fp);
        }
    }
}

void logging()
{
    for (size_t i = 0; i < 19; i++)
    {
        std::stringstream sstream;
        sstream << "Azusa Miura" << 123 << "imas" << 765;
        const char *hr46 = "hr46_b3m_debug";
        openlog(hr46, LOG_PID, LOG_USER);
        syslog(LOG_DEBUG, "%s", sstream.str().c_str());
        closelog();
    }
}

void print()
{
    for (size_t i = 0; i < 5; i++)
    {
        printf("*******ERROR**** xv_sv[%d].deg_sv=%f\n", i, i * 150 / 100.0f);
    }
}

void fstream()
{
    std::ofstream ofs("/var/tmp/error.txt",std::ios::app);
    for (size_t i = 0; i < 5; i++)
    {
        ofs << "*******ERROR**** xv_sv[" << i << "].deg_sv=" << i * 150 / 100.0f << "\n";
    }
    // ofs.close();
}

int main(int argc, char const *argv[])
{
    std::deque<uint64_t> d;
    for (size_t i = 0; i < 10000; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        fstream();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        d.emplace_back(std::chrono::duration_cast<std::chrono::microseconds>(diff).count());
    }
    std::sort(d.begin(), d.end());
    uint64_t total = 0;
    for (const auto &val : d)
    {
        std::cout << val << "us" << std::endl;
        total += val;
    }
    std::cout << "average " << total / 10000.0f << "us" << std::endl;
    return 0;
}
