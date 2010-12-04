#ifndef PAGE_LAYOUT_H_
#define PAGE_LAYOUT_H_

class PageLayout
{
 public:
  int numPartitions;
  int numFields;
  int* fieldToPartition;
  int* partitionToByte;
  int* partitionToLoc;
  int* fieldToByte;
  int* fieldToLoc;
  int numBytesPerRecord;

  PageLayout(int nPartitions, int nFields, int nBytesPerRecord, int* fToP, int* fToB, int* fToL, int* pToB, int* pToL);
  int getPartitionStart(int partition) const;
  int getPartitionBytes(int partition) const;
  int getNumPartitions() const;
  int getNumFields() const;
  int getFieldsPartition(int fieldNum) const;
  int getFieldsBytes(int fieldNum) const;
  int getNBytesPerRec() const;
  int getFieldLoc(int fieldNum) const;
};

#endif
