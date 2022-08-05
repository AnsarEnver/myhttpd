#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/uuid/uuid.hpp>
#include <functional>

#include "receiver.h"
#include "sender.h"
#include "resource.h"
#include "handler.h"

class connection {
public:
    typedef boost::uuids::uuid connection_id;
    typedef std::function<void (connection*)> close_handler;

private:
    connection_id _client_id;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::io_service &_io_service;
    resource &_resource;
    request *_req = nullptr;
    response *_res  = nullptr;
    receiver _receiver;
    sender _sender;
    handler _handler;
    close_handler _close_handler;
    receiver::receive_handler _receive_handler;
    handler::response_handler _response_handler;
    sender::send_handler _send_handler;

private:
    void receive_handler(request *req);
    void response_handler(response *res);
    void send_handler();

public:
    connection(
        boost::asio::ip::tcp::socket &&socket,
        boost::asio::io_service &io_service,
        resource &resource_,
        close_handler handler);
    void do_service();
    connection_id id();
    ~connection();
    void close();
};

    
#endif /* CONNECTION_H */