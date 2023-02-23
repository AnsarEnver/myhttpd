#include <cstddef>

class basic_connection {
public:
    virtual ~basic_connection() {}
    virtual void close() = 0;
    virtual void async_write() = 0;
    virtual void async_read() = 0;
    virtual void async_wait() = 0;
    virtual std::size_t available() = 0;
};