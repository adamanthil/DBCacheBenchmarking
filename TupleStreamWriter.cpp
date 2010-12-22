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
  //delete m_layout;
}

void TupleStreamWriter::layout(const MaterializationLayout * layout)
{
  m_layout = layout;
}

void TupleStreamWriter::discard()
{
  m_nRecs = 0;
  m_pos = 0;
  m_block.clear();
}

bool TupleStreamWriter::isStreamFull()
{
  return ((m_block.capacity() - m_pos) < m_record_size);
}

void TupleStreamWriter::write(Tuple & t)
{
  if (m_layout != NULL)
  {    
    int tuple_offset = 0;
    for (int i = 0; i < 2 /* m_layout->npartitions() */; i++)
      {
	const Partition * p = m_layout->partition(i);
	int offset = p->start() + p->bytes() * m_nRecs;

	/* If we cascaded the interesting fields down, we could optimize
	   the writes as below. However, testing has shown this will not 
	   improve performance beyond that of a n-ary layout. Therefore no 
	   more development on this optimization 
       
	m_block.put(t.m_data + tuple_offset, offset, p->bytes());
	tuple_offset += p->bytes();
	*/

	for (int j = 0; j < p->nitems(); j++)
	  {
	    const Attribute * a = p->attribute(j);
	    
	    m_block.put(t.m_data + t.schema()->offset(a), offset, a->size());
	    offset += a->size();
	  }
      }
  }
  else
  {
    int offset = t.schema()->rsize()*m_nRecs;
    m_block.put((byte*)t.m_data, offset, t.schema()->rsize());
  }

  m_pos += m_record_size;
  m_nRecs++;
  m_block.setSize(m_nRecs);
}
