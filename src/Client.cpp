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

void Client::readReal()
{
    net::Address sender;
    unsigned char buffer[BUFFER_SIZE];

    unsigned long bytes_received = socket.Receive( sender, buffer, BUFFER_SIZE);

    if(bytes_received)
    {
        NetworkPacket np(buffer);

        if(np.type() == Gameplay::BUILD) {
            glm::vec3 position = glm::vec3(np.get<float>(1), np.get<float>(2), np.get<float>(3));
            if(np.get<int>(0) == Gameplay::WORKER) {
                gp->createGhostWorker(position, ung->flip(np.id()));
            }
            if (np.get<int>(0) == Gameplay::B_INFANTRY) {
                gp->createGhostBasicInfantrie(position, ung->flip(np.id()));
            }
            if (np.get<int>(0) == Gameplay::ORBITAL) {
                gp->buildGhostOrbitalDropBeacon(position, ung->flip(np.id()));
            }
            if (np.get<int>(0) == Gameplay::CICORE) {
                gp->buildGhostCentralIntelligenceCore(position, ung->flip(np.id()));
            }
        }

        if(np.type() == Gameplay::MOVE) {
            glm::vec3 position = glm::vec3(np.get<float>(0), np.get<float>(1), np.get<float>(2));
            gp->moveEntity(position, ung->flip(np.id()));
        }

        if(np.type() == Gameplay::ATTACK) {
            gp->attackEntity(ung->flip(np.id()), ung->flip(np.get<int>(0)));
        }

        if(np.type() == Gameplay::WIN_LOSE) {
            gp->winGame();
        }

        if(np.type() == Gameplay::SELL) {
            gp->removeEntity(ung->flip(np.id()));
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
