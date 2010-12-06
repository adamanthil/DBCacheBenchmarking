#include <new>
#include <assert.h>
#include "FileManager.h"

FileManager* FileManager::instance = 0;

void FileManager::Initialize(const std::string & config_file, const std::string & schema_file)
{
  if (!FileManager::instance)
  {
    instance = new FileManager(config_file,schema_file);
  }
}

FileManager * FileManager::getInstance()
{
  if (!FileManager::instance)
  {
    FileManager::instance = new FileManager("config","schema");
  }
  return FileManager::instance;
}

FileDescriptor * FileManager::open(const std::string & filename)
{
  std::list<int>* pList = m_namePagesMap[filename];
  std::list<int>::iterator it;
  it = pList->begin();
  std::vector<DiskPage *> * tPages = new std::vector<DiskPage *>();
  for (it; it != pList->end(); it++)
  {
    DiskPage* pg = m_files.at(*it);
    tPages->push_back(pg);
  }
  FileDescriptor * descript = new FileDescriptor(tPages);
  return descript;
}

void FileManager::close(FileDescriptor * fd)
{
  int a = 0;
  delete fd;
}

FileManager::FileManager(const std::string & formatFile, const std::string & schemaFile)
{
  loadData(formatFile);
}

void FileManager::loadData(const std::string & formatFile)
{
  std::ifstream pageFormat(formatFile.c_str());
  std::string tableName;
  int bufferLoc = 0;
  std::string pgSize;
  std::getline(pageFormat, pgSize);
  m_pageSize = atoi(pgSize.c_str());
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
    int recordsPerPage = m_pageSize/nBytesPerRecord;
    int currentStart = 0;
    int* startPositions = new int[nPartitions];
    int* currentPositions = new int[nPartitions];
    int* FtoPMap = new int[nFields];
    int* FtoBMap = new int[nFields];
    int* FtoLMap = new int[nFields];
    int* PtoBMap = new int[nPartitions];
    int* PtoLMap = new int[nPartitions];
    for (int i = 0; i < nPartitions; i++)
    {
      int fLoc = 0;
      startPositions[i] = currentStart;
      currentPositions[i] = currentStart;
      std::string numFtoP;
      std::string numPBytes;
      std::getline(pageFormat, numFtoP, '|');
      int nFtoP = atoi(numFtoP.c_str());
      std::getline(pageFormat, numPBytes);
      int nPBytes = atoi(numPBytes.c_str());
      PtoLMap[i] = currentStart;
      currentStart += nPBytes*recordsPerPage;
      PtoBMap[i] = nPBytes;
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
        FtoLMap[fieldNum] = fLoc;
        fLoc += fieldBytes;
      }
    }

    
    PageLayout * a = new PageLayout(nPartitions, nFields, nBytesPerRecord, FtoPMap, FtoBMap, FtoLMap, PtoLMap, PtoBMap);
    
    std::ifstream table(tableName.c_str(), std::ios::in | std::ios::binary);
    int nRecs = 0;
    std::list<int> * pages = new std::list<int>();
    MemoryBlock * data = new MemoryBlock(m_pageSize);

    
    for (int k = 0; k < nRecords; k++)
    {
      if(nRecs == recordsPerPage)
      {
        pages->push_back(bufferLoc);
        bufferLoc++;
        for(int l = 0; l < nPartitions; l++)
	{
           currentPositions[l] = startPositions[l];
        }
	DiskPage * dp = new DiskPage(a,data,tableName);
	data->setSize(nRecs);
	nRecs = 0;
	data = new MemoryBlock(m_pageSize);
        m_files.push_back(dp);
      }
      
      for (int z = 0; z < nFields; z++)
      {
	
        int fB = FtoBMap[z];
        int currentPartition = FtoPMap[z];
        int writeLocation = currentPositions[currentPartition];
        
        char * wrt = new char[fB];
	
        table.read(wrt,fB);
        data->put((byte*)wrt, writeLocation, fB);
        delete [] wrt;
	currentPositions[currentPartition] = writeLocation + fB;
	
      }
      nRecs++;
    }
    pages->push_back(bufferLoc);
    data->setSize(nRecs);
    DiskPage *dp = new DiskPage(a, data, tableName);
    bufferLoc++;
    m_files.push_back(dp);
    m_namePagesMap[tableName] = pages;
    table.close();
    
  }
  pageFormat.close();
}


