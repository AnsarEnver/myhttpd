#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <map>

class mapper {
private:
    std::map<std::string, std::string> _table;

public:
    mapper(std::map<std::string, std::string> table);
    std::string mapping(std::string url);
};

#endif /* MAPPER_H */