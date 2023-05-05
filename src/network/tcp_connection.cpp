#include <boost/asio/buffer.hpp>
#include <boost/asio.hpp>

#include "tcp_connection.hpp"

namespace myhttpd::network {

    void tcp_connection::async_read_some(mutable_buffer buf, reading_handler handler) {

        this->_stream->async_read_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_write_some(const_buffer buf, writing_handler handler) {

        this->_stream->async_write_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_receive(mutable_buffer buf, receiving_handler handler) {

        boost::asio::async_read(*(this->_stream), boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_send(const_buffer buf, sending_handler handler) {

        boost::asio::async_write(*(this->_stream), boost::asio::buffer(buf.data, buf.size), handler);
    }

    void tcp_connection::async_wait(socket_wait_type type, waiting_handler handler) {

        this->_stream->async_wait(type, handler);
    }

    std::string tcp_connection::get_type() {

        return "tcp";
    }

    std::string tcp_connection::get_remote_address() {

        return this->_stream->remote_endpoint().address().to_string();
    }

    int tcp_connection::get_remote_port() {

        return this->_stream->remote_endpoint().port();
    }

    std::string tcp_connection::get_local_address() {

        return this->_stream->local_endpoint().address().to_string();
    }

    int tcp_connection::get_local_port() {

        return this->_stream->local_endpoint().port();
    }

    void tcp_connection::cancel() {

        this->_stream->cancel();
    }

    bool tcp_connection::is_open() {

        return this->_stream->is_open();
    }

    void tcp_connection::reset_io_context(boost::asio::io_context& ctx) {

        auto protocol = this->_stream->local_endpoint().protocol();
        auto soc_handle = this->_stream->release();
        this->_stream.reset(new boost::asio::ip::tcp::socket(ctx));
        this->_stream->assign(protocol, soc_handle);
    }

    tcp_connection::tcp_connection(boost::asio::ip::tcp::socket stream): 
        _stream(new boost::asio::ip::tcp::socket(std::move(stream))) {

    }
}