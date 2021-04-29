#include "Client.h"

//Increase if too large
#define PACKET_SIZE 4096

void Client::callServer(Event& e)
{
    cout << "WRITING TO SERVER" << endl;

    char hBuf[PACKET_SIZE];

    //Header
    boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
    boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

    boost::archive::text_oarchive hAR(hSource);
    hAR << e;
    hSource << "\r\n\r\n";
    hSource << '\0';


    boost::system::error_code error;
    boost::asio::write(connection->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);
}

void Client::do_read() {
    cout << "DO READ ONCE" << endl;

    boost::asio::async_read_until(connection->getSocket(), buf, "\r\n\r\n",
        boost::bind(&Client::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void Client::handle_read(boost::system::error_code error, size_t bytes_read) {
    cout << "RECEIVED FROM SERVER" << endl;

    GameState gs;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> gs;

    camera->pos = glm::vec3(gs.posX, gs.posY, gs.posZ);

    do_read();
}