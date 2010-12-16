#ifndef SYS_CATALOG_H_
#define SYS_CATALOG_H_

#include "Database.h"

class SystemCatalog
{
 public:
  static const Table * table(const std::string & name)
  {
    return Database::getInstance()->table(name);
  }
};

#endif
