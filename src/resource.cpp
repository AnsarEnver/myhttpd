#include <boost/filesystem.hpp>

#include "mime_types.h"
#include "resource.h"

resource::resource(std::string root, std::map<std::string, std::string> table)
:_mapper(mapper(table)){
    for(int i = 0; i < sizeof(mime_types) / sizeof(_mime_types[0]); i++){
        this->_mime_types.insert({mime_types[i][0], mime_types[i][1]});
    }
    if(root[root.size() - 1] == '\\' || root[root.size() - 1] == '/'){
        this->_root = root.substr(root.size() - 1);
    }else{
        this->_root = root;
    }
}

bool resource::exists(std::string url){
    return boost::filesystem::exists(this->_root + url);
}
