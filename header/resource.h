#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <map>

class resource {
private:
    std::string _root;
    std::map<std::string, std::string> _mapping_table;
    std::map<std::string, std::string> _mime_types;

private:
    std::string mapping(std::string url);
    bool exists(std::string url);

public:
    resource(std::string root, std::map<std::string, std::string> &&mapping_table);
};

#endif /* RESOURCE */
