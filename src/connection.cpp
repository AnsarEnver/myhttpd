#include <iostream>
#include <boost/asio/streambuf.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "connection.h"

void connection::receive_handler(request *req){
    this->_req = req;
    this->_handler.async_handle(req, this->_response_handler);
}

void connection::response_handler(response *res){
    this->_res = res;
    this->_sender.async_send(res, this->_send_handler);
}

void connection::send_handler(){
    // bool keep_alive = (this->_req->header("Connection").compare("Keep-alive") == 0);
    // delete this->_req;
    // delete this->_res;
    // if(keep_alive){
    //     this->_receiver.async_receive(this->_receive_handler);
    // }else{
    //     delete this;
    // }
    delete this;
}

connection::connection(
    boost::asio::ip::tcp::socket &&socket,
    boost::asio::io_service &io_service,
    resource &resource_,
    close_handler handler
)
:_socket(std::move(socket)),
_client_id(boost::uuids::random_generator()()),
_io_service(_io_service),
_resource(resource_),
_receiver(this->_socket),
_sender(this->_socket),
_close_handler(handler),
_handler(resource_, io_service),
_receive_handler(
    std::bind(&connection::receive_handler, this, std::placeholders::_1)
),
_response_handler(
    std::bind(&connection::response_handler, this, std::placeholders::_1)
),
_send_handler(
    std::bind(&connection::send_handler, this)
){}

void connection::do_service(){
    this->_receiver.async_receive(this->_receive_handler);
}

connection::connection_id connection::id(){
    return this->_client_id;
}

connection::~connection(){
    this->_socket.close();
}
