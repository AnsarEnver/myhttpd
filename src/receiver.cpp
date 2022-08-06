#include <boost/asio/read_until.hpp>
#include <boost/asio/read.hpp>
#include <iostream>
#include <string>

#include "receiver.h"

void receiver::read_header_handler(
    const boost::system::error_code& error, 
    std::size_t bytes_transferred
){
    if(!error){
        const char *raw_buf = 
            boost::asio::buffer_cast<const char*>(this->_buffer.data());
        int header_size = 0;
        for(int &i = header_size; i < this->_buffer.size() - 3; i++){
            if(raw_buf[i] == '\r' && raw_buf[i + 1] == '\n' 
                && raw_buf[i + 2] == '\r' && raw_buf[i + 3] == '\n'){
                i += 4;
                break;
            }
        }
        this->req = new request(
            boost::asio::buffer_cast<const char*>(this->_buffer.data()), 
            header_size
        );
        this->_buffer.consume(header_size);
        if(!this->req->method().compare("POST")){
            int content_size = std::stoi(this->req->header("Content-Length"));
            boost::asio::async_read(
                this->_socket,
                this->_buffer,
                boost::asio::transfer_at_least(
                    content_size - this->_buffer.size()
                ),
                this->_read_content_handler
            );
        }else{
            this->_receive_handler(this->req);
        }
    }
}

void receiver::read_content_handler(
    const boost::system::error_code& error, 
    std::size_t bytes_transferred){
    if(!error){
        int size = this->_buffer.size();
        char *content = new char[size];
        std::memcpy(
            content, 
            boost::asio::buffer_cast<const char*>(this->_buffer.data()), 
            size
        );
        boost::asio::const_buffer const_buf(content, size);
        req->set_content(std::move(const_buf));
        this->_buffer.consume(size);
        this->_receive_handler(this->req);
    }
}

void receiver::async_receive(receive_handler handler){
    this->_receive_handler = handler;
    boost::asio::async_read_until(
        this->_socket,
        this->_buffer,
        "\r\n\r\n",
        this->_read_header_handler
    );
}

receiver::receiver(boost::asio::ip::tcp::socket &socket)
:_socket(socket),
_read_header_handler(
    std::bind(
        &receiver::read_header_handler, 
        this, 
        std::placeholders::_1, 
        std::placeholders::_2
    )
),
_read_content_handler(
    std::bind(
        &receiver::read_content_handler, 
        this, 
        std::placeholders::_1, 
        std::placeholders::_2
    )
){}



