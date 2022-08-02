#include <iostream>
#include <boost/asio/streambuf.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "connection.h"

void connection::receive_handler(request *req){
    this->_socket.write_some(boost::asio::buffer("HTTP/2 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World!"));
    delete req;
    this->_receiver.async_receive(
        std::bind(&connection::receive_handler, this, std::placeholders::_1)
    );
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
_close_handler(handler){}

void connection::do_service(){
    this->_receiver.async_receive(
        std::bind(&connection::receive_handler, this, std::placeholders::_1)
    );
}

connection::connection_id connection::id(){
    return this->_client_id;
}

connection::~connection(){
    this->_socket.close();
}
