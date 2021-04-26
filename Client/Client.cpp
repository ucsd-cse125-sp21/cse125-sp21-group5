#include "Client.h"

// Somehow get server to give us unique id
Client::Client() : id(0) {}

Client::Client(const unsigned int& id) : id(id) {}

void Client::callServer()
{
    try
    {
        std::string ip = "192.168.56.1";
        unsigned short port = 13;

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(boost::asio::ip::make_address_v4(ip), port);

        tcp::socket socket(io_context, endpoint.protocol());
        socket.connect(endpoint);
        for (;;)
        {
            Event e = Event();
            string convertedEvent = reinterpret_cast<char*>(&e);
            boost::system::error_code ignored_error;

            boost::asio::write(socket, boost::asio::buffer(convertedEvent), ignored_error);

            /*
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
            */
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}


void Client::callFakeServer(const Event& e)
{

}