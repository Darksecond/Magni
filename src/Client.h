#pragma once

#include "Network.h"

namespace Ymir
{
    class Client : public Network
    {
        private:

        public:
            Client();
            virtual ~Client();

            virtual void read();
            virtual void write();
    };
};
