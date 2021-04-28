#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "../Shared/Event.h"

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    return "Hello world!";
}

int main()
{
    /*
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ip::address_v4 addrV4(boost::asio::ip::make_address_v4("192.168.56.1"));
        tcp::acceptor acceptor(io_context, tcp::endpoint(addrV4, 13));

        std::cout << "Starting server on local port 13" << std::endl;

        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Accepting new connection from " << socket.remote_endpoint().address().to_string() << std::endl;

            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            Event* convertedEvent = reinterpret_cast<Event*>(&buf);

            if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(convertedEvent->test, len);

            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    */

    return 0;
}