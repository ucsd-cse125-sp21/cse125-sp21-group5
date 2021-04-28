#include "Server.h"

using namespace std;

std::string make_daytime_string()
{
    return "Hello world!";
}

int main()
{

    try
    {
        boost::asio::io_service io_context;
        boost::asio::ip::address_v4 addrV4(boost::asio::ip::address_v4::loopback());
        tcp::acceptor acceptor(io_context, tcp::endpoint(addrV4, 13));

        cout << "Starting server on local port 13" << endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Accepting new connection from " << socket.remote_endpoint().address().to_string() << std::endl;

        for (;;)
        {
            //Event
            Event e;
            boost::asio::streambuf eBuf;

            boost::system::error_code error;
            size_t n = boost::asio::read_until(socket, eBuf, "\r\n\r\n", error);

            if (!error) {
                cout << "READ UNTIL" << endl;


                std::string s = std::string{
                    boost::asio::buffers_begin(eBuf.data()),
                    boost::asio::buffers_begin(eBuf.data()) + n - 4
                }; // -4 for \r\n\r\n
                eBuf.consume(n);

                boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), n);
                boost::archive::text_iarchive eAR(eSource);
                eAR >> e;

                cout << "GOT FROM CLIENT" << e.dirX << endl;
            }
            else if (error == boost::asio::error::eof) {
                cout << "EOF READ" << endl;
            }
            else {
                //cout << error << endl;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}