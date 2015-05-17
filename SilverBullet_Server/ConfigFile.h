#ifndef _CONF_CLASS_H_
#define _CONF_CLASS_H_

#include <string>
#include <map>

#include "Chameleon.h"

class ConfigFile {

  std::map<std::string, std::string> section_;
  std::map<std::string,Chameleon> content_;

public:
  ConfigFile(std::string const& configFile);

  std::string* getSections();

  Chameleon const& Value(std::string const& section, std::string const& entry) const;

  Chameleon const& Value(std::string const& section, std::string const& entry, double value);
  Chameleon const& Value(std::string const& section, std::string const& entry, std::string const& value);


};

#endif // _CONF_CLASS_H_
