#include "Server.h"

using namespace Ymir;

Server::Server() {
    net::InitializeSockets();

	printf( "creating socket on port %d\n", port );

	if (!socket.Open( port )) {
		printf( "failed to create socket!\n" );
	}
}

Server::~Server() {

}

void Server::setupListener() {
    while ( true )
    {
        net::Address sender;
        unsigned char buffer[256];

        int bytes_read = socket.Receive( sender, buffer, sizeof( buffer ));

        printf( "received packet from %d.%d.%d.%d:%d (%d bytes)\n",
            sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(),
            sender.GetPort(), bytes_read );
    }
}

void Server::read() {

}

void Server::write() {

}
