#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include "FileDescriptor.h"
#include "Table.h"
#include "Attribute.h"

typedef unsigned char byte;
class IFileManager
{
 public:
  virtual FileDescriptor * open(const std::string & filename) = 0;
  virtual void close(FileDescriptor * fd) = 0;

};

class FileManager : public IFileManager

{
  private:
    int m_pageSize;
    std::vector<DiskPage *> m_files;
    std::map<std::string,std::list<int>* > m_namePagesMap;
    std::map<std::string,PageLayout> m_pageLayoutMap;
    std::map<std::string,Table *> m_schemaMap;


  public:
    static void Initialize(const std::string & config_file = "", const std::string & schema_file = "");
    static FileManager * getInstance();
    static FileManager * instance;
    
    void loadData(const std::string & config_file);

    FileManager(const std::string & formatFile, const std::string & schemaFile);
    ~FileManager();

    FileDescriptor * open(const std::string & filename);
    void close(FileDescriptor * fd);



};

#endif
