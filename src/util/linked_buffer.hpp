#include <list>
#include <array>
#include <cstddef>


namespace myhttpd {

    template <std::size_t BlockSize>
    class linked_buffer {

#ifdef __MYHTTPD_UNIT_TEST__
    public:
#else
    private:
#endif
        typedef std::array<char, BlockSize> BlockType;
    
    public:
        struct block {
            char* base;
            std::size_t size;
        };

#ifdef __MYHTTPD_UNIT_TEST__
    public:
#else
    private:
#endif
        std::size_t _size = 0;

        std::size_t _capacity = 0;

        std::size_t _offset = 0;

        std::list<BlockType> _blocks;

    public:
        linked_buffer() {};

        linked_buffer(linked_buffer&& buf) 
        : _blocks(std::move(buf._blocks)), _size(buf._size), _capacity(buf._capacity) {}

        void consume(std::size_t size) {
            this->_size -= size;
            this->_offset += size;
            while(this->_offset >= BlockSize) {
                this->_offset -= BlockSize;
                this->_blocks.pop_front();
            }
        }

        void commit(std::size_t size) {
            this->_size += size;
            this->_capacity -= size;
        }

        std::size_t size() {
            return this->_size;
        }

        block get_available_block() {
            if (this->_capacity == 0) {
                this->_blocks.push_back(BlockType());
                this->_capacity += BlockSize;
                return block{ this->_blocks.back().data(), BlockSize };
            } else {
                return block{ this->_blocks.back().data() + ((this->_offset + this->_size) % BlockSize), this->_capacity};
            }
        }

        std::list<block> get_data(std::size_t size) {
            std::list<block> data_blocks;
            if (size <= this->_size) {
                auto first_block_size = (size > (BlockSize - this->_offset)) ? BlockSize - this->_offset : size;
                auto full_blocks = (size - this->_offset) / BlockSize;
                auto last_block_size = size - (full_blocks * BlockSize) - first_block_size;
                auto it = this->_blocks.begin();
                data_blocks.push_back(block{ it->data() + this->_offset, first_block_size });
                it++;
                for (std::size_t i = 0; i < full_blocks; i++) {
                    data_blocks.push_back(block{ it->data(), BlockSize });
                    it++;
                }
                if (last_block_size != 0) {
                    data_blocks.push_back(block{ it->data(), last_block_size });
                }
            }
            return data_blocks;
        }
    };
}