#include <algorithm>
#include <string.h>

#include "Settings.h"
#include "MergeJoin.h"
#include "BufferManager.h"

MergeJoin::MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild,
		     const Columns & joinColumns)
  : m_write_offset(0), m_merge_with(0)
{
  bool enable = false;

  m_layout[0] = NULL;
  m_layout[1] = NULL;

  Settings::get("partition-materialization", enable);
  if (enable)
    InitializePartitionLayout(lChild, rChild, joinColumns);
  else
    InitializeFlatLayout(lChild, rChild, joinColumns);
}

void MergeJoin::InitializeFlatLayout(IRelationalOperator * lChild, 
				     IRelationalOperator * rChild,
				     const Columns & joinColumns)
{
  m_child[LEFT] = lChild;
  m_child[RIGHT] = rChild;

  /* create (left|right)+(proj|join) schemas. */
  for (int i = 0; i < N_BRANCHES; i++)
    {
      int idx = i * 2;
      Schema * joinSchema = new Schema();

      m_consumed[i] = true;
      m_inBuffer[i] = BufferManager::getInstance()->allocate();
      
      /* create projection & join schemas */
      m_tuple[idx|PROJ].schema(m_child[i]->schema());
      m_tuple[idx|JOIN].schema(m_child[i]->schema());

      for (int j = 0; j < joinColumns.count(); j++)
	{ 
	  const Column * column = joinColumns.at(j);
	  const Schema * schema = m_tuple[idx|PROJ].schema();
	  
	  if (schema->contains(column->m_qualified_name))
	    {
	      const Attribute * a = (*schema)[column->m_qualified_name];
	      m_joinCols[idx|PROJ].push_back(a);
	      m_joinCols[idx|JOIN].push_back(a);
	    }  
	}

      m_tuple[idx|PROJ].m_data = new byte[m_tuple[idx|PROJ].schema()->rsize()];
      m_tuple[idx|JOIN].m_data = new byte[m_tuple[idx|JOIN].schema()->rsize()];

      m_tsr[i] = new TupleStreamReader(*m_inBuffer[i]);
    }
  
  /* create merged schema */
  for (int i = 0; i < N_BRANCHES; i++)
    {
      int idx = i * 2;
      for (int j = 0; j < m_tuple[idx].schema()->nitems(); j++)
	{
	  m_schema.add(m_tuple[idx].schema()->at(j));
	}
    }
  
  m_data = new byte[m_schema.rsize()];
  m_buffer = BufferManager::getInstance()->allocate();
  m_tsw = new TupleStreamWriter(*m_buffer, m_schema.rsize());
}

void MergeJoin::InitializePartitionLayout(IRelationalOperator * lChild, 
					  IRelationalOperator * rChild,
					  const Columns & joinColumns)
{
  std::vector<const Attribute *> layout;
  std::vector<const Attribute *> partition[2];

  m_child[LEFT] = lChild;
  m_child[RIGHT] = rChild;

  /* create (left|right)+(proj|join) schemas. */
  for (int i = 0; i < N_BRANCHES; i++)
    {
      int idx = i * 2;
      Schema * joinSchema = new Schema(); joinSchema->m_partitions = 1;

      layout.clear();
      layout.reserve(m_child[i]->schema()->nitems());

      m_consumed[i] = true;
      m_inBuffer[i] = BufferManager::getInstance()->allocate();

      /* create projection schema */
      m_tuple[idx|PROJ].schema(m_child[i]->schema());
            
      /* retrieve join columns for ith child */
      for (int j = 0; j < joinColumns.count(); j++)
	{ 
	  const Column * column = joinColumns.at(j);
	  const Schema * schema = m_tuple[idx|PROJ].schema();

	  if (schema->contains(column->m_qualified_name))
	    {
	      const Attribute * a = (*schema)[column->m_qualified_name];
	      m_joinCols[idx|PROJ].push_back(a);

	      joinSchema->add(a);
	      m_joinCols[idx|JOIN].push_back(a);
	    }  
	}

      /* create layouts for children. */
      partition[0].clear();
      partition[1].clear();
      m_layout[i] = new MaterializationLayout(2, m_inBuffer[i]->capacity(), 
					      m_tuple[idx|PROJ].schema()->rsize());

      for (int j = 0; j < m_tuple[idx|PROJ].schema()->nitems(); j++)
	{
	  const Attribute * a = m_tuple[idx|PROJ].schema()->at(j);
	  if (!joinColumns.contains(a->qualified_name()))
	    {
	      partition[1].push_back(a);
	    }
	  else
	    {
	      partition[0].push_back(a);
	      layout.push_back(a);
	    }
	}

      m_layout[i]->add(partition[0]);
      m_layout[i]->add(partition[1]);

      /* synchronize schemas for read optimization */
      for (int j = 0; j < partition[1].size(); j++)
	{
	  layout.push_back(partition[1][j]);
	}

      Schema * s = new Schema(&layout);
      s->m_partitions = 3;
      m_tuple[idx|PROJ].schema(s); 
      
      /* create join schema */
      m_tuple[idx|JOIN].schema(joinSchema);

      m_tuple[idx|PROJ].m_data = new byte[m_tuple[idx|PROJ].schema()->rsize()];
      m_tuple[idx|JOIN].m_data = new byte[m_tuple[idx|JOIN].schema()->rsize()];

      m_tsr[i] = new TupleStreamReader(*m_inBuffer[i]);
      m_tsr[i]->layout(m_layout[i]);
      m_child[i]->layout(m_layout[i]);
    }
  
  /* create merged schema */
  for (int i = 0; i < N_BRANCHES; i++)
    {
      int idx = i * 2;
      for (int j = 0; j < m_tuple[idx].schema()->nitems(); j++)
	{
	  m_schema.add(m_tuple[idx].schema()->at(j));
	}
    }
  
  m_data = new byte[m_schema.rsize()];
  m_buffer = BufferManager::getInstance()->allocate();
  m_tsw = new TupleStreamWriter(*m_buffer, m_schema.rsize());
}

