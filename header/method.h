#ifndef METHOD_H
#define METHOD_H

#include <boost/asio/io_service.hpp>

#include "request.h"
#include "response.h"
#include "resource.h"

class method {
private:
    resource &_resource;

private:
    void get(request *req);
    void post(request *req);
    void head(request *req);
    
public:
    void call(request *req);
    method();
};

#endif /* METHOD */