#ifndef PARTITION_H
#define PARTITION_H

#include <map>
#include <string>

#include "Attribute.h"

class Partition
{
  private:
    int m_numBytes;
    int m_currentOffset;
    int m_size;
    std::map<std::string,int> m_fMap;
  public:
    Partition();
    void add(const Attribute * at);
    void setOffset(int offset);
    void calcSize(int numRecords);
    int getSize();
    int start();
    int bytes();
    int getFLoc(std::string & fName);
};

#endif
