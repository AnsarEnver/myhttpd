#ifndef HANDLER_H
#define HANDLER_H

#include <functional>
#include <boost/asio/io_service.hpp>

#include "resource.h"
#include "request.h"
#include "response.h"
 
class handler {
public:
    typedef std::function<void (response*)> response_handler;

private:
    boost::asio::io_service &_io_service;
    resource &_resource;
    response *_res;
    response_handler _respons_handler;

private:
    void build_handler(resource::status code);
    void get();
    void post();
    void head();

public: 
    handler(resource &resource, boost::asio::io_service &io_service);
    void async_handle(request *req, response_handler handler);
};

#endif /* HANDLER_H */