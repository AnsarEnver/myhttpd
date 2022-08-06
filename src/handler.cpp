#include <boost/asio/random_access_file.hpp>
#include <boost/asio/buffer.hpp>

#include "handler.h"

handler::handler(resource &resource, boost::asio::io_service &io_service)
:_io_service(io_service),
_resource(resource){}

void handler::build_handler(resource::status code){
    this->_respons_handler(this->_res);
}

void handler::async_handle(request *req, response_handler handler){
    this->_respons_handler = handler;
    this->_res = new response();
    this->_resource.async_build_content(
        *req, 
        *this->_res, 
        std::bind(&handler::build_handler, this, std::placeholders::_1)
    );
}

void handler::get(){

}

void handler::post(){

}

void handler::head(){
    
}