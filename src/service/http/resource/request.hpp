#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../transceiver/message.hpp"
#include "network/connection.hpp"

#undef DELETE

namespace myhttpd::service::http {

    class request: public message {

    public:
        enum method {

            OPTIONS,
            GET,
            HEAD,
            POST,
            PUT,
            DELETE,
            TRACE,
            CONNECT,
            UNKNOWN,
        };

    private:
        method _method;

        std::string _url;

        std::string _query_string;

        std::string _version;

        network::connection& _conn;

    private:
        void _extract_title() {

            auto title = message::get_title();
            std::size_t offset = 0;
            auto size = title.find(' ', 0);
            auto method_str = title.substr(0, size);
            this->_method = this->method_parse(method_str);
            offset = size + 1;

            if (title.find('?') != std::string::npos) {

                size = title.find('?', offset) - offset;
                this->_url = title.substr(offset, size);
                offset += size + 1;
                size = title.find(' ', offset) - offset;
                this->_query_string = title.substr(offset, size);
                offset += size + 1;

            } else {

                size = title.find(' ', offset) - offset;
                this->_url = title.substr(offset, size);
                offset += size + 1;
            }

            size = title.find('\r', offset) - offset;
            this->_version = title.substr(offset, size);
        }

    public:
        static method method_parse(std::string met_str) {

            if (met_str == "OPTIONS") {

                return method::OPTIONS;

            } else if (met_str == "GET") {

                return method::GET;

            } else if (met_str == "HEAD") {

                return method::HEAD;

            } else if (met_str == "POST") {

                return method::POST;

            } else if (met_str == "PUT") {

                return method::PUT;

            } else if (met_str == "DELETE") {

                return method::DELETE;

            } else if (met_str == "TRACE") {

                return method::TRACE;

            } else if (met_str == "CONNECT") {

                return method::CONNECT;

            } else {

                return method::UNKNOWN;
            }
        }

        static const std::string& method_to_string(method met) {

            static const std::string method_options_str = "OPTIONS";
            static const std::string method_get_str = "GET";
            static const std::string method_head_str = "HEAD";
            static const std::string method_post_str = "POST";
            static const std::string method_put_str = "PUT";
            static const std::string method_delete_str = "DELETE";
            static const std::string method_trace_str = "TRACE";
            static const std::string method_connect_str = "CONNECT";
            static const std::string method_unknown_str = "CONNECT";

            switch (met) {

            case method::OPTIONS:
                return method_options_str;
            case method::GET:
                return method_get_str;
            case method::HEAD:
                return method_head_str;
            case method::POST:
                return method_post_str;
            case method::PUT:
                return method_put_str;
            case method::DELETE:
                return method_delete_str;
            case method::TRACE:
                return method_trace_str;
            case method::CONNECT:
                return method_connect_str;
            default:
                return method_unknown_str;
            }
        }

    public:
        inline std::string get_version() const {

            return this->_version;
        }

        inline void set_version(std::string version) {

            this->_version = version;
        }

        inline method get_method() const {

            return this->_method;
        }

        inline void set_method(method met) {

            this->_method = met;
        }

        inline std::string get_url() const {

            return this->_url;
        }

        inline void set_url(std::string url) {

            this->_url = url;
        }

        inline std::string get_query_string() const {

            return this->_query_string;
        }
            
        inline void set_query_string(std::string value) {

            this->_query_string = value;
        }

        inline const network::connection& get_connection() {

            return this->_conn;
        }

    public:
        virtual std::string get_title() override {

            std::string title;
            std::size_t size = 0;

            if (this->_query_string.empty()) {

                return (this->_method + " " + this->_url + " " + this->_version);

            } else {

                return (this->_method + " " + this->_url + "?" + this->_query_string + " " + this->_version);
            }
        }

        virtual void set_title(const std::string title) override {

            message::set_title(title);
            this->_extract_title();
        }

    public:
        request(message&& msg, network::connection &conn): 
            message(std::move(msg)), _conn(conn) {

            this->_extract_title();
        }

        virtual ~request() = default;
    };
}

#endif // REQUEST_HPP