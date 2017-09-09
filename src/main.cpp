#include "audio/WAVParser.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
  if(argc < 3)
  {
    std::cerr << "Usage: ./wavparser <wav file> <out file>" << std::endl;
    exit(1);
  }
  
  WAVParser parser;

  std::ifstream i_file(argv[1], std::ios::in | std::ios::binary);
  std::ofstream o_file(argv[2], std::ios::out | std::ios::binary);

  WAVFile w_file = parser.parse(i_file);
  
  w_file.writeData<int16_t>(o_file);
  
  return 0;
}
