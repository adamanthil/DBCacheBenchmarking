#include <stdio.h>
#include <iostream>

#include "DiskPage.h"

DiskPage::DiskPage(const PageLayout * layout, MemoryBlock * block,
		   const std::string & table) :
  m_block(block), m_layout(NULL), m_table(table)
{
}

size_t DiskPage::size() const
{
  return m_block->getSize();
}

void DiskPage::get(int rid, const std::string & field, 
		   byte * buffer, size_t length) const
{
  if (m_table == "Student")
    {
      int offset = rid * 61;
      switch (field.c_str()[0])
	{
	case 'i': offset += 0; break;
	case 's': offset += 4; break;
	case 'f': offset += 14; break;
	case 'l': offset += 34; break; 
	case 'g': offset += 54; break;
	case 'b': offset += 55; break;
	case 'y': offset += 59; break;
	}
      
      m_block->get(buffer, offset, length);
    }
}
