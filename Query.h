#ifndef QUERY_H_
#define QUERY_H_

#include <iostream>
#include <ctime>
#include "IRelationalOperator.h"

class Query
{
 private:
  int m_id;
  IRelationalOperator * m_op;

  /* statistics collected */
  clock_t m_stime;
  clock_t m_etime;
  int m_count;
  float m_duration;
  int m_nrecords;
  int m_nfetches;
 public:
  Query(int id, IRelationalOperator * m_op);
  ~Query();
  void * execute();

  void profile(int count = 1);
  void stats(std::ostream &);
};

#endif
