#include "Settings.h"


bool Settings::m_layout = false;

void Settings::set(const std::string & name, bool value)
{
  m_layout = value;
}

void Settings::get(const std::string & name, bool & value)
{
  value = m_layout;
}
