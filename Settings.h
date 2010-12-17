#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <map>

class Settings
{
 private:
  static std::map<std::string, bool> m_values;
  static bool m_layout;
 public:
  static void set(const std::string & name, bool value);
  static void get(const std::string & name, bool & value);
 private:
  Settings();
};

#endif
