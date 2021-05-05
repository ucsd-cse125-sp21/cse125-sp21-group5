#include "Server.h"

using namespace std;

#define PACKET_SIZE 4096

void Server::do_read() {
    boost::asio::async_read_until(connection->getSocket(), buf, "\r\n\r\n",
        boost::bind(&Server::handle_read,
            this,
            0,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

void Server::handle_read(int playerId, boost::system::error_code error, size_t bytes_transferred) {
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
    gm.handleEvent(e, playerId);

    do_read();
}

int main()
{
    boost::asio::io_context ioContext;

    Server server(ioContext);

    boost::thread_group worker_threads;
    worker_threads.create_thread(
        boost::bind(&boost::asio::io_service::run, &ioContext)
    );

    cout << "RUNNING STUFF" << endl;
    boost::system::error_code error;

    auto start = chrono::steady_clock::now();
    int a = 0;

    while (true) {
        auto end = chrono::steady_clock::now();
        if ((end - start) <= std::chrono::milliseconds(16))
        {
            continue;
        }
        start = chrono::steady_clock::now();

        //Send to client
        char hBuf[PACKET_SIZE];
        boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
        boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

        boost::archive::text_oarchive hAR(hSource);
        hAR << server.gm.getGameState(0);
        hSource << "\r\n\r\n";
        hSource << '\0';

        boost::asio::write(server.connection->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);
    }

    return 0;
}