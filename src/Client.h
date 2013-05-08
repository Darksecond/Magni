#pragma once

#include "Network.h"
#include "NetworkPacket.h"
#include "Gameplay.h"
#include <GLM/glm.hpp>

namespace Ymir
{
    class Gameplay;

    class Client : public Network
    {
        private:
            boost::thread _thread;
            int a, b, c, d, _datasize;

        public:
            Client();
            virtual ~Client();

            Gameplay* gp;

            virtual void read();
            virtual void write(const unsigned char* data, const size_t size);
            void setIPAdress(int a, int b, int c, int d);
    };
};
