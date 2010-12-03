#include <new>
#include "FileManager.h"

FileManager * FileManager::instance = NULL;

struct Student
{
  int id;
  char ssn[10 + 1];
  char fname[20 + 1];
  char lname[20 + 1];
  char gender;
  int birthdate;
  char year[2 + 1];
};

struct Course
{
  int id;
  char instructor[64];
  char title[32];
};

struct Roster
{
  int sid;
  int cid;
};

FileManager::FileManager()
{

  Student s[] = 
    {
      {0, "2389389392", 
       "Greig               ", 
       "Hazell              ", 'M', 0101, "SR"},
      
      {1, "3949493484",
       "Andrew              ",
       "Bender              ", 'M', 2001, "JR"},
      {2, "3879093894",
       "Emma                ",
       "Turetsky            ", 'F', 2010, "SR"}
    };
  int offset = 0;

  MemoryBlock * b = new MemoryBlock(sizeof(Student) * 2);

  for (int i = 0; i < 2; i++)
    {
      offset = b->put((byte*)&s[i].id, offset, sizeof(s[i].id));
      offset = b->put((byte*)&s[i].ssn, offset, sizeof(s[i].ssn) - 1);
      offset = b->put((byte*)&s[i].fname, offset, sizeof(s[i].fname) - 1);
      offset = b->put((byte*)&s[i].lname, offset, sizeof(s[i].lname) - 1);
      offset = b->put((byte*)&s[i].gender, offset, sizeof(s[i].gender));
      offset = b->put((byte*)&s[i].birthdate, offset, sizeof(s[i].birthdate));
      offset = b->put((byte*)&s[i].year, offset, sizeof(s[i].year) - 1);
    }
  //m_files[0] = new std::vector<DiskPage *>();
  b->setSize(2);
  m_files[0].push_back(new DiskPage(NULL, b, "Student"));

  b = new MemoryBlock(sizeof(Student) * 2);

  offset=0;
  for (int i = 2; i < sizeof(s) / sizeof(Student); i++)
    {
      offset = b->put((byte*)&s[i].id, offset, sizeof(s[i].id));
      offset = b->put((byte*)&s[i].ssn, offset, sizeof(s[i].ssn) - 1);
      offset = b->put((byte*)&s[i].fname, offset, sizeof(s[i].fname) - 1);
      offset = b->put((byte*)&s[i].lname, offset, sizeof(s[i].lname) - 1);
      offset = b->put((byte*)&s[i].gender, offset, sizeof(s[i].gender));
      offset = b->put((byte*)&s[i].birthdate, offset, sizeof(s[i].birthdate));
      offset = b->put((byte*)&s[i].year, offset, sizeof(s[i].year) - 1);
    }
  //m_files[0] = new std::vector<DiskPage *>();
  b->setSize(1);
  m_files[0].push_back(new DiskPage(NULL, b, "Student"));

  b = new MemoryBlock(512);
  
}

void FileManager::Initialize(const std::string & database)
{
  if (FileManager::instance == NULL)
    {
      FileManager::instance = new FileManager();
    }
}

FileManager * FileManager::getInstance()
{
  if (FileManager::instance == NULL)
    {
      FileManager::Initialize("");
    }

  return FileManager::instance;
}

FileDescriptor * FileManager::open(const std::string & filename)
{
  if (filename == "Student")
    {
      return new FileDescriptor(m_files[0]);
    }
  else if (filename == "Course")
    {
      return new FileDescriptor(m_files[1]);
    }
  else
    {
      return new FileDescriptor(m_files[2]);
    }
}

void FileManager::close(FileDescriptor * fd)
{
  delete fd;
}
