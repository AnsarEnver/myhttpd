#ifndef RNODE_HPP
#define RNODE_HPP

#include <functional>
#include <memory>
#include <string>


#include "request.hpp"
#include "response.hpp"

namespace myhttpd::http {

    class rnode {

    public:
        typedef std::function<void (std::unique_ptr<response> rsp)> request_handler;

    public:
        virtual bool exists(std::string url) = 0;

        virtual void async_request(std::string url, std::unique_ptr<request> req, request_handler handler) = 0;

    public:
        virtual ~rnode() {}
    };
}

#endif // RNODE_HPP
