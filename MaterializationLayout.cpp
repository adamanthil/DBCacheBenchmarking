#include "MaterializationLayout.h"

/**
 * Constructor
 */
MaterializationLayout::MaterializationLayout(int n_partitions, size_t capacity, size_t recordSize)
{
  m_capacity = capacity;
  m_partitions = new Partition[n_partitions];
  m_currentPartition = 0;
  m_recordSize = recordSize;
  m_currentOffset = 0;
}

MaterializationLayout::~MaterializationLayout()
{
  delete m_partitions;
}

void MaterializationLayout::add(const std::vector<const Attribute *> & atts)
{
  for (int i = 0; i < atts.size(); i++)
  {
    const Attribute * curAt = atts[i];
    m_partitions[m_currentPartition].add(curAt);
    m_pMap[curAt->qualifiedName()] = &m_partitions[m_currentPartition];
  }
  
  int numRecords = m_capacity/m_recordSize;

  m_partitions[m_currentPartition].setOffset(m_currentOffset);
  m_partitions[m_currentPartition].calcSize(numRecords);
  m_currentOffset += m_partitions[m_currentPartition].bytes();
}

Partition * MaterializationLayout::getPartition(std::string column) const
{
  return (*m_pMap.find(column)).second;
}
