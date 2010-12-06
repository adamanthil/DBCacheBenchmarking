#include <algorithm>

#include "MergeJoin.h"
#include "BufferManager.h"

MergeJoin::MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild)
  : m_write_offset(0), m_merge_with(0), m_total(0)
{
  m_child[LEFT] = lChild;
  m_child[RIGHT] = rChild;

  for (int i = 0; i < N_BRANCHES; i++)
    {
      m_tuple[i].schema(m_child[i]->schema());
      m_tuple[i].m_data = new byte[m_tuple[i].schema()->rsize()];
      m_consumed[i] = true;
      m_inBuffer[i] = BufferManager::getInstance()->allocate();
    }

  for (int i = 0; i < N_BRANCHES; i++)
    {
      for (int j = 0; j < m_tuple[i].schema()->nitems(); j++)
	{
	  m_schema.add(m_tuple[i].schema()->at(j));
	}
    }

  m_data = new byte[m_schema.rsize()];
  m_buffer = BufferManager::getInstance()->allocate();
}

MergeJoin::~MergeJoin()
{
  BufferManager * bm = BufferManager::getInstance();

  for (int i = 0; i < N_BRANCHES; i++)
    {
      delete m_child[LEFT];
      delete [] m_tuple[i].m_data;
      bm->deallocate(m_inBuffer[i]);
    }

  bm->deallocate(m_buffer);
  delete [] m_data;

  std::for_each(m_merge_stack.begin(), m_merge_stack.end(), free);
}

int MergeJoin::compare(const Tuple & t0, const Tuple & t1, int fid)
{
  const Attribute * l = t0.schema()->at(0);
  const Attribute * r = t1.schema()->at(0); // TODO: hard-coded values. 

  int a = 0;
  int b = 0;

  t0.value(&a, *l);
  t1.value(&b, *r);
  
  return a - b;
}

bool MergeJoin::hasData(int branch)
{
  /* check if consumed all data in previous pass. */
  if (m_consumed[branch])
    {
      m_next[branch] = 0;
      m_consumed[branch] = false;

      m_inBuffer[branch]->clear();
      if (m_child[branch]->moveNext())
	m_child[branch]->next(*m_inBuffer[branch]);
      else
	{
	  m_consumed[branch] = true;
	  m_next[branch] = -1;
	  return false;
	}
    }

  return true;
}

bool MergeJoin::isEmpty(int branch)
{
  return !hasData(branch);
}

void MergeJoin::concatenate(Tuple & dst, const Tuple & s, const Tuple & t)
{
  memcpy(dst.m_data, s.m_data, s.schema()->rsize());
  memcpy(dst.m_data + s.schema()->rsize(), t.m_data, t.schema()->rsize());
    
  /*
  std::cerr << "concat("; s.dump(std::cerr, '|', ';'); t.dump(std::cerr, '|', ')');
  std::cerr << "=";
  dst.schema(&m_schema); dst.dump(std::cerr);
  */
}

bool MergeJoin::get_tuple(int branch)
{
  if (m_next[branch] != -1 && m_next[branch] < m_inBuffer[branch]->getSize())
    {
      m_inBuffer[branch]->get(m_tuple[branch].m_data, 
			      m_next[branch] * m_tuple[branch].schema()->rsize(), 
			      m_tuple[branch].schema()->rsize());
      return true;
    }
  return false;
}

void MergeJoin::create_merge_stack()
{
  
  Tuple t;

  t.schema(m_child[LEFT]->schema());
  t.m_data = m_data;

  m_merge_stack.clear();

  // get first item of merge stack for future comparisons.
  m_inBuffer[LEFT]->get(m_data, m_next[LEFT] * m_tuple[LEFT].schema()->rsize(), 
			m_tuple[LEFT].schema()->rsize());
  
  while (get_tuple(LEFT) && compare(t, m_tuple[LEFT], 0) == 0)
    {
      // push item unto merge stack.
      byte * data = new byte[m_tuple[LEFT].schema()->rsize()];
      memcpy(data, m_tuple[LEFT].m_data, m_tuple[LEFT].schema()->rsize());
      m_merge_stack.push_back(data);

      if (++m_next[LEFT] >= m_inBuffer[LEFT]->getSize())
	{
	  m_consumed[LEFT] = true;
	  m_next[LEFT] = isEmpty(LEFT) ? -1 : 0;
	}
      
      if (m_next[LEFT] == -1)
	{
	  break;
	}
    }
}

