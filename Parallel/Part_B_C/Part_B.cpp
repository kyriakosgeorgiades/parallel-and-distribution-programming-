#include "omp.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include <random>
#include <iomanip>
#include "states.h"

int main (void)
{
//Setting up random generator of range 1-5 for particle position alteration variable
  const int range_from =1;
  const int range_to = 5;
  std::random_device                  rand_dev;
  std::mt19937                        generator(rand_dev());
  std::uniform_int_distribution<int>  increase(range_from,range_to);  //increase(generator)
  std::uniform_int_distribution<int>  symbol(0,1); //symbol(generator) if 0 decremented if 1 incremented
  
  int objects[10][3] ={
     {5, 14, 10},
     {7, -8, -14},
     {-2, 9, 8},
     {15, -6, 3},
     {12, 4, -5},
     {4, 20, 17},
     {-16, 5, -1},
     {-11, 3, 16},
     {3, 10, 19},
     {-16, 7, 4}
    };
    
    
    int alert_pos = 2; //Particle position alteration varible starting at 2
    
    print_original(objects);
  
  
  for (int i = 0; i<50; i++){ //Starting the 50 steps loop
    
    #pragma omp parallel for //Creating parallel section for the movement of the objects
      for (int x = 0; x<10; x++)
      {
        for (int y = 0 ; y<3; y++)
          {
            
            if (symbol(generator) == 1){ //Using the generator to generate to either minus or plus for the next move
              objects[x][y] += alert_pos; //Increasing the movement if 1
            }else{
            objects[x][y] -= alert_pos; //Decreasinng the movement if 0
            }
          }
      }
    
    if (i == 25) { //On the 25th state printing the state of the objects on X, Y ,Z axis
      print_25(objects);
    }
    alert_pos += increase(generator); //Increase the particle position alteration variable using the random generator of range 1 to 5
  
  
  }
  //Printing the last state of the objects
  print_final(objects);
  
  
  
//#############################################################################################################################################
  
  //Section C of B moving towards the center
  int axis_X, axis_Y, axis_Z = 0;
  
  //Adding all the position of the particles to get the center of them at each axis X,Y and Z
  for (int i = 0; i<10; i++)
    for(int j=0; j<3; j++){
      switch(j) {
        case 0:
          axis_X += objects[i][j]; //Adding for axis X
          break;
        case 1:
          axis_Y += objects[i][j]; //Adding for axis Y
          break;
        case 2:
          axis_Z += objects[i][j]; //Adding for axis Z
          break;
      }
    }
    
  //Getting the center location 
  axis_X /= 10;
  axis_Y /= 10;
  axis_Z /= 10;
  
  std::cout << std::endl << std::endl;
  std::cout<< "Center of particles" << std::endl;
  std::cout<< "=============================" << std::endl;
  std::cout << "X : " << axis_X << "| Y : " << axis_Y << "| Z : " << axis_Z << std::endl;
  std::cout << std::endl << std::endl;
  
  //Iniz the particle position aleration variable back to 2
  alert_pos = 2;
  
  std::cout<< "=============================" << std::endl;
  std::cout<< "MOVING TOWARDS THE CENTER" << std::endl;
  std::cout<< "=============================" << std::endl;
  std::cout << std::endl << std::endl;
  print_original(objects);
  
  for (int i = 0; i<50; i++)
  {
    #pragma omp parallel for //Creating parallel section for the movement of the objects
    for ( int x = 0; x<10; x++)
    {
      for ( int y = 0; y<3; y++)
      {
        switch(y){//Checking for each axis X, Y and then Z if they are smaller or greater than the center point. And then increasing or decreasing depending on the if statement.
          case 0:
            if ( objects[x][y] < axis_X) //Current object's X smaller than X CENTER
            {
              objects[x][y] += alert_pos; //Incremation of position current object's X
            
            } else if ( objects[x][y] > axis_X)//Current object's X greater than X CENTER
              {
                objects[x][y] -= alert_pos;//Subtraction of position current object's X
              }
          break;
          case 1:
            if ( objects[x][y] < axis_Y) //Current object's Y smaller than Y CENTER
            {
              objects[x][y] += alert_pos; //Incremation of position current object's Y
            
            } else if ( objects[x][y] > axis_Y) //Current object's Y greater than Y CENTER
              {
                objects[x][y] -= alert_pos; //Subtraction of position current object's Y
              }
          break;
          case 2:
            if ( objects[x][y] < axis_Z) //Current object's Z smaller than Z CENTER
            {
              objects[x][y] += alert_pos; //Incremation of position current object's Z
            
            } else if ( objects[x][y] > axis_Z) //Current object's Z greater than Z CENTER
              {
                objects[x][y] -= alert_pos; //Subtraction of position current object's Z
              }
          break;
        }
      }  
    }
    if (i == 25) { //On the 25th state printing the state of the objects on X, Y ,Z axis
      print_25(objects);
    }
    alert_pos += increase(generator); //Increase the particle position alteration variable using the random generator of range 1 to 5
  
  }
  
  
  //Printing the last state of the objects
  print_final(objects);
  std::cout<< "Center of original particles" << std::endl;
  std::cout<< "=============================" << std::endl;
  std::cout << "X : " << axis_X << "| Y : " << axis_Y << "| Z : " << axis_Z << std::endl;
  
  
  //Adding all the position of the particles to get the center of them at each axis X,Y and Z
  for (int i = 0; i<10; i++)
    for(int j=0; j<3; j++){
      switch(j) {
        case 0:
          axis_X += objects[i][j]; //Adding for axis X
          break;
        case 1:
          axis_Y += objects[i][j]; //Adding for axis Y
          break;
        case 2:
          axis_Z += objects[i][j]; //Adding for axis Z
          break;
      }
    }
    
  //Getting the new center location 
  axis_X /= 10;
  axis_Y /= 10;
  axis_Z /= 10;
  
  std::cout<< "=============================" << std::endl;
  std::cout<< "Center of new particles" << std::endl;
  std::cout<< "=============================" << std::endl;
  std::cout << "X : " << axis_X << "| Y : " << axis_Y << "| Z : " << axis_Z << std::endl;
  std::cout << std::endl << std::endl;
  
  
  
  
  
  return 0;

}
  