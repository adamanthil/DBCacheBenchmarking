#ifndef MERGE_JOIN_H_
#define MERGE_JOIN_H_

#include <vector>

#include "IRelationalOperator.h"
#include "Tuple.h"
#include "Column.h"

#include "TupleStreamReader.h"
#include "TupleStreamWriter.h"

//typedef std::vector<const Attribute *> SortKey;

typedef std::vector<const Attribute *> ComparisonList;

/**
 * equi-join operator on pre-sorted data.
 */
class MergeJoin : public IRelationalOperator
{
 private:
  enum { LEFT = 0, RIGHT, N_BRANCHES };

  /*
   * TLEFT  = left-child (projection/join) tuple.
   * TRIGHT = right-child (projection/join) tuple.
   */ 
  enum { TLEFT = 0, TRIGHT = 2, NCHILD_TUPLES = 4 }; 
  
  /*
   * PROJ = projection columns.
   * JOIN = retrieve only the columns involed in the join. 
   */
  enum { PROJ = 0, JOIN = 1 }; 

  IRelationalOperator * m_child[N_BRANCHES];

  bool m_eof[N_BRANCHES]; // flag indicating if file has been completely exhausted.
  bool m_consumed[N_BRANCHES];  // flag indicating if current block has been exhausted
  MemoryBlock * m_inBuffer[N_BRANCHES]; // input buffers from left & right branch

  Tuple m_tuple[NCHILD_TUPLES]; // current child tuple being examined

  MemoryBlock * m_buffer; // output buffer.
  byte * m_data; // underlying data-buffer for merged tuple.

  /* rewind stack since operators do not have rewind feature. */
  std::vector<byte *> m_merge_stack;
  int m_merge_with;
  int m_write_offset;
  
  std::vector<const Attribute *> m_joinCols[4];
  Schema m_schema; // projected schema. 
  
  MaterializationLayout * m_layout[N_BRANCHES];
  TupleStreamReader * m_tsr[N_BRANCHES];
  TupleStreamWriter * m_tsw;
 public:
  MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild,
	    const Columns & joinColumns);
  MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild,
	    const Columns & joinColumns, const Columns & projectionList);
  ~MergeJoin();

  virtual const Schema * schema() const;
  virtual void layout(const MaterializationLayout * layout);
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();

 private:
  void InitializePartitionLayout(IRelationalOperator *,
				 IRelationalOperator *,
				 const Columns &);
  void InitializeFlatLayout(IRelationalOperator *, 
			    IRelationalOperator *,
			    const Columns &);

  bool hasData(int);
  bool isEmpty(int branch);
  bool get_tuple(int branch, int tidx, bool peek);
 
  int compare(const Tuple &, const std::vector<const Attribute *> &, 
	      const Tuple &, const std::vector<const Attribute *> &);

  void concatenate(Tuple &, const Tuple &, const Tuple &); 
 void create_merge_stack();
  void merge();
};

#endif
