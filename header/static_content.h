#ifndef STATIC_CONTENT
#define STATIC_CONTENT

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>

#include "content.h"

class static_content: public content {
private:
    boost::interprocess::file_mapping _file;
    boost::interprocess::mapped_region _region;
public:
    boost::asio::const_buffer* buffer() override;
    static_content(std::string path);
};

#endif /* STATIC_CONTENT */