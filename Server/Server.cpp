#include "Server.h"

std::string make_daytime_string()
{
    return "Hello world!";
}

int main()
{
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ip::address_v4 addrV4(boost::asio::ip::address_v4::loopback());
        tcp::acceptor acceptor(io_context, tcp::endpoint(addrV4, 13));

        std::cout << "Starting server on local port 13" << std::endl;

        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Accepting new connection from " << socket.remote_endpoint().address().to_string() << std::endl;

            Header header(0);

            //Convert from input stream into a string
            boost::asio::streambuf buf;
            Sleep(3);
            size_t lengthRead = boost::asio::read(socket, buf);
          
            //Deserialize string into header object
            std::cout << lengthRead << std::endl;
            std::istream archive_stream(&buf);

            std::cout << "222222" << std::endl;
            boost::archive::binary_iarchive archive(archive_stream);
            std::cout << " 3 " << std::endl;

            archive >> header;

            std::cout << "Length of header from server is: " << header.length << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}