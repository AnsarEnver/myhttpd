#ifndef CONTENT_H
#define CONTENT_H

#include <boost/asio/buffer.hpp>

class content {
public:
    virtual boost::asio::const_buffer* buffer() = 0;
    virtual ~content();
};

#endif /* CONTENT_H */