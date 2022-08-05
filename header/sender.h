#ifndef SENDER_H
#define SENDER_H

#include <functional>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "response.h"

class sender {
public:
    typedef std::function<void ()> send_handler;

private:
    boost::asio::ip::tcp::socket &_socket;
    send_handler _send_handler;
    boost::asio::const_buffer *_header_buffer;
    boost::asio::const_buffer *_content_buffer = nullptr;
    response *_res;
private:
    void header_write_handler(
        const boost::system::error_code& error,
        std::size_t bytes_transferred
    );
    void content_write_handler(
        const boost::system::error_code& error,
        std::size_t bytes_transferred
    );

public:
    void async_send(response *res, send_handler handler);
    sender(boost::asio::ip::tcp::socket &socket);
};

#endif /* SENDER_H */