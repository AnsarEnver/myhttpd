#include <boost/asio/buffer.hpp>
#include <boost/asio/read_until.hpp>
#include <regex>

#include "request.h"

std::string request::method(){
    std::istringstream ss(this->_header);
    std::string method_str;
    std::getline(ss, method_str, ' ');
    return method_str;
}

std::string request::url(){
    std::istringstream ss(this->_header);
    std::string url_str;
    std::getline(ss, url_str, ' ');
    std::getline(ss, url_str, ' ');
    return url_str;
}

std::string request::version(){
    std::istringstream ss(this->_header);
    std::string version_str;
    std::getline(ss, version_str, ' ');
    std::getline(ss, version_str, ' ');
    std::getline(ss, version_str, '\r');
    return version_str;
}

std::string request::header(std::string name){
    std::string exp = "\\r\\n" + name + ": .+\\r\\n";
    std::regex pattern(exp); 
    std::match_results<std::string::const_iterator> results;
    if(std::regex_search(this->_header, results, pattern)){
        std::string result = *results.begin();
        return result.substr(name.length() + 4, result.length() - name.length() - 6);
    }else{
        return "";
    }
}

request::request(const char* header_data, std::size_t size){
    char* this_header_data = new char[size];
    std::memcpy(this_header_data, header_data, size);
    this->_header = std::string(this_header_data, size);
}

request::~request(){
    if(this->_content != nullptr){
        delete this->_content;
    }
}