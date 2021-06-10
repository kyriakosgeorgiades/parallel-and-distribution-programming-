void print_state(int (&objects)[10][3])
{

  
    //Printing the first state of the 10 objects
    for (int i = 0; i!=10; i++)
    {
      for (int x = 0; x<3; x++)
          std::cout << std::setw(8) << objects[i][x] << "|";
      std::endl( std::cout);
    }

}


void print_original(int (&objects)[10][3])
{
  std::cout<< "ORIGINAL STATE OF THE OBJECTS" << std::endl;
  std::cout<< "=============================" << std::endl; 
  print_state(objects);  
  std::cout<< "=============================" << std::endl;
}


void print_25(int (&objects)[10][3])
{
  std::cout<< "25th STATE OF THE OBJECTS" << std::endl;
  std::cout<< "=============================" << std::endl;
  print_state(objects);    
  std::cout<< "=============================" << std::endl;
}

void print_final(int (&objects)[10][3])
{
  std::endl( std::cout);
  std::cout<< "50th STATE OF THE OBJECTS" << std::endl;
  std::cout<< "=============================" << std::endl;
  print_state(objects);    
  std::cout<< "=============================" << std::endl;
}