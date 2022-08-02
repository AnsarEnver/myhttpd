#ifndef HANDLER_H
#define HANDLER_H

#include <functional>
#include <boost/filesystem.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/stream_file.hpp>

#include "resource.h"
#include "request.h"
#include "response.h"
 
class handler {
public:
    typedef std::function<void (response*)> response_handler;
    
private:
    boost::asio::io_service &_io_service;
    resource &_resource;

public: 
    void async_handle(request *req, response_handler handler);
    void get();
    void post();
    void head();
};

#endif /* HANDLER_H */