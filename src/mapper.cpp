#include "mapper.h"

mapper::mapper(std::map<std::string, std::string> table)
:_table(table){}
    
std::string mapper::mapping(std::string url){
    if(this->_table.count(url) == 0){
        return url;
    }else{
        return this->_table[url];
    }
}