MergeJoin::~MergeJoin()
{
  bool enable = false;
  BufferManager * bm = BufferManager::getInstance();

  Settings::get("partition-materialization", enable);

  for (int i = 0; i < N_BRANCHES; i++)
    {
      delete m_layout[i];
      delete m_child[i];
      delete m_tsr[i];
      bm->deallocate(m_inBuffer[i]);
    }
  
  for (int i = 0; i < NCHILD_TUPLES; i++)
    {
      delete [] m_tuple[i].m_data;
      if (enable)
	delete m_tuple[i].schema();
    }

  bm->deallocate(m_buffer);
  delete [] m_data;
  delete m_tsw;

  std::for_each(m_merge_stack.begin(), m_merge_stack.end(), free);
}

void MergeJoin::layout(const MaterializationLayout * layout)
{
  m_tsw->layout(layout);
}

int MergeJoin::compare(const Tuple & lhs, const std::vector<const Attribute *> & lCols,
		       const Tuple & rhs, const std::vector<const Attribute *> & rCols)
{
  for (int i = 0; i < lCols.size(); i++)
    {
      int cmp = 0;
      const Attribute * lField = lCols[i];
      const Attribute * rField = rCols[i];
      
      switch (lField->type())
	{
	case INTEGER: // endianness :(
	  cmp = *(int *)(lhs.m_data + lhs.schema()->offset(lField)) -
	    *(int *)(rhs.m_data + rhs.schema()->offset(rField));
	  break;
	case CHAR:
	case STRING:
	case BIT:
	  cmp = memcmp(lhs.m_data + lhs.schema()->offset(lField),
		       rhs.m_data + rhs.schema()->offset(rField),
		       lField->size());
	  break;
	}
      
      if (cmp)
	{
	  return cmp;
	}
    }
  return 0;
};

