#include<string>
#include<iostream>
#include <stdlib.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/timer.hpp>
using namespace std;
namespace mpi = boost::mpi;

double f(double x){
    double t = 4.0/(1+x*x);
    return t;
}

int main(int argc, char*argv[]){
    int m = 1000000;
    double global_sum;
    double start, end;
    // double global_sum = f(0.0)+ f(1.0);
    mpi::environment env;
    mpi::communicator world;
    world.barrier();
    mpi::timer timer;
    broadcast(world, m, 0);
    

    double local_sum;
    for (int i=world.rank(); i<m; i+=world.size()){
        local_sum+= 2*f(1.0*i/m);
    }
    for (int i=world.rank(); i<m+1; i+=world.size()){
        local_sum+= 4*f(1.0*(2*i-1)/(2*m));
    }
    
    reduce(world, local_sum, global_sum, std::plus<double>(), 0);
    world.barrier();
    end = timer.elapsed();
    if (world.rank() == 0){
        global_sum += f(0.0)+ f(1.0);
        double pi = 1.0/(6*m)*global_sum;

        std::cout<<"m = "<< m <<std::endl;
        std::cout<<"PI = "<< pi <<std::endl;
        std::cout<<"Time = "<< end << std::endl;
    }

    return 0;
}
