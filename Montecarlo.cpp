/* boost random_demo.cpp profane demo
 *
 * Copyright Jens Maurer 2000
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id: random_demo.cpp 71018 2011-04-05 21:27:52Z steven_watanabe $
 *
 * A short demo program how to use the random number library.
 */

 #include <iostream>
 #include <fstream>

 #include<cmath>
 #include <boost/random/linear_congruential.hpp>
 #include <boost/random/uniform_int.hpp>
 #include <boost/random/uniform_real.hpp>
 #include <boost/random/variate_generator.hpp>
 #include <boost/generator_iterator.hpp>
 
 // This is a typedef for a random number generator.
 // Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
 typedef boost::minstd_rand base_generator_type;
 
 // This is a reproducible simulation experiment.  See main().
 double Montecarlo(double e){
   int n=pow(1.0/(2*e),2);
   
   base_generator_type generator(42);
   int inside=0;
   int outside=0;

   
   
     // Define a uniform random number distribution which produces "double"
     // values between 0 and 1 (0 inclusive, 1 exclusive).
     boost::uniform_real<> uni_dist(0,1);
     boost::variate_generator<base_generator_type&, boost::uniform_real<> > X(generator, uni_dist);
     boost::variate_generator<base_generator_type&, boost::uniform_real<> > Y(generator, uni_dist);
    
     std::cout.setf(std::ios::fixed);
     // You can now retrieve random numbers from that distribution by means
     // of a STL Generator interface, i.e. calling the generator as a zero-
     // argument function.
     for(int i = 0; i < n; i++){
         double x=X();
         double y=Y();
         if(x*x+y*y<=1)
           inside++;
        
     }
    
    double pi= double(4*inside)/n;
    return pi;
 }
 
 int main()
 {
   double pi=Montecarlo(0.001);
   std::cout<<"PI "<< pi;
   
   return 0;
 }