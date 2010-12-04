#include <stdio.h>
#include <iostream>

#include "DiskPage.h"
#include "Attribute.h"

DiskPage::DiskPage(const PageLayout * layout, MemoryBlock * block,
		   const std::string & table) 
{
  m_block = block;
  m_table = table;
  m_layout = layout;
}

size_t DiskPage::size() const
{
  return m_block->getSize();
}

size_t DiskPage::capacity() const
{
  return m_block->capacity();
}

void DiskPage::get(int rid, const Schema * fields, 
		   byte * buffer, size_t length)
{
  int numFields = fields->nitems();
  int totalNumBytes = fields->rsize();
  int currentLoc = 0;
  for (int j=0; j < numFields; j++)
  {
    int fieldNum = fields->at(j)->id();
    int fieldSize = m_layout->getFieldsBytes(fieldNum);
    int locByte = m_layout->getFieldLoc(fieldNum);
    int partition = m_layout->getFieldsPartition(fieldNum);
    int partitionStart = m_layout->getPartitionStart(partition);
    int partitionBytes = m_layout->getPartitionBytes(partition);
    int offset = partitionStart + partitionBytes*rid + locByte;
    byte * bLoc = & buffer[currentLoc];
    m_block->get(bLoc, offset, fieldSize);
    currentLoc += fieldSize;
  }
}

