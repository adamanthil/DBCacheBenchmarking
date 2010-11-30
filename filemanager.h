#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <list>

class PageObject
{
  private:
    char* pageData;
    bool isNext;
    PageObject* nPage;

  public:
    PageObject(char* data);
    
    void addNext(PageObject* enxtObject);
    bool existsNext();
};

class FileManager
{
  private:
    int pageSize;
    char **buffer;
    std::map<std::string,std::list<int> > namePagesMap;

  public:
    FileManager(int pgSize, int bufferSize, std::string formatFile);
    PageObject* scan(std::string fileName);
    void loadBuffer(std::string formatFile);
};

#endif
