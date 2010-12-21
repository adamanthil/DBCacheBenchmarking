#ifndef TUPLE_STREAM_READER_H_
#define TUPLE_STREAM_READER_H_

#include "Attribute.h"
#include "MemoryBlock.h"
#include "MaterializationLayout.h"
#include "Tuple.h"
#include "globals.h"
#include <set>

class TupleStreamReader
{
 private:
  int m_nRecs;
  const MaterializationLayout * m_layout;
  MemoryBlock & m_block;
 public:
  TupleStreamReader(MemoryBlock & block);
  ~TupleStreamReader();
  void layout(const MaterializationLayout * layout);

  void peek(Tuple & t);
  void read(Tuple & t);

  void rewind(int back);
  bool isEndOfStream();
  void reset(); 
};

#endif
