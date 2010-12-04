#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(std::vector<DiskPage *> & pages) :
  m_pages(pages), m_pid(0)
{
}

DiskPage * FileDescriptor::read()
{
  return m_pages[m_pid++];
}

bool FileDescriptor::eof() 
{
  return m_pages.size() <= m_pid;
}

int FileDescriptor::size()
{
  return m_pages.size();
}

void FileDescriptor::seek(int pid)
{
  m_pid = pid;
}
