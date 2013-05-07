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

    _thread  = boost::thread(&Client::read, this);
}

Client::~Client() {

}

void Client::read() {
    while ( true )
    {
        net::Address sender;
        unsigned char buffer[BUFFER_SIZE];

        unsigned long bytes_received = socket.Receive( sender, buffer, BUFFER_SIZE);

        if(bytes_received)
        {
            NetworkPacket np(buffer);
            std::cout << np.get<int>(0) << std::endl;
            std::cout << np.get_array<char*>(1) << std::endl;
            std::cout << std::endl;
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
