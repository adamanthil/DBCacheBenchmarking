#include "DataCreator.h"

void DataCreator::Create(const std::string & configFile)
{
  std::ofstream oFile ("Student.tab", std::ios::out | std::ios::binary);
  const char * data = "02389389392Greig               Hazell              M0101SR0238938938213949493484Andrew              Bender              M2001JR23879093894Emma                Turetsky            F2010SR";
  oFile.write(data, 183);
  oFile.close();
}
