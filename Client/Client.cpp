#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        std::cout << "Starting Client" << std::endl;
        std::string ip = "192.168.56.1";
        unsigned short port = 13;

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(boost::asio::ip::make_address_v4(ip), port);

        tcp::socket socket(io_context, endpoint.protocol());
        socket.connect(endpoint);
        std::cout << "Starting Client" << std::endl;
        for (;;)
        {
            std::cout << "Starting Client" << std::endl;
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
        std::cout << "e Client" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}