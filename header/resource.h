#ifndef RESOURCE_H
#define RESOURCE_H

#include <boost/asio/buffer.hpp>
#include <boost/asio/random_access_file.hpp>
#include <boost/asio/io_service.hpp>
#include <functional>
#include <string>
#include <map>

#include "request.h"
#include "response.h"

class resource {
public:
    enum status {
        SUCCESS = 0,
        NOT_EXISTS
    };
    typedef std::function<void (status)> build_handler;

private:
    boost::asio::io_service &_io_service;
    std::string _root;
    std::map<std::string, std::string> _mapping_table;
    std::map<std::string, std::string> _mime_types;

private:
    std::string mime_type(std::string url);
    std::string url_to_path(std::string url);

public:
    bool exists(std::string url);
    std::string mapping(std::string url);
    void async_build_content(request &req, response &res, build_handler handler);
    resource(
        boost::asio::io_service &io_service, 
        std::string root, 
        std::map<std::string, std::string> &&mapping_table
    );
};

#endif /* RESOURCE */
