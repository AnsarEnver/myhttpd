#ifndef REQUEST
#define REQUEST

#include <string>

class request {
private:
    std::string _header;
    boost::asio::const_buffer _content;
    bool _has_content = false;

public:
    std::string method();
    std::string url();
    std::string query_string();
    std::string version();
    std::string header(std::string name);
    void set_content(boost::asio::const_buffer &&buf);
    bool has_content();
    boost::asio::const_buffer& get_content();
    request(const char* header_data, std::size_t size);
    request(request &&req);
    ~request();
};

#endif /* REQUEST */