#ifndef MYHTTPD_H
#define MYHTTPD_H

#include <boost/asio/io_service.hpp>
#include <boost/signals2/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>
#include <map>

#include "acceptor.h"
#include "connection.h"

class myhttpd {
private:
    boost::asio::io_service _io_service;
    boost::asio::io_service::work _work;
    std::map<connection::connection_id, connection*> connections;
    boost::mutex connections_lock;
    acceptor _acceptor;
    std::vector<boost::thread> worker_threads;
    int _thread_number;
    connection::close_handler _close_handler;
    
private:
    void work();
    void connect_handler(boost::asio::ip::tcp::socket *socket);
    void close_handler(connection *conn);

public:
    myhttpd(int thread_number, std::string ipv4_addr, int port);
    void start();
    void join();
};

#endif /* MYHTTPD_H */