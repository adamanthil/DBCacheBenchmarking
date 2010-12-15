#ifndef MERGE_JOIN_H_
#define MERGE_JOIN_H_

#include <vector>

#include "IRelationalOperator.h"
#include "Tuple.h"
#include "Column.h"

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
   * PROJ = retrieve all columns involved in the projected tuple.
   * JOIN = retrieve only the columns involed in the join. 
   */
  enum subset_t { PROJ = 0, JOIN = 1 }; 

  IRelationalOperator * m_child[N_BRANCHES];

  int m_total; // total records created as result of join.

  int m_next[N_BRANCHES]; // next record id to consume.
  bool m_consumed[N_BRANCHES];  // flag indicating if current block has been exhausted
  MemoryBlock * m_inBuffer[N_BRANCHES]; // input buffers from left & right branch
  Tuple m_tuple[NCHILD_TUPLES]; // current child tuple being examined

  MemoryBlock * m_buffer; // output buffer.
  byte * m_data; // underlying data-buffer for merged tuple.

  std::vector<byte *> m_merge_stack;
  int m_merge_with;
  int m_write_offset;
  
  Columns m_joinCols[N_BRANCHES]; // columns involved in equi-join 

  Schema m_schema; // projected schema. 
  
 public:
  MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild,
	    const Columns & joinColumns);
  MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild,
	    const Columns & joinColumns, const Columns & projectionList);
  ~MergeJoin();

  virtual const Schema * schema() const;
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();

 private:
  bool hasData(int);
  bool isEmpty(int branch);

  bool get_tuple(int branch, int tidx);
  // TODO: another cheat. 
  int compare(const Tuple &, const Columns *, const Tuple &, const Columns *);
  void concatenate(Tuple &, const Tuple &, const Tuple &);
  void create_merge_stack();
  int merge(size_t);
};

#endif
