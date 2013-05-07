#pragma once

#include "Net.h"
#include <boost/thread.hpp>

namespace Ymir
{
    class Network
    {
    protected:
        static const int port = 30000;
        net::Socket socket;
        char data[];
    public:
        Network();
        virtual ~Network();

        virtual void read() = 0;
        virtual void write(const unsigned char* data, const size_t size) = 0;
    };
};
