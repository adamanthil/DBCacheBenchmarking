#ifndef PAGE_LAYOUT_H_
#define PAGE_LAYOUT_H_

class PageLayout
{
 public:
  int numPartitions;
  int numFields;
  int* fieldToPartition;
  int* partitionsToBytes;
  int* fieldToByte;
  int* fieldToLoc;
  int numBytesPerRecord;

  PageLayout(int nPartitions, int nFields, int nBytesPerRecord, int* fToP, int* fToB, int* fToL);
  int getNumPartitions() const;
  int getNumFields() const;
  int getFieldsPartition(int fieldNum) const;
  int getFieldsBytes(int fieldNum) const;
  int getNBytesPerRec() const;
  int getFieldLoc(int fieldNum) const;
};

#endif
