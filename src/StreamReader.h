#pragma once

#include <cstddef>
#include <string>
#include <istream>

namespace Ymir
{
    //TODO move into it's own file
    class StreamReader
    {
    public:
        
        /**
         * \param s buffer to read into
         * \param max_size size of the buffer, maximum amount to read
         * \return amount of bytes read
         */
        virtual size_t read(char* s, const size_t max_size) = 0;
        
        /**
         * \param amount amount of bytes to skip
         */
        virtual void skip(const size_t amount) = 0;
        
        /**
         * \return true if we are at the end of the stream
         */
        virtual bool eof() const = 0;
        
        /**
         * \return the size of the stream, -1 if it's not possible to determine
         */
        virtual size_t size() = 0;
        
        virtual std::string string();
        
        virtual std::istream& stream() = 0;
    };
};