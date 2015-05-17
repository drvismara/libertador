#include <iostream>
#include <fstream>

#include "ConfigFile.h"

std::string trim(std::string const& source, char const* delims = " \t\r\n")
{
  std::string result(source);
  std::string::size_type index = result.find_last_not_of(delims);
  if(index != std::string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}

ConfigFile::ConfigFile(std::string const& configFile) {
  std::ifstream file(configFile.c_str());

  std::string line;
  std::string name;
  std::string value;
  std::string inSection;
  std::string nothing("n");

  int posEqual;
  while (std::getline(file,line)) {

    if (! line.length()) continue;

    if (line[0] == '#') continue;
    if (line[0] == ';') continue;

    if (line[0] == '[') {
      inSection=trim(line.substr(1,line.find(']')-1));
      section_[inSection] = nothing;
      std::cerr << "ConfigFile: section_ -> " << inSection << std::endl;
      continue;
    }

    posEqual=line.find('=');
    name  = trim(line.substr(0,posEqual));
    value = trim(line.substr(posEqual+1));

    content_[inSection+'/'+name]=Chameleon(value);
  }
}

std::string* ConfigFile::getSections()
{
  std::cerr << "ConfigFile: getSections | init " << std::endl;
  std::string* list = new std::string[section_.size()+1];
  int i=0;

  std::cerr << "ConfigFile: getSections | alloc List " << std::endl;
  for (std::map<std::string,std::string>::const_iterator si = section_.begin() ; si != section_.end(); ++si)
  //for(auto const &si : section_)
  {
    if ( ((*si).first).compare("system") != 0)
    {
      list[i] = (*si).first;
      std::cerr << "ConfigFile: getSections | si.first " << (*si).first << std::endl;
      std::cerr << "ConfigFile: getSections | list[i] " << list[i] << std::endl;
      i++;
    }
  }

  list[i] = "END";

  std::cerr << "ConfigFile: getSections | list[i] " << list[i] << std::endl;

  return list;
}

Chameleon const& ConfigFile::Value(std::string const& section, std::string const& entry) const {

  std::map<std::string,Chameleon>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end()) throw "does not exist";

  return ci->second;
}

Chameleon const& ConfigFile::Value(std::string const& section, std::string const& entry, double value) {
  try {
    return Value(section, entry);
  } catch(const char *) {
    return content_.insert(std::make_pair(section+'/'+entry, Chameleon(value))).first->second;
  }
}

Chameleon const& ConfigFile::Value(std::string const& section, std::string const& entry, std::string const& value) {
  try {
    return Value(section, entry);
  } catch(const char *) {
    return content_.insert(std::make_pair(section+'/'+entry, Chameleon(value))).first->second;
  }
}
