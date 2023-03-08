#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <functional>
#include <map>
#include <tinyxml2.h>

#include "rnode.hpp"
#include "request.hpp"
#include "response.hpp"

namespace myhttpd::http {
    class resource {
    
    public:
        typedef std::function<void (std::unique_ptr<response> rsp)> request_handler;

    private:
        std::map<std::string ,std::unique_ptr<rnode>> _rnodes;
            
    public:
        void async_request(std::unique_ptr<request> req, request_handler handler);

        void config(tinyxml2::XMLElement *config);
    };
}

#endif // RESOURCE_HPP
