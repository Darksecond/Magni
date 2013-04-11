#include "Network.h"

using namespace Ymir;

Network::Network() {

}

Network::~Network() {

}
/*
void Network::createListenerThread() {
	net::InitializeSockets();

	printf( "creating socket on port %d\n", port );

	if (!socket.Open( port )) {
		printf( "failed to create socket!\n" );
	}

	while ( true )
	{
		const char data[] = "hello world!";

		socket.Send( net::Address(127,0,0,1, port), data, sizeof(data) );

		while ( true )
		{
			net::Address sender;
			unsigned char buffer[256];

			int bytes_read = socket.Receive( sender, buffer, sizeof( buffer ) );

			if ( !bytes_read )
				break;

			printf( "received packet from %d.%d.%d.%d:%d (%d bytes)\n",
				sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(),
				sender.GetPort(), bytes_read );
		}

		net::wait( 1.00f );
	}

	net::ShutdownSockets();
}
*/
