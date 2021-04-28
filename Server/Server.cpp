#include "Server.h"

using namespace std;

void tcp_connection::do_read() {
    boost::asio::async_read_until(socket_, buf, "\r\n\r\n",
        boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_read(boost::system::error_code error, size_t bytes_transferred) {
    if (error) {
        return;
    }

    Event e;

    std::string s{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_transferred - 4 }; // -4 for \r\n\r\n
    buf.consume(bytes_transferred);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_transferred);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> e;
    cout << "GOT FROM CLIENT" << e.dirX << endl;

    do_read();
}
   

int main()
{
    boost::asio::io_context ioContext;

    boost::shared_ptr<Server> server;
    server.reset(new Server(ioContext));
    ioContext.run();

    for (;;) {
        //game loop
    }

    return 0;
}