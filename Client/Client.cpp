#include "Client.h"

//Increase if too large
#define PACKET_SIZE 4096

Client::Client(boost::asio::io_context& ioContext)
    : io_context_(ioContext)
{
    cout << "CREATING NEW CLIENT OBJ" << endl;
    string port = "13";
    string host = boost::asio::ip::address_v4::loopback().to_string();

    boost::asio::ip::tcp::resolver resolver(ioContext);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(
        host,
        port
    );

    connection = tcp_connection::create(ioContext);
    boost::asio::connect(connection->getSocket(), endpoints);

    acquireGameInfo();
    start_client();

    cout << "FINISHED CREATING CLIENT OBJ" << endl;

}

void Client::callServer(Event& e)
{
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
    boost::asio::async_read_until(connection->getSocket(), buf, "\r\n\r\n",
        boost::bind(&Client::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void Client::handle_read(boost::system::error_code error, size_t bytes_read) {
    GameState gs;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> gs;

    camera->pos = gs.pos;
    camera->front = gs.front;
    camera->update(gs.pos, gs.front);

    playerT->setTranslate(gs.pos + glm::vec3(5.0f, 0.0f, 0.0f));

    do_read();
}

void Client::acquireGameInfo() {
    MapState ms;
    size_t bytes_read = boost::asio::read_until(connection->getSocket(), buf, "\r\n\r\n");
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> ms;

    

}