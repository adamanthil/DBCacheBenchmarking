#ifndef MATERIALIZATION_LAYOUT_H_
#define MATERIALIZATION_LAYOUT_H_

#include <map>
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
  void add(Schema * atts);
  Partition * getPartition(std::string column) const;
};

#endif
