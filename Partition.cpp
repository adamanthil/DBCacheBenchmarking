#include "Partition.h"

Partition::Partition()
{
  m_numBytes = 0;
  m_currentOffset = 0;
  m_size = 0;
}

void Partition::add(const Attribute * at)
{
  m_fMap[at->qualifiedName()] = m_numBytes;
  m_numBytes += at->size();
  m_items.push_back(at);
}

void Partition::setOffset(int offset)
{
  m_currentOffset = offset;
}

void Partition::calcSize(int numRecords)
{
  m_size = m_numBytes*numRecords;
}

int Partition::getSize() const
{
  return m_size;
}

int Partition::bytes() const
{
  return m_numBytes;
}

int Partition::start() const 
{
  return m_currentOffset;
}

int Partition::getFLoc(std::string & fName) 
{
  return m_fMap[fName];
}
