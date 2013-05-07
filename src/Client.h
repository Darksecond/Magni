#pragma once

#include "Network.h"

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
            virtual void write(const unsigned char* data);
            void setIPAdress(int a, int b, int c, int d);
    };
};
