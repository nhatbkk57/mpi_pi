#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <cstdlib>
#include <limits>
#include <iomanip>
#define PI 3.14159265
#define chunksize 100000

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

struct pi{
    double value;
    double error;
};

pi cal_Pi_seq()
{
    pi pi;
    int counter = 0;
    for (int i = 0; i < chunksize; i++)
    {
      double x = dis(gen);
      double y = dis(gen);
      if (x * x + y * y < 1){
        counter++;
      }
    }
    pi.value = 4.0 * counter / chunksize;
    pi.error = fabs((pi.value-PI)/PI);
    return pi;
}

int main(int argc, char** argv)
{
    double final_time=0.0;
    double final_pi=0.0;
    double final_err=0.0;
    using millis = std::chrono::milliseconds;
    using clock = std::chrono::steady_clock;
    using std::chrono::duration_cast;
    
    double e = std::stod(argv[1]);
    pi pi;
    pi.error = 9999.9999;

    for (int i=0; i<100; i++){
        clock::time_point start = clock::now();
        while (pi.error > e){
            pi = cal_Pi_seq();
        }
        clock::time_point end = clock::now();
        final_pi += pi.value;
        final_err += pi.error;
        final_time += duration_cast<millis>(end-start).count();
    }
    std::cout << "OUTPUT:" << std::endl;
    std::cout << "chunksize: " << chunksize << std::endl;
    std::cout << "repeat: " << 100 << std::endl;
    std::cout << "Real Pi: 3.14159265..." << std::endl;
    std::cout << "Avg Calculated Pi: " << std::setprecision(12) << final_pi/100 << std::endl;
    std::cout << "Avg Calculated Error: " << std::setprecision(12) << final_err/100 << std::endl;
    std::cout << "Avg Time: " << final_time/100 << " milliseconds" << std::endl;
    return 0;
}