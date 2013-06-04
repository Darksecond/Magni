#pragma once

#include "Network.h"
#include "NetworkPacket.h"
#include "Gameplay.h"
#include <GLM/glm.hpp>
#include "UniqueNumberGenerator.h"

namespace Ymir
{
    class Gameplay;

    class Client : public Network
    {
        private:
            int a, b, c, d, _datasize;
            UniqueNumberGenerator* ung;

        public:
            Client();
            virtual ~Client();

            Gameplay* gp;

            virtual void read(){}
            virtual void write(const unsigned char* data, const size_t size);
            void setIPAdress(int a, int b, int c, int d);
            void readReal();
    };
};
