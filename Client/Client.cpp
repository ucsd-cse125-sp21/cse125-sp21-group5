#include "Client.h"

int PACKET_SIZE = 128;

void Client::callServer()
{
    try
    {
        unsigned short port = 13;

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), port);

        tcp::socket socket(io_context, endpoint.protocol());
        socket.connect(endpoint);
        for (;;)
        {
            Header header(15);

            char buf[128];
            
            boost::iostreams::basic_array_sink<char> sink(buf, PACKET_SIZE);
            boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> source(sink);

            boost::archive::text_oarchive ar(source);
            ar << header;
            source << "\r\n\r\n";
            source << '\0';

            boost::system::error_code error;
            boost::asio::write(socket, boost::asio::buffer(buf, strlen(buf)), error);


            break;
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