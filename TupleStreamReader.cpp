#include <stdio.h>
#include <string.h>

#include "TupleStreamReader.h"

TupleStreamReader::TupleStreamReader(MemoryBlock & block) : m_block(block)
{
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

void TupleStreamReader::peek(Tuple & t)
{
  read(t);
  m_nRecs--;
}

void TupleStreamReader::read(Tuple & t)
{
  if (m_layout != NULL)
  {
<<<<<<< HEAD
    std::set<Partition *> pVisited;
    std::set<Partition *>::iterator it;
    int numFields = atts->nitems();
    for (int j = 0; j < numFields; j++)
    {
      std::string fName = atts->at(j)->qualifiedName();
      Partition * part = m_layout->getPartition(fName);
      it = pVisited.find(part);
      if (it == pVisited.end())
      {
      pVisited.insert(part);
      int partitionStart = part->start();
      int partitionBytes = part->bytes();
      int offset = partitionStart + partitionBytes;
      int readOffset = atts->offset(fName);
      byte * readInto = &t.m_data[readOffset];
      m_block.get(readInto,offset,partitionBytes);
      }
    }
  }
  else
  {
    const Schema * atts = t.schema();
    int totalNumBytes = atts->rsize();
    int offset = totalNumBytes*m_nRecs;
   
    m_block.get(t.m_data,offset,totalNumBytes);
  }
  m_nRecs++;
}

bool TupleStreamReader::isEndOfStream()
{
  bool ret = (m_nRecs == m_block.getSize());
  return ret;
}

void TupleStreamReader::rewind(int nback)
{
  m_nRecs -= nback;
  if (m_nRecs < 0)
    {
      m_nRecs = 0;
    }
}

void TupleStreamReader::reset()
{
  m_nRecs = 0;
}
