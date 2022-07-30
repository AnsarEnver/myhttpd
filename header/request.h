#ifndef REQUEST
#define REQUEST

#include <string>

struct request {
    std::string _header;
    boost::asio::mutable_buffer *_content = nullptr;
    std::string method();
    std::string url();
    std::string version();
    std::string header(std::string name);
    request(const char* header_data, std::size_t size);
    ~request();
};

#endif /* REQUEST */