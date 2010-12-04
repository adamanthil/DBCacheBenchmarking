#include "PageLayout.h"

PageLayout::PageLayout(int nPartitions, int nFields, int nBytesPerRecord, int* fToP, int* fToB, int* fToL, int* pToL, int* pToB)
{
  numPartitions = nPartitions;
  numFields = nFields;
  fieldToPartition = fToP;
  fieldToByte = fToB;
  fieldToLoc = fToL;
  partitionToLoc = pToL;
  partitionToByte = pToB;
  numBytesPerRecord = nBytesPerRecord;
}

int PageLayout::getPartitionStart(int partition) const
{
  return partitionToLoc[partition];
}

int PageLayout::getPartitionBytes(int partition) const
{
  return partitionToByte[partition];
}

int PageLayout::getNumPartitions() const
{
  return numPartitions;
}

int PageLayout::getNumFields() const
{
  return numFields;
}

int PageLayout::getFieldsPartition(int fieldNum) const
{
  return fieldToPartition[fieldNum];
}

int PageLayout::getFieldsBytes(int fieldNum) const
{
  return fieldToByte[fieldNum];
}

int PageLayout::getNBytesPerRec() const
{
  return numBytesPerRecord;
}

int PageLayout::getFieldLoc(int fieldNum) const
{
  return fieldToLoc[fieldNum];
}
