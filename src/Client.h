#pragma once

#include "Network.h"
#include "NetworkPacket.h"

namespace Ymir
{
    class Client : public Network
    {
        private:
            boost::thread _thread;
            int a, b, c, d, _datasize;

        public:
            Client();
            virtual ~Client();

            virtual void read();
            virtual void write(const unsigned char* data, const size_t size);
            void setIPAdress(int a, int b, int c, int d);
    };
};
