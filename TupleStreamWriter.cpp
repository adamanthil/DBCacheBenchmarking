#include <stdio.h>
#include <string.h>

#include "TupleStreamWriter.h"

TupleStreamWriter::TupleStreamWriter(MemoryBlock & block, size_t record_size) : m_block(block)
{
  m_pos = 0;
  m_layout = NULL;
  m_nRecs = 0;
  m_record_size = record_size;
}

TupleStreamWriter::~TupleStreamWriter()
{
  delete m_layout;
}

void TupleStreamWriter::layout(const MaterializationLayout * layout)
{
  m_layout = layout;
}

void TupleStreamWriter::discard()
{
  m_nRecs = 0;
  m_pos = 0;
}

bool TupleStreamWriter::isStreamFull()
{
  return ((m_block.capacity() - m_pos) < m_record_size);
}

void TupleStreamWriter::write(Tuple & t)
{
  const Schema * atts = t.schema();
  int numFields = atts->nitems();
  if (m_layout != NULL)
  {
    for (int j = 0; j < numFields; j++)
    {
      const Attribute * a = atts->at(j);
      std::string fName = a->qualifiedName();
      Partition * part = m_layout->getPartition(fName);
      int partitionStart = part->start();
      int partitionByte = part->bytes();
      int fieldLoc = part->getFLoc(fName);
      int fSize = a->size();
      int offset = partitionStart + partitionByte*m_nRecs + fieldLoc;
      char * fVal = new char[fSize];
      t.value(fVal,*a);
      m_block.put((byte*)fVal, offset, fSize);
    }
  }
  else
  {
    int totalNumBytes = atts->rsize();
    for (int k = 0; k < numFields; k++)
    {
      const Attribute * a = atts->at(k);
      int fSize = a->size();
      char * fVal = new char[fSize];
      t.value(fVal,*a);
      int p = a->position();
      int offset = totalNumBytes*m_nRecs + p;
      m_block.put((byte*)fVal, offset, fSize);
    }
  }
  m_pos += m_record_size;
  m_nRecs++;
}
