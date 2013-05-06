#include "Client.h"

using namespace Ymir;

Client::Client()
{
    net::InitializeSockets();

	printf( "creating socket on port %d\n", port );

	if (!socket.Open( port )) {
		printf( "failed to create socket!\n" );
	}

    _thread  = boost::thread(&Client::read, this);
}

Client::~Client() {

}

void Client::read() {
    while ( true )
    {
        net::Address sender;
        unsigned char buffer[256];

        socket.Receive( sender, buffer, sizeof( buffer ));

        if(sender.GetA() != 0)
            std::cout << buffer << std::endl;
    }
}

void Client::write(unsigned char *data) {
    socket.Send( net::Address(a,b,c,d, port), data, 256 );
}

void Client::setIPAdress(int aa, int bb, int cc, int dd) {
    a = aa;
    b = bb;
    c = cc;
    d = dd;
}
