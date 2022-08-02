#ifndef RECEIVER_H
#define RECEIVER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <functional>

#include "request.h"

class receiver {
public:
    typedef std::function<void (request*)> receive_handler;
    
private: 
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket &_socket;
    receive_handler _receive_handler;
    boost::asio::streambuf _buffer;
    request *req;
    std::function<void (const boost::system::error_code&, std::size_t)> 
        _read_header_handler;
    std::function<void (const boost::system::error_code&, std::size_t)> 
        _read_content_handler;
        
private:
    void read_header_handler(
        const boost::system::error_code& error, 
        std::size_t bytes_transferred
    );
    void read_content_handler(
        const boost::system::error_code& error, 
        std::size_t bytes_transferred
    );

public:
    receiver(boost::asio::ip::tcp::socket &socket);
    void async_receive(receive_handler handler);
    
};

#endif /* RECEIVER_H */