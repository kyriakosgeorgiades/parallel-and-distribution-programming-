#include <iostream>
#include <string>    
#include <cstring>
#include "mpi.h"
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

typedef struct {
  char poem_store[MPI_MAX_PROCESSOR_NAME];
  int  index_store;
}hostStruct;


MPI::Datatype  createMpiWordType(){
    //how many data types in the struct
    const int count = 2; 
    //type of every different block of data
    MPI::Datatype typesInStruct[count] = {MPI::CHAR,MPI::INT};
    //how many elements per block 
    int arrayBlockLengths [count] = {MPI_MAX_PROCESSOR_NAME,1};
    //Now we need to specify starting memory location of each block, *relative to the start of the struct only*
    // it's a bit of a tedious process, but neccesary. 
    MPI::Aint objAddress, address1,address2;
    MPI::Aint arrayDisplacements[count];	
    hostStruct sbuf;//Just has to be a struct instance but not
                    // the one you're actually sending, since we only need those locations,
                    // which, since this is a static struct, will always be the same
    objAddress = MPI::Get_address(&sbuf);
    address1 = MPI::Get_address(&sbuf.poem_store);
    address2 = MPI::Get_address(&sbuf.index_store);
    arrayDisplacements[0] = address1 - objAddress;
    arrayDisplacements[1] = address2 - objAddress;
    // now we create the MPI equivilent datatype using the data we just collected
    MPI::Datatype mpiHostStruct;
    mpiHostStruct = MPI::Datatype::Create_struct(count,arrayBlockLengths,arrayDisplacements,typesInStruct);
    // and commit it the the Communicator, so it can be used accross the entire cluster
    mpiHostStruct.Commit();
    return mpiHostStruct;
  }


int main(int argc, char** argv) {
  int com_size, rank, namelen;
 hostStruct receive;
  memset(receive.poem_store, 0, MPI_MAX_PROCESSOR_NAME);
  MPI_Init(NULL,NULL);
  char node_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm_size(MPI_COMM_WORLD, &com_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  memset(node_name, 0, MPI_MAX_PROCESSOR_NAME);
  MPI_Get_processor_name(node_name, &namelen);
  memset(node_name+namelen,0,MPI_MAX_PROCESSOR_NAME-namelen);
  
  //Create an instance of the datatype you built
  MPI::Datatype mpiHostStruct = createMpiWordType();
  int node_to_send[24];
  std::ifstream infile("poem.txt");
  std::string line;
  std::vector<int> index_slaves;
  std::vector<std::string> original_poem;
  std::vector<std::string> poem_slaves;
  hostStruct receiving;
  hostStruct sending;
  
  
  #pragma omp parallel for
   for ( int i =0; i<24; i++){ //Creating an array for the random choosen nodes in the cluster to be used by the head node
    node_to_send[i] = rand() % 7 + 1;
  } 
  
  
  if (rank==0) { //In head node rank = 0 
    while (std::getline(infile, line)) //Getting the lines of the file in an array of size 24 total lines of the file
  {
     original_poem.push_back(line);
  }
    for (int i =0; i<24; i++){
      std::string lines;
      lines = original_poem[i];
      strcpy(sending.poem_store, lines.c_str());//Coverting and storing the string to array of char the struct var
      sending.index_store = i; //Storing the index of the line as it is in order i can respent the order index
      MPI_Send(&sending, 1 , mpiHostStruct, node_to_send[i], 1, MPI_COMM_WORLD); //Sending through the custom data type the line and index
      
    }
 }
 
   for ( int i = 0; i<24; i++){ //Creating a loop of 24 iters to match the 24 MPI_Send above
      if (rank == node_to_send[i] ){ //Making sure we are in the correct rank to MPI_REcv the line and index
         
         MPI_Recv(&receiving, 1, mpiHostStruct, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //Recv the line and index
         index_slaves.push_back(receiving.index_store);//Pushing back at each nodes int vector as each process has its own memory
         poem_slaves.push_back(receiving.poem_store); //Pushing back for each line to string vector as each process has its own memory
       }
      }
      
   for ( int i = 1; i<=7; i++){ //Loop so each node sends back what it has received
     if ( rank == i ) {
       std::string local_line;
       receiving.index_store = index_slaves.size();//Getting the size so we can use it as the COUNT of the index numbers
       MPI_Send(&receiving,1, mpiHostStruct, 0, 1, MPI_COMM_WORLD);//Sending the size;
       for (unsigned int k = 0; k < index_slaves.size(); ++k){ //Loop to to send index and lines of each node
            receiving.index_store = index_slaves[k];
            local_line = poem_slaves[k];
            strcpy(receiving.poem_store, local_line.c_str());
            MPI_Send(&receiving,1, mpiHostStruct, 0, 0, MPI_COMM_WORLD);
           }    
      }
    }
      
      
  if ( rank == 0 ){ //Head node now will receive the index and recreate the poem using MAP is it is sorted by itself
  std::map<int,std::string> links;
  std::vector<int> head_indexs;
    for ( int i = 1; i<=7; i++){
      int inner_loop;
      MPI_Recv(&receiving, 1 , mpiHostStruct, i , 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      inner_loop = receiving.index_store;//Getting the size of the loop
      std::cout<<"==============================================="<<std::endl;
      std::cout<< "RANK:" << std::setw(5)<<i<<std::endl;
      std::cout<<"COUNT"<< std::setw(10) <<"INDEXS"<<std::endl;
      std::cout<<inner_loop<<std::setw(9);
      for ( int k = 0; k<inner_loop; ++k){//Receiving from each node the lines and indeces sent
        MPI_Recv(&receiving, 1 , mpiHostStruct, i , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        links.insert(std::pair<int, std::string>(receiving.index_store, receiving.poem_store));
        std::cout<<receiving.index_store<< std::setw(5);
      }
      std::cout<<std::endl;
      std::cout<<"==============================================="<<std::endl;
      
    }
    for(auto elem : links)//Presenting the rebuild poem from the slaves to the master
          {
               std::cout<< elem.second << "\n";
          }
        
  } 
     
   
   

 
  MPI_Finalize();
  return 0;
}