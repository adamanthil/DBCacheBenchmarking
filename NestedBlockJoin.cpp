#include "NestedBlockJoin.h"

#include "BufferManager.h"

NestedBlockJoin::NestedBlockJoin(IRelationalOperator * r1, IRelationalOperator * r2,
		WhereClause * clause) {
	m_buffer = BufferManager::getInstance()->allocate();
	m_r1 = r1;
	m_r2 = r2;
	m_clause = clause;
	m_schema = concatSchema(r1->schema(), r2->schema());
	m_data = new byte[m_schema->rsize()];
	m_leftBlockRid = 0;
	m_rightBlockRid = 0;
}

NestedBlockJoin::~NestedBlockJoin() {
	if (m_buffer != NULL) {
		BufferManager::getInstance()->deallocate(m_buffer);
    }

	delete[] m_data;
	delete m_schema;
}

const Schema * NestedBlockJoin::schema() const {
  return m_schema;
}

void NestedBlockJoin::next(MemoryBlock & block) {
	block.copy(*m_buffer);
}
/*
void NestedBlockJoin::nextLeftTuple(Tuple * tuple) {
	
}

void NestedBlockJoin::nextRightTuple(Tuple * tuple) {
	if(m_rightBlockRid < m_rightBlock->size()) {
		
	}
	
}*/

bool NestedBlockJoin::moveNext() {
	/*
	int nrecords = 0;
	int offset = 0;
	int available = m_buffer->capacity();
	
	Tuple tuple;
	byte buffer[512]; // TODO: should be dynamically allocated
	tuple.m_data = buffer;
  	
	m_buffer->clear();
	
	
	
	// Reset inner loop & increment outer loop if we reached the end
	if (!m_r2->moveNext()) {	
		m_r2->reset();
		m_rightBlock = m_r2->next();
		m_rightBlockRid = 0;
		
		m_r1->moveNext();
		m_leftBlock = m_r1->next();
	}
	
	else {	// Otherwise, get next inner loop block
		m_rightBlock = m_r2->next();
	}
*/
  /*
		while (m_buffer.full()== false && m_outer.moveNext()) {
      		MemoryBlock b = m_outer.next();
      		foreach (Tuple t in inner_block) {

	  foreach (Tuple t0 in b)
	    {
	      if (match(t, t0))
		{
		  m_block.add(t, t0, attributes);
		}
	    }
	}
    }

	if (m_outer.is_eof())
	  {
	    m_inner_block = m_inner.next();
	  }
    } */

	//m_buffer->setSize(nrecords);
	//return  nrecords > 0;
}

void NestedBlockJoin::reset() {
	// Reset both relations to reset join
	m_r1->reset();
	m_r2->reset();
}
