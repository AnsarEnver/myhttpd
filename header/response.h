#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>
#include <boost/asio/buffer.hpp>

#include "content.h"

struct response {
    int _status_code;
    std::map<std::string, std::string> header;
    content *content = nullptr;
    ~response();
};

#endif /* RESPONSE_H */