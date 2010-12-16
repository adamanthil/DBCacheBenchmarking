#ifndef TUPLE_STREAM_WRITER_H_
#define TUPLE_STREAM_WRITER_H_

#include "MemoryBlock.h"
#include "MaterializationLayout.h"
#include "Attribute.h"
#include "Tuple.h"
#include "globals.h"

class TupleStreamWriter
{
 private:
  int m_pos;
  int m_nRecs;
  size_t m_record_size;
  const MaterializationLayout * m_layout;
  MemoryBlock & m_block;
 public:
  TupleStreamWriter(MemoryBlock & block, size_t record_size);
  ~TupleStreamWriter();
  void layout(const MaterializationLayout *);

  void write(Tuple & t);
  bool isStreamFull();

  void discard();
};

#endif