int MergeJoin::merge(size_t available)
{
  int nrecords = 0;
  int rsize = m_schema.rsize();
  int used = 0;
  Tuple t;

  Tuple merged;

  merged.m_data = m_data;
  merged.schema(&m_schema);

  // if stack is empty return 0;
  if (m_merge_stack.empty())
    {
      return 0;
    }

  t.m_data = m_merge_stack[0];
  t.schema(m_child[LEFT]->schema());

  while (m_next[RIGHT] != -1 && available >= rsize &&
	 get_tuple(RIGHT))
    {
      // compare tuple from right branch with merge-stack. 
      if (compare(t, m_tuple[RIGHT], 1) == 0)
	{

	  // merge item with all items on stack. 
	  for ( ; m_merge_with < m_merge_stack.size() && rsize <= available; m_merge_with++)
	    {
	      t.m_data = m_merge_stack[m_merge_with]; 
	      concatenate(merged, t, m_tuple[RIGHT]); 
	      m_buffer->put(m_data, m_write_offset, m_schema.rsize());
	      m_write_offset += rsize;

	      used += rsize;
	      available -= rsize;
	      nrecords++; m_total++;
	    }
	  
	  // determine if concatenation is complete. 
	  if (m_merge_with >= m_merge_stack.size())
	    {
	      m_merge_with = 0;
	      
	      // retrieve next tuple from right branch
	      if (++m_next[RIGHT] >= m_inBuffer[RIGHT]->getSize())
		{
		  m_consumed[RIGHT] = true;
		  m_next[RIGHT] = isEmpty(RIGHT) ? -1 : 0;
		}
	    }
	} 
      
      // determine if merge is complete. remove and free data. 
      if (m_next[RIGHT] == -1 || (get_tuple(RIGHT) && compare(t, m_tuple[RIGHT], 1) != 0))
	{
	  std::for_each(m_merge_stack.begin(), m_merge_stack.end(), free); // free valid in gnu++
	  m_merge_stack.clear();
	  m_merge_with = 0;
	  break;
	}
    }
  m_buffer->setSize(nrecords + m_buffer->getSize());
     
  return used;
}

bool MergeJoin::moveNext()
{
  int available = m_buffer->capacity();
  int nrecords = 0;
  int rsize = m_schema.rsize();

  m_write_offset = 0;
  m_buffer->clear();

  available -= merge(available);
  while (available >= rsize)
    {
      // determined if we consumed data from both branches.	
      if (isEmpty(LEFT) || isEmpty(RIGHT))
	break; // terminate loop;

      get_tuple(RIGHT);

      // grab the current tuple. 
      int comparison = 0;
      while (m_next[LEFT] != -1 && get_tuple(LEFT) &&
	     (comparison = compare(m_tuple[LEFT], m_tuple[RIGHT], 1)) < 0)
	{
	  // retrieve the next tuple if available.
	  // determine if we reached the end of current page.
	  if (++m_next[LEFT] >= m_inBuffer[LEFT]->getSize())
	    {
	      m_consumed[LEFT] = true;
	      m_next[LEFT] = isEmpty(LEFT) ? -1 : 0;
	    }
	}
      
      if (m_next[LEFT] != -1 && comparison == 0)
	{
	  // create merge stack. 
	  create_merge_stack();
	  available -= merge(available); // merge tuples from right branch with rewind stack. 

	  // determine if we have reached at eof for right branch
	  if (m_next[RIGHT] == -1 || available < rsize) 
	    {
	      break; // terminate loop eof from right branch
	    }
	}

      while (m_next[LEFT] != -1 && get_tuple(RIGHT) &&
	     compare(m_tuple[LEFT], m_tuple[RIGHT], 1) > 0)
	{
	  // reached end of current buffer. 
	  if (++m_next[RIGHT] >= m_inBuffer[RIGHT]->getSize())
	    {
	      m_consumed[RIGHT] = true;
	      m_next[RIGHT] = !isEmpty(RIGHT) ? 0 : -1;
	    }
	} 
    }

  return m_buffer->getSize() > 0;
}

void MergeJoin::next(MemoryBlock & buffer)
{
  buffer.copy(*m_buffer);
}


const Schema * MergeJoin::schema() const 
{
  return &m_schema;
}
 
void MergeJoin::reset()
{
}
