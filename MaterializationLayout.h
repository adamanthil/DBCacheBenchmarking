#ifndef MATERIALIZATION_LAYOUT_H_
#define MATERIALIZATION_LAYOUT_H_

#include <map>
#include <vector>
#include "Partition.h"
#include "Schema.h"

class MaterializationLayout
{
 private:
  int m_capacity;
  int m_currentPartition;
  int m_recordSize;
  int m_currentOffset;
  Partition * m_partitions;
  std::map<std::string,Partition*> m_pMap;
 public:
  MaterializationLayout(int n_partitions, size_t capacity, size_t recordSize);
  ~MaterializationLayout();
  void add(const std::vector<const Attribute *> & atts);
  Partition * getPartition(std::string column) const;

  inline const Partition * partition(int i) const { return &m_partitions[i]; };
  inline int npartitions() const { return m_currentPartition; }
};

#endif
