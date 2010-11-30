#include "FileManager.h"

PageObject::PageObject(char* data)
{
  pageData = data;
  isNext = false;
  nPage = NULL;
}

void PageObject::addNext(PageObject* nextObject)
{
  isNext = true;
  nPage = nextObject;
}

bool PageObject::existsNext()
{
  return isNext;
}

FileManager::FileManager(int pgSize, int bufferSize, std::string formatFile)
{
  pageSize = pgSize;
  buffer = new char*[bufferSize];
  for (int i = 0; i < bufferSize; i++)
  {
    buffer[i] = new char[pageSize];
  }
  loadBuffer(formatFile);
}

PageObject* FileManager::scan(std::string tableName)
{
  std::list<int> pList = namePagesMap[tableName];
  std::list<int>::iterator it;
  it = pList.begin();
  char* data = buffer[*it];
  it++;
  PageObject* startObject = new PageObject(data);
  PageObject* currentObject = startObject;
  for (it; it != pList.end(); ++it)
  {
    char* nextData = buffer[*it];
    PageObject* nextObject = new PageObject(nextData);
    currentObject->addNext(nextObject);
    currentObject = nextObject;
  }
  return startObject;
}

void FileManager::loadBuffer(std::string formatFile)
{
  std::ifstream pageFormat(formatFile.c_str());
  std::string tableName;
  int bufferLoc = 0;
  while(!std::getline(pageFormat, tableName).eof())
  {
    std::string numPartitions;
    std::string numFields;
    std::string numRecords;
    std::string numBytesPerRecord;
    std::getline(pageFormat, numPartitions, '|');
    std::getline(pageFormat, numFields, '|');
    std::getline(pageFormat, numRecords, '|');
    std::getline(pageFormat, numBytesPerRecord);
    int nPartitions = atoi(numPartitions.c_str());
    int nFields = atoi(numFields.c_str());
    int nRecords = atoi(numRecords.c_str());
    int nBytesPerRecord = atoi(numBytesPerRecord.c_str());
    int recordsPerPage = pageSize/nBytesPerRecord;
    int currentStart = 0;
    int* startPositions = new int[nPartitions];
    int* currentPositions = new int[nPartitions];
    int* FtoPMap = new int[nFields];
    int* FtoBMap = new int[nFields];
    for (int i = 0; i < nPartitions; i++)
    {
      startPositions[i] = currentStart;
      currentPositions[i] = currentStart;
      std::string numFtoP;
      std::string numPBytes;
      std::getline(pageFormat, numFtoP, '|');
      int nFtoP = atoi(numFtoP.c_str());
      std::getline(pageFormat, numPBytes);
      int nPBytes = atoi(numPBytes.c_str());
      currentStart += nPBytes;
      for (int j = 0; j < nFtoP; j++)
      {
	std::string field;
        std::string bytes;
        std::getline(pageFormat,field, '|');
        std::getline(pageFormat,bytes);
        int fieldNum = atoi(field.c_str());
        int fieldBytes = atoi(bytes.c_str());
        FtoPMap[fieldNum] = i;
        FtoBMap[fieldNum] = fieldBytes;
      }
    }
    std::ifstream table(tableName.c_str(), std::ios::in | std::ios::binary);
    char* data = new char[pageSize];
    int nRecs = 0;
    std::list<int> pages;
    for (int k = 0; k < nRecords; k++)
    {
      if(nRecs == recordsPerPage)
      {
        buffer[bufferLoc] = data;
        data = new char[pageSize];
        pages.push_back(bufferLoc);
        bufferLoc++;
        nRecs = 0;
        for(int l = 0; l < nPartitions; l++)
        {
           currentPositions[l] = startPositions[l];
        }
      }
      for (int m = 0; m < nFields; m++)
      {
        int fB = FtoBMap[m];
        int currentPartition = FtoPMap[m];
        int writeLocation = currentPositions[currentPartition];
        char* wrt = & data[writeLocation];
        table.read(wrt,fB);
        currentPositions[currentPartition] = writeLocation + fB;
      }
    }
    pages.push_back(bufferLoc);
    buffer[bufferLoc] = data;
    bufferLoc++;
    namePagesMap[tableName] = pages;
    table.close();
  }
  pageFormat.close();
}
