#include "NestedBlockJoin.h"

#include "BufferManager.h"

NestedBlockJoin::NestedBlockJoin(const IRelationalOperator * r1, const IRelationalOperator * r2,
		BooleanExpression & m_boolExp) {
	m_block = BufferManager::getInstance()->allocate();
	m_r1 = r1;
	m_r2 = r2;
	m_schema = schema;
	m_clause = clause;
	m_data = new byte[m_schema->rsize()];
}

NestedBlockJoin::~NestedBlockJoin() {
	if (m_block != NULL) {
		BufferManager::getInstance()->deallocate(m_block);
    }
}

const Schema * SequentialScan::schema() const {
  return m_schema;
}

void NestedBlockJoin::next(MemoryBlock & block) {
	block.copy(*m_block);
}

bool NestedBlockJoin::moveNext() {
	
	int nrecords = 0;
	int offset = 0;
	int available = m_buffer->capacity();
	
	Tuple tuple;
	byte buffer[256]; // TODO:
	tuple.m_data = buffer;
  	
	m_buffer->clear();
	
	// Reset inner loop if we reached the end
	if (!m_r2->moveNext()) {	
		m_r2->reset();
	}
	
	else {	// Otherwise, compare to next block of inner relation

		
		// Match tuples
		do {
			
		} while(m_r2->moveNext());
		
	}

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

	m_buffer->setSize(nrecords);
	return  nrecords > 0;
}

void NestedBlockJoin::reset() {
	// Reset both relations to reset join
	m_r1->reset();
	m_r2->reset();
}