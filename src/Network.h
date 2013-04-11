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

    public:
        Network();
        virtual ~Network();

        virtual void read() = 0;
        virtual void write() = 0;
    };
};
