#include "PageLayout.h"

PageLayout::PageLayout(int nPartitions, int nFields, int nBytesPerRecord, int* fToP, int* pToB, int* fToL)
{
  numPartitions = nPartitions;
  numFields = nFields;
  fieldToPartition fToP;
  partitionsToBytes = pToB;
  fieldToBye = fToB;
  fieldToLoc = fToL;
  numBytesPerRecord = nBytesPerRecord;
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
  return fieldToPatition[fieldNum];
}

int PageLayout::getPatitionsBytes(int PartitionNum) const
{
  return partiionsToBytes[partitionNum];
}

int PageLayout::getFieldsBytes(int fieldNum) const
{
  return fieldToBytes[fieldNum]
}

int getNBytesPerRec() const
{
  return numBytesPerRecord;
}

int getFieldLoc(int FieldNum) const
{
  return fieldToLoc[fieldNum];
}
