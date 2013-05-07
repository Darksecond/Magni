#include "Client.h"

using namespace Ymir;

Client::Client()
{
    net::InitializeSockets();
    _datasize = 0;
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
        if(_datasize != 0) {
            net::Address sender;
            unsigned char buffer[_datasize];

            socket.Receive( sender, buffer, sizeof( buffer ));

            if(sender.GetA() != 0)
                for(int i = 0; i < sizeof(buffer); i++)
                    std::cout << buffer[i];
        }
    }
}

void Client::write(const unsigned char *data) {
    _datasize = sizeof( data );
    socket.Send( net::Address(a,b,c,d, port), data, _datasize );
}

void Client::setIPAdress(int aa, int bb, int cc, int dd) {
    a = aa;
    b = bb;
    c = cc;
    d = dd;
}
