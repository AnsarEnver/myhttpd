#include <iostream>
#include <functional>
#include <boost/bind.hpp>

#include "acceptor.h"

void acceptor::accept_handler(
    const boost::system::error_code& error,
    boost::asio::ip::tcp::socket peer){
    this->_acceptor.async_accept(this->_accept_handler);
    if(!error){
        this->_connection_handler(
            new boost::asio::ip::tcp::socket(std::move(peer)));
    }
}

acceptor::acceptor(
    std::string ipv4_addr,
    int port,
    boost::asio::io_service &io_service,
    connect_handler handler)
:_connection_handler(handler),
_io_service(io_service),
_acceptor(
    boost::asio::ip::tcp::acceptor(
        io_service,
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address_v4::from_string(ipv4_addr),
            port
        )
    )
),
_accept_handler(
    std::bind(
        &acceptor::accept_handler, 
        this, 
        std::placeholders::_1, 
        std::placeholders::_2
    )
){}

void acceptor::start_accept(){
    this->_acceptor.async_accept(this->_accept_handler);
}