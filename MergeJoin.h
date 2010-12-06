#ifndef MERGE_JOIN_H_
#define MERGE_JOIN_H_

#include <vector>

#include "IRelationalOperator.h"
#include "Tuple.h"

//typedef std::vector<const Attribute *> SortKey;

typedef std::vector<const Attribute *> ComparisonList;

/**
 * equi-join operator on pre-sorted data.
 */
class MergeJoin : public IRelationalOperator
{
 private:
  enum { LEFT = 0, RIGHT, N_BRANCHES };

  IRelationalOperator * m_child[N_BRANCHES];

  int m_total; // total records created as result of join.

  int m_next[N_BRANCHES]; // next record id to consume.
  bool m_consumed[N_BRANCHES];  // 
  Tuple m_tuple[N_BRANCHES]; // current child tuple being examined
  MemoryBlock * m_inBuffer[N_BRANCHES];

  //Tuple m_t;
  MemoryBlock * m_buffer;
  byte * m_data; // underlying buffer for merged tuple.

  std::vector<byte *> m_merge_stack;
  int m_merge_with;
  int m_write_offset;

  Schema m_schema;
  
 public:
  MergeJoin(IRelationalOperator * lChild, IRelationalOperator * rChild);
  ~MergeJoin();

  virtual const Schema * schema() const;
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();

 private:
  bool hasData(int);
  bool isEmpty(int branch);

  bool get_tuple(int branch);
  // TODO: another cheat. 
  int compare(const Tuple &, const Tuple &, int id);
  void concatenate(Tuple &, const Tuple &, const Tuple &);
  void create_merge_stack();
  int merge(size_t);
};

#endif
