#include "Partition.h"

Partition::Partition()
{
  m_numBytes = 0;
  m_currentOffset = 0;
  m_size = 0;
}

void Partition::add(const Attribute * at)
{
  m_fMap[at->qualifiedName()] = m_size;
  m_numBytes += at->size();
}

void Partition::setOffset(int offset)
{
  m_currentOffset = offset;
}

void Partition::calcSize(int numRecords)
{
  m_size = m_numBytes*numRecords;
}

int Partition::getSize()
{
  return m_size;
}

int Partition::bytes()
{
  return m_numBytes;
}

int Partition::start()
{
  return m_currentOffset;
}

int Partition::getFLoc(std::string & fName)
{
  return m_fMap[fName];
}
