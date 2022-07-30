#include <iostream>
#include <algorithm>

#include "myhttpd.h"

void myhttpd::connect_handler(boost::asio::ip::tcp::socket *socket){
    this->connections_lock.lock();
    auto conn = new connection(
        std::move(*socket), 
        this->_io_service, 
        this->_close_handler
    );
    this->connections.insert({conn->id(), conn});
    this->connections_lock.unlock();
    delete socket;
    conn->do_service();
}

void myhttpd::close_handler(connection *conn){
    
}

myhttpd::myhttpd(int thread_number, std::string ipv4_addr, int port)
:_acceptor(
    ipv4_addr,
    port,
    this->_io_service,
    std::bind(&myhttpd::connect_handler, this, std::placeholders::_1)
),
_work(boost::asio::io_service::work(this->_io_service)),
_thread_number(thread_number),
_close_handler(std::bind(&myhttpd::close_handler, this, std::placeholders::_1)){}

void myhttpd::work(){
    std::cout << "worker started.\n";
    this->_io_service.run();
}

void myhttpd::start(){
    this->_acceptor.start_accept();
    for(int i = 0; i < this->_thread_number; i++){
        this->worker_threads.push_back(boost::thread(
            std::bind(&myhttpd::work, this)
        ));
    }
}

void myhttpd::join(){
    std::for_each(
        this->worker_threads.begin(), 
        this->worker_threads.end(),
        [](boost::thread &worker){
            worker.join();
        }
    );
}

int main(int argc, char *argv[]) {
    myhttpd _myhttpd(64, "127.0.0.1", 80);
    _myhttpd.start();
    _myhttpd.join();
}