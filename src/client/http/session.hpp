#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <memory>

#include "client/session.hpp"
#include "timer.hpp"
#include "transceiver_1_1.hpp"
#include "network/connection.hpp"
#include "resource.hpp"


namespace myhttpd::http {

    class session: public myhttpd::session {

    private:
        std::unique_ptr<connection> _conn;

        std::unique_ptr<transceiver> _transceiver;

        terminated_handler _terminated_handler;

        timer _timer;
    private:
        bool _transceiver_receive_busy = false;

        bool _transceiver_send_busy = false;

        bool _transceiver_wait_busy = false;

        bool _timer_busy = false;

        bool _terminating_required = false;

    private:
        void _terminate();

    private:
        void _timeout_handler(timer::status st);

        void _wait_handler(transceiver::error_code code);

        void _receive_handler(transceiver::error_code code, std::unique_ptr<message> request);

        void _send_handler(transceiver::error_code code);

    public:
        virtual void start(terminated_handler handler);
        
    public:
        session(std::unique_ptr<connection> conn, boost::asio::io_context &ctx);

        virtual ~session();
    };
}


#endif // HTTP_SESSION_HPP