bool MergeJoin::hasData(int branch)
{
  /* check if consumed all data in previous pass. */
  if (m_consumed[branch])
    {
      m_eof[branch] = false;
      m_consumed[branch] = false;
      
      m_inBuffer[branch]->clear();
      if (m_child[branch]->moveNext())
	{
	  m_tsr[branch]->reset();
	  m_child[branch]->next(*m_inBuffer[branch]);
	}
      else
	{
	  m_consumed[branch] = true;
	  m_eof[branch] = true;

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

bool MergeJoin::get_tuple(int branch, int tidx, bool peek)
{
    if (!m_eof[branch] && !m_tsr[branch]->isEndOfStream())
    {
      if (peek)
	{
	  m_tsr[branch]->peek(m_tuple[tidx]);
	}
      else
	{
	  m_tsr[branch]->read(m_tuple[tidx]);
	}

      return true;
    }
  return false;
}

void MergeJoin::create_merge_stack()
{
  
  Tuple t;

  t.schema(m_tuple[TLEFT|PROJ].schema());
  t.m_data = m_data; // merged_data > left.proj_data

  m_merge_stack.clear();

  // get first item of merge stack for future comparisons.
  m_tsr[LEFT]->read(t);
  m_tsr[LEFT]->rewind(1); // rewind to push first item unto stack. 
  
  while (!m_eof[LEFT] &&
	 get_tuple(LEFT, TLEFT|PROJ, false) && 
	 compare(t, m_joinCols[TLEFT|PROJ], m_tuple[TLEFT|PROJ], 
		 m_joinCols[TLEFT|PROJ]) == 0)
    {
      // push item unto merge stack.
      byte * data = new byte[m_tuple[TLEFT|PROJ].schema()->rsize()];
      memcpy(data, m_tuple[LEFT].m_data, m_tuple[TLEFT|PROJ].schema()->rsize());
      m_merge_stack.push_back(data);
      
      if (m_tsr[LEFT]->isEndOfStream())
	{
	  m_consumed[LEFT] = true;
	  m_eof[LEFT] = isEmpty(LEFT);
	}
    }

  // rewind to retrieve next item for subsequent processing.
  if (!m_eof[LEFT])
    m_tsr[LEFT]->rewind(1);
}

void MergeJoin::merge()
{
  Tuple t;
  Tuple merged;

  merged.m_data = m_data;
  merged.schema(&m_schema);

  // if stack is empty return 0;
  if (m_merge_stack.empty())
    {
      return;
    }

  t.m_data = m_merge_stack[0];
  t.schema(m_tuple[TLEFT|PROJ].schema()); 
  
  while (!m_eof[RIGHT] && !m_tsw->isStreamFull() &&
	 get_tuple(RIGHT, TRIGHT|PROJ, false))
    {
      // compare tuple from right branch with merge-stack. 
      if (compare(t, m_joinCols[TLEFT|PROJ], m_tuple[TRIGHT|PROJ], m_joinCols[TRIGHT|PROJ]) == 0)
	{
	  // merge item with all items on stack. 
	  for ( ; m_merge_with < m_merge_stack.size() && !m_tsw->isStreamFull(); m_merge_with++)
	    {
	      t.m_data = m_merge_stack[m_merge_with]; 
	      concatenate(merged, t, m_tuple[TRIGHT|PROJ]); 
	      m_tsw->write(merged);
	    }
	  
	  // determine if concatenation is complete. 
	  if (m_merge_with >= m_merge_stack.size())
	    {
	      m_merge_with = 0;
	      
	      // retrieve next tuple from right branch
	      if (m_tsr[RIGHT]->isEndOfStream()) 
		{
		  m_consumed[RIGHT] = true;
		  m_eof[RIGHT] = isEmpty(RIGHT);
		}
	    }
	}
      else
	{
	  m_tsr[RIGHT]->rewind(1);
	}
      
      // determine if merge is complete. remove and free data. 
      if (m_eof[RIGHT] || (get_tuple(RIGHT, TRIGHT|PROJ, true) && 
			   compare(t, m_joinCols[TLEFT|PROJ], 
				   m_tuple[TRIGHT|PROJ], m_joinCols[TRIGHT|PROJ]) != 0))
	{
	  std::for_each(m_merge_stack.begin(), m_merge_stack.end(), free); // free valid in gnu++
	  m_merge_stack.clear();
	  m_merge_with = 0;
	  
	  break;
	}
    }
}

bool MergeJoin::moveNext()
{
  int nrecords = 0;
  int rsize = m_schema.rsize();

  m_write_offset = 0;
  m_tsw->discard(); 
  bool peek = false;

  merge();
  while (!m_tsw->isStreamFull()) 
    {
      // determine if we consumed data from both branches.	
      if (isEmpty(LEFT) || isEmpty(RIGHT))
	break; // terminate loop;
      
      // peek at the current tuple from r-child. 
      get_tuple(RIGHT, TRIGHT|JOIN, true);
      
      int comparison = 0;
      while (!m_eof[LEFT] && get_tuple(LEFT, TLEFT|JOIN, false) &&
	     (comparison = compare(m_tuple[TLEFT|JOIN], m_joinCols[TLEFT|JOIN],
				   m_tuple[TRIGHT|JOIN], m_joinCols[TRIGHT|JOIN])) < 0)
	{
	  // retrieve the next tuple if available.
	  // determine if we reached the end of current page.
	  if (m_tsr[LEFT]->isEndOfStream()) 
	    {
	      m_consumed[LEFT] = true;
	      m_eof[LEFT] = isEmpty(LEFT);
	    }
	}

      if (!m_eof[LEFT])
	m_tsr[LEFT]->rewind(1);
      
      if (!m_eof[LEFT] && comparison == 0)
	{
	  // create merge stack. 
	  create_merge_stack();
	  merge(); // merge tuples from right branch with rewind stack. 

	  // determine if we have reached at eof for right branch
	  if (m_eof[RIGHT] || m_tsw->isStreamFull()) 
	    {
	      break; // terminate loop eof from right branch
	    }
	}

      if (!m_eof[LEFT])
	{
	  while (!m_eof[RIGHT] && get_tuple(RIGHT, TRIGHT|JOIN, false) &&
		 compare(m_tuple[TLEFT|JOIN], m_joinCols[TLEFT|JOIN],
			 m_tuple[TRIGHT|JOIN], m_joinCols[TRIGHT|JOIN]) > 0)
	    {
	      
	      // reached end of current buffer. 
	      if (m_tsr[RIGHT]->isEndOfStream())
		{
		  m_consumed[RIGHT] = true;
		  m_eof[RIGHT] = isEmpty(RIGHT);
		}
	    }

	  if (!m_eof[RIGHT])
	    m_tsr[RIGHT]->rewind(1);
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
