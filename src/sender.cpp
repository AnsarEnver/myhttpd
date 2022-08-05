#include <sstream>

#include "sender.h"

std::string status_to_string(int scode){
    switch (scode)
    {
    case 100:
        return "100 Continue";
        break;
    case 101:
        return "101 Switching Protocols";
        break;
    case 200:
        return "200 OK";
        break;
    case 201:
        return "201 Created";
        break;
    case 202:
        return "202 Accepted";
        break;
    case 203:
        return "203 Non-Authoritative Information";
        break;
    case 204:
        return "204 No Content";
        break;
    case 205:
        return "205 Reset Content";
        break;
    case 206:
        return "206 Partial Content";
        break;
    case 300:
        return "300 Multiple Choices";
        break;
    case 301:
        return "301 Moved Permanently";
        break;
    case 302:
        return "302 Found";
        break;
    case 303:
        return "303 See Other";
        break;
    case 304:
        return "304 Not Modified";
        break;
    case 305:
        return "305 Use Proxy";
        break;
    case 306:
        return "306 Unused";
        break;
    case 307:
        return "307 Temporary Redirect";
        break;
    case 400:
        return "400 Bad Request";
        break;
    case 401:
        return "401 Unauthorized";
        break;
    case 402:
        return "402 Payment Required";
        break;
    case 403:
        return "403 Forbidden";
        break;
    case 404:
        return "404 Not Found";
        break;
    case 405:
        return "405 Method Not Allowed";
        break;
    case 406:
        return "406 Not Acceptable";
        break;
    case 407:
        return "407 Proxy Authentication Required";
        break;
    case 408:
        return "408 Request Time-out";
        break;
    case 409:
        return "409 Conflict";
        break;
    case 410:
        return "410 Gone";
        break;
    case 411:
        return "411 Length Required";
        break;
    case 412:
        return "412 Precondition Failed";
        break;
    case 413:
        return "413 Request Entity Too Large";
        break;
    case 414:
        return "414 Request-URI Too Large";
        break;
    case 415:
        return "415 Unsupported Media Type";
        break;
    case 416:
        return "416 Requested range not satisfiable";
        break;
    case 417:
        return "417 Expectation Failed";
        break;
    case 500:
        return "500 Internal Server Error";
        break;
    case 501:
        return "501 Not Implemented";
        break;
    case 502:
        return "502 Bad Gateway";
        break;
    case 503:
        return "503 Service Unavailable";
        break;
    case 504:
        return "504 Gateway Time-out";
        break;
    case 505:
        return "505 HTTP Version not supported";
        break;
    default:
        return std::to_string(scode) + " Internal Server Error";
        break;
    }
}

void sender::header_write_handler(
    const boost::system::error_code& error,
    std::size_t bytes_transferred
){
    if(!error){
        if(this->_content_buffer == nullptr){
            this->_send_handler();
        }else{
            this->_socket.async_write_some(
                this->_content_buffer, 
                std::bind(
                    &sender::content_write_handler,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
        }
    }
}

void sender::content_write_handler(
    const boost::system::error_code& error,
    std::size_t bytes_transferred
){
    this->_send_handler();
}

void sender::async_send(response *res, send_handler handler){
    this->_send_handler = handler;
    this->_res = res;
    this->_content_buffer = nullptr;
    if(res->content != nullptr){
        this->_content_buffer = res->content->buffer();
        res->header["Content-Length"] = std::to_string(this->_content_buffer->size());
    }
    std::ostringstream header_stream;
    header_stream << "HTTP/1.0 " << status_to_string(res->_status_code) << "\r\n";
    for(auto fild: res->header){
        header_stream << fild.first << ": " << fild.second << "\r\n";
    }
    header_stream << "\r\n";
    auto header_str = header_stream.str();
    auto size = header_str.size();
    char *buf = new char[size];
    std::memcpy(buf, header_str.data(), size);
    this->_header_buffer = new boost::asio::const_buffer(buf, size);
    this->_socket.async_write_some(
        this->_header_buffer, 
        std::bind(
            &sender::header_write_handler,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );
}

sender::sender(boost::asio::ip::tcp::socket &socket): _socket(socket){}