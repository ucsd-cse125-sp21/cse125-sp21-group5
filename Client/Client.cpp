#include "Client.h"

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

            boost::asio::streambuf buf;
            std::ostream os(&buf);
            boost::archive::binary_oarchive ar(os);
            ar & header;

            boost::asio::write(socket, buf);
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