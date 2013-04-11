#pragma once

#include "Network.h"

namespace Ymir
{
    class Client : public Network
    {
        private:
            boost::thread _thread;
        public:
            Client();
            virtual ~Client();

            void setupSender();
            virtual void read();
            virtual void write();
    };
};
