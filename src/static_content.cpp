#include <iostream>

#include "static_content.h"

boost::asio::const_buffer* static_content::buffer(){
    return new boost::asio::const_buffer(
        this->_region.get_address(), 
        this->_region.get_size()
    );
}

static_content::static_content(std::string path)
:_file(path.c_str(), boost::interprocess::mode_t::read_only),
_region(this->_file, boost::interprocess::mode_t::read_only){}

static_content::~static_content(){
    std::cout << "file released: " << this->_file.get_name() << std::endl;
}