#include "Server.h"

using namespace std;

#define PACKET_SIZE 4096

void Server::do_read() {
    boost::asio::async_read_until(connection->getSocket(), buf, "\r\n\r\n",
        boost::bind(&Server::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void Server::handle_read(boost::system::error_code error, size_t bytes_transferred) {
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
    
    //Update camera position and create a game state to send back
    player1Cam.pos += glm::vec3(e.dirX, e.dirY, e.dirZ) * e.speed;
    GameState gs(player1Cam.pos);

    //Send to client
    cout << "WRITING TO Client" << endl;

    char hBuf[PACKET_SIZE];
    boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
    boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

    boost::archive::text_oarchive hAR(hSource);
    hAR << gs;
    hSource << "\r\n\r\n";
    hSource << '\0';

    boost::asio::write(connection->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);

    do_read();
}


int main()
{
    boost::asio::io_context ioContext;

    Server server(ioContext);
    ioContext.run();

    for (;;) {
        //game loop
    }

    return 0;
}