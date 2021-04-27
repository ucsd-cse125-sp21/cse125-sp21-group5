#include "Client.h"

// Somehow get server to give us unique id
Client::Client() : id(0) {}

Client::Client(const unsigned int& id) : id(id) {}

void Client::callServer()
{
    /*
    unsigned short port = 13;

    boost::asio::io_context io_context;
    tcp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), port);

    tcp::socket socket(io_context, endpoint.protocol());
    socket.connect(endpoint);

    Header header(sizeof(e));

    char hBuf[PACKET_SIZE];
    char eBuf[PACKET_SIZE];

    //Header
    boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
    boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);


    boost::archive::text_oarchive hAR(hSource);
    hAR << header;
    hSource << "\r\n\r\n";
    hSource << '\0';


    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(hBuf, strlen(hBuf)), error);
    */
}


void Client::callFakeServer(const Event& e)
{

}