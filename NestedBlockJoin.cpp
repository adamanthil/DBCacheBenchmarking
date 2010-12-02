#include "NestedBlockJoin.h"

#include "BufferManager.h"

NestedBlockJoin::NestedBlockJoin(IRelationalOperator & r1, IRelationalOperator & r2,
		BooleanExpression & m_boolExp) {
	m_block = BufferManager::getInstance()->allocate();
	m_r1 = &r1;
	m_r2 = &r2;
}

NestedBlockJoin::~NestedBlockJoin() {
	if (m_block != NULL) {
		BufferManager::getInstance()->deallocate(m_block);
    }
}

void NestedBlockJoin::next(MemoryBlock & block) {
	block.copy(*m_block);
}

bool NestedBlockJoin::moveNext() {
	// If we reached the end of the outer loop, return false
	if(!m_r1->moveNext()) {	
		return false;
	}
	else {	// Otherwise, compare to next block of inner relation
		// Reset inner loop if we reached the end
		if (!m_r2->moveNext()) {	
			m_r2->reset();
		}
		
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
}

void NestedBlockJoin::reset() {
	// Reset both relations to reset join
	m_r1->reset();
	m_r2->reset();
}