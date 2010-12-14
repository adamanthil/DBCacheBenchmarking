#include <stdio.h>
#include <string.h>

#include "TupleStreamReader.h"

TupleStreamReader::TupleStreamReader(MemoryBlock & block) : m_block(block)
{
  m_pos = 0;
  m_nRecs = 0;
  m_layout = NULL;
}

TupleStreamReader::~TupleStreamReader()
{
  delete m_layout;
}

void TupleStreamReader::layout(const MaterializationLayout * layout)
{
  m_layout = layout;
}

void TupleStreamReader::read(Tuple & t)
{
  const Schema * atts = t.schema();
  int totalNumBytes = atts->rsize();
  if (m_layout != NULL)
  {
    int numFields = atts->nitems();
    for (int j = 0; j < numFields; j++)
    {
      std::string fName = atts->at(j)->qualifiedName();
      Partition * part = m_layout->getPartition(fName);
      int partitionStart = part->start();
      int partitionBytes = part->bytes();
      int fieldLoc = part->getFLoc(fName);
      int fieldSize = atts->at(j)->size();
      int offset = partitionStart + partitionBytes*m_nRecs + fieldLoc;
      int readOffset = atts->at(j)->position();
      byte * readInto = &t.m_data[readOffset];
      m_block.get(readInto,offset,fieldSize);
    }
  }
  else
  {
    const Schema * atts = t.schema();
    int offset = totalNumBytes*m_nRecs;
    m_block.get(t.m_data,offset,totalNumBytes);
  }
  m_pos += totalNumBytes;
  m_nRecs++;
}

bool TupleStreamReader::isEndOfStream()
{
  bool ret = (m_nRecs == m_block.getSize());
  return ret;
}

void TupleStreamReader::reset()
{
  m_pos = 0;
  m_nRecs = 0;
}
