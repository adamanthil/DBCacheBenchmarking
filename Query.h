#ifndef QUERY_H_
#define QUERY_H_

#include <iostream>
#include "IRelationalOperator.h"

class Query
{
 private:
  int m_id;
  IRelationalOperator * m_op;

  /* statistics collected */
  int m_stime;
  int m_etime;
  int m_duration;
  int m_nrecords;
  int m_nfetches;
 public:
  Query(int id, IRelationalOperator * m_op);
  ~Query();
  void * execute();

  void profile();
  void stats(std::ostream &);
};

#endif
