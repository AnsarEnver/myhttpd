#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>

#include "connection.h"

class acceptor {
public:
    typedef 
        std::function<void(boost::asio::ip::tcp::socket*)> 
            connect_handler;

private:
    boost::asio::io_service &_io_service;
    boost::asio::ip::tcp::acceptor _acceptor;
    connect_handler _connection_handler;
    std::function<void (
        const boost::system::error_code&,
        boost::asio::ip::tcp::socket
    )> _accept_handler;

private:
    void accept_handler(
        const boost::system::error_code& error,
        boost::asio::ip::tcp::socket peer
    );

public:
    acceptor(
        std::string ipv4_addr,
        int port, 
        boost::asio::io_service &io_service, 
        connect_handler handler
    );
    void start_accept();
};

#endif /* ACCEPTOR_Hs */