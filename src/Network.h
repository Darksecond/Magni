#pragma once

#include "Net.h"

namespace Ymir
{
    class Network
    {
    public:
        static const int port = 30000;
        net::Socket socket;
        char data[];
        Network();
        virtual ~Network();

        virtual void read() = 0;
        virtual void write(const unsigned char* data, const size_t size) = 0;
    };
};
