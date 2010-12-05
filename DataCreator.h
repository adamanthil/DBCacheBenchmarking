#ifndef DATA_CREATOR_H_
#define DATA_CREATOR_H_

#include <string>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <utility>

class DataCreator
{
  public:
  static void Create(const std::string & config_file);
  static void CreateDB(const std::string & config_file,bool makeHuman);
};

#endif
