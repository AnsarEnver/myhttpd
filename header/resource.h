#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "mime_types.h"
#include "mapper.h"

class resource {
private:
    mapper _mapper;
    std::string _root;
    std::map<std::string, std::string> _mime_types;
public:
    bool exists(std::string url);
    resource(std::string root, std::map<std::string, std::string> table);
};

#endif /* RESOURCE */
