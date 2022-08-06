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
    int url_len = 0;
    for(; url_len < url_str.size(); url_len++){
        if(url_str[url_len] == '?') break;
    }
    return url_str.substr(0,url_len);
}

std::string request::query_string(){
    std::istringstream ss(this->_header);
    std::string url_str;
    std::getline(ss, url_str, ' ');
    std::getline(ss, url_str, ' ');
    int url_len = 0;
    for(; url_len < url_str.size(); url_len++){
        if(url_str[url_len] == '?'){
            return url_str.substr(url_len + 1, url_str.size() - url_len - 1);
        }
    }
    return std::string("");
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

void request::set_content(boost::asio::const_buffer &&buf){
    this->_has_content = true;
    this->_content = boost::asio::const_buffer(std::move(buf));
}

bool request::has_content(){
    return this->_has_content;
}

boost::asio::const_buffer& request::get_content(){
    return this->_content;
}

request::request(const char* header_data, std::size_t size):_content(){
    this->_header = std::string(header_data, size);
}

request::request(request &&req)
:_header(std::move(req._header)),
_content(std::move(req._content)),
_has_content(req._has_content){}

request::~request(){
    if(this->_has_content){
        delete this->_content.data();
    }
}