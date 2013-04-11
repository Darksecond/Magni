#include "Client.h"

using namespace Ymir;

Client::Client()
{
    net::InitializeSockets();

    int clientport = 29999;
	printf( "creating socket on port %d\n", clientport );

	if (!socket.Open( clientport )) {
		printf( "failed to create socket!\n" );
	}

    _thread  = boost::thread(&Client::setupSender, this);
    //boost::thread *t =  boost::thread(&Client::setupSender, this);
}

Client::~Client() {

}

void Client::setupSender() {
	while (true) {
		const char data[] = "hello world!";
		socket.Send( net::Address(127,0,0,1, port), data, sizeof(data) );
	}
}

void Client::read() {

}

void Client::write() {

}
