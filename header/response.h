#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>
#include <boost/asio/buffer.hpp>

struct response {
    int _status_code;
    std::map<std::string, std::string> header;
    boost::asio::mutable_buffer content;
};

#endif /* RESPONSE_H */