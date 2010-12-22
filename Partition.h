#ifndef PARTITION_H
#define PARTITION_H

#include <map>
#include <string>
#include <vector>

#include "Attribute.h"

class Partition
{
  private:
    int m_numBytes;
    int m_currentOffset;
    int m_size;
    std::map<std::string,int> m_fMap;
    std::vector<const Attribute * > m_items;
  public:
    Partition();
    ~Partition();
    void add(const Attribute * at);
    void setOffset(int offset);
    void calcSize(int numRecords);
    int getSize() const;
    int start() const;
    int bytes() const;
    int getFLoc(std::string & fName);

    inline int nitems() const { return m_items.size(); }
    inline const Attribute * attribute(int idx) const { return m_items[idx]; }
};

#endif
