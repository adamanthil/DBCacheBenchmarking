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
  //delete m_layout;
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
    int tuple_offset = 0;
    
    for (int i = 0; i < 2 /* m_layout->npartitions() */; i++)
      {
	const Partition * p = m_layout->partition(i);

	// all or none
	if ((t.schema()->m_partitions & (i+1)) == 0)
	  continue;
	
	int partition_offset = p->start() + p->bytes() * m_nRecs;
	m_block.get(t.m_data + tuple_offset, partition_offset, p->bytes());
	tuple_offset += p->bytes();
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
