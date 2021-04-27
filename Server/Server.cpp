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

            //Header header(0);

            ////Convert from input stream into a string
            //boost::asio::streambuf buf;
            //std::size_t n = boost::asio::read_until(socket, buf, "\r\n\r\n");

            //std::string s{
            //    boost::asio::buffers_begin(buf.data()),
            //    boost::asio::buffers_begin(buf.data()) + n - 4 }; // -4 for \r\n\r\n
            //buf.consume(n);

            ////Deserialize string into header object
            //boost::iostreams::stream<boost::iostreams::array_source> hSource(s.data(), n);
            //boost::archive::text_iarchive hAR(hSource);
            //hAR >> header;

            //std::cout << header.length << std::endl;

            //Event
            Event e;
            boost::asio::streambuf eBuf;
            size_t n = boost::asio::read_until(socket, eBuf, "\r\n\r\n");
            


            std::string s = std::string{
                boost::asio::buffers_begin(eBuf.data()),
                boost::asio::buffers_begin(eBuf.data()) + n - 4 
            }; // -4 for \r\n\r\n
            eBuf.consume(n);

            boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), n);
            boost::archive::text_iarchive eAR(eSource);
            eAR >> e;

            std::cout << e.dirX << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}