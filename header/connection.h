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
    boost::asio::io_service &_io_service;
    resource &_resource;
    boost::asio::ip::tcp::socket _socket;
    receiver _receiver;
    sender _sender;
    close_handler _close_handler;
    request *_req;

private:
    void receive_handler(request *req);

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