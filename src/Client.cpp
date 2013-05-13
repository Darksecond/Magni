#include "Client.h"

using namespace Ymir;

static const int BUFFER_SIZE = 512;

Client::Client()
{
    net::InitializeSockets();
    _datasize = 0;
	printf( "creating socket on port %d\n", port );

	if (!socket.Open( port )) {
		printf( "failed to create socket!\n" );
	}

    ung = new UniqueNumberGenerator();
}

Client::~Client() {

}

void Client::read() {
    while ( false )
    {
        net::Address sender;
        unsigned char buffer[BUFFER_SIZE];

        unsigned long bytes_received = socket.Receive( sender, buffer, BUFFER_SIZE);

        if(bytes_received)
        {
            NetworkPacket np(buffer);
            std::cout << "Type: " << np.type() << std::endl;

            if(np.type() == Gameplay::BUILD) {
                if(np.get<int>(Gameplay::B_INFANTRY)) {
                    glm::vec3 position = glm::vec3(np.get<float>(1), np.get<float>(2), np.get<float>(3));
                    gp->createGhostWorker(position, ung->flip(np.id()));
                }
            }

            if(np.type() == Gameplay::MOVE) {
                glm::vec3 position = glm::vec3(np.get<float>(0), np.get<float>(1), np.get<float>(2));
                gp->moveEntity(position, ung->flip(np.id()));
            }
        }
    }
}

void Client::readReal()
{
    net::Address sender;
    unsigned char buffer[BUFFER_SIZE];

    unsigned long bytes_received = socket.Receive( sender, buffer, BUFFER_SIZE);

    if(bytes_received)
    {
        NetworkPacket np(buffer);
        std::cout << "Type: " << np.type() << std::endl;

        if(np.type() == Gameplay::BUILD) {
            if(np.get<int>(Gameplay::B_INFANTRY)) {
                glm::vec3 position = glm::vec3(np.get<float>(1), np.get<float>(2), np.get<float>(3));
                gp->createGhostWorker(position, np.id());
            }
        }

        if(np.type() == Gameplay::MOVE) {
            std::cout << "Derp" << std::endl;
            glm::vec3 position = glm::vec3(np.get<float>(0), np.get<float>(1), np.get<float>(2));
            gp->moveEntity(position, np.id());
        }
    }
}

void Client::write(const unsigned char *data, const size_t size) {
    socket.Send( net::Address(a,b,c,d, port), data, size );
}

void Client::setIPAdress(int aa, int bb, int cc, int dd) {
    a = aa;
    b = bb;
    c = cc;
    d = dd;
}
