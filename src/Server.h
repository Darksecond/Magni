#pragma once

#include "Network.h"

namespace Ymir
{
    class Server : public Network
    {
    private:
    public:
        Server();
        virtual ~Server();

        void setupListener();
        virtual void read();
        virtual void write();
    };
};
