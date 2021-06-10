#include <iostream>
#include "mpi.h"
#include <cstring>
#include <sys/sysinfo.h>
#include <stdint.h>
#include <cstdint>
#include <thread>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>


int main(int argc, char** argv) {

  struct sysinfo sys_info;
  
 

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  char node_name[MPI_MAX_PROCESSOR_NAME];
  int rank,size, namelen;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Get_processor_name(node_name, &namelen);
  memset(node_name+namelen,0,MPI_MAX_PROCESSOR_NAME-namelen);

  const auto cores_count = std::thread::hardware_concurrency();
  int total_ram = 0;
  if (sysinfo(&sys_info) == -1) return 1;
  total_ram =((uint64_t)sys_info.totalram * sys_info.mem_unit)/1024; //Getting the RAM size
  
  
  std::string target = "cpu MHz";
  std::string speed;

  std::ifstream reader{"/proc/cpuinfo"};
  if (reader) { //Reading the file where CPUINFO is store so we can get the line that is realted to MHZ
    for (std::string line; std::getline(reader, line);)
        if (line.find(target) != std::string::npos){
            int pos = line.find(":");   
            speed = line.substr(pos + 1);  
        }
}

  reader.close();
    
 
  //std::cout<<std::endl;
  bool check = true;
  
  for (int i = 0; i<10; ++i){
  if ( rank == 0 && check == true ) {
    std::cout<<"Counts presented by head node: " << size << std::endl;
      check=false;
    }
  if ( rank == i ){
  usleep(rank*10000+10000);
  std::cout << std::setw(8) << "Node: " << std::setw(8) << node_name << std::setw(8) << "|| Rank: " << std::setw(2) <<rank << std::setw(8) << "|| RAM: " 
  << std::setw(8) << total_ram << std::setw(8) << "|| Cores count: " << std::setw(2) << cores_count<< std::setw(8) << "|| ClockSpeed: "<< std::setw(2) << speed << "MHz"<< std::endl;
  }
 }
  MPI_Finalize();
  
  return 0;

}
