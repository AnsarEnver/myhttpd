#include "mime_types.h"
#include "resource.h"

resource::resource(std::string root, std::map<std::string, std::string> table)
:_mapper(mapper(table)){
    for(int i = 0; i < sizeof(mime_types) / sizeof(_mime_types[0]); i++){
        this->_mime_types.insert({mime_types[i][0], mime_types[i][1]});
    }
}
