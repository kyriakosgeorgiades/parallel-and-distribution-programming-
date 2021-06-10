#include "omp.h"
#include <chrono>
#include <thread>
#include <iostream>

int main (void)
{
  

    
   #pragma omp parallel for schedule (runtime)
   for (int count=1; count<=64;++count)
     std::cout<<count<<std::endl;
 
}
