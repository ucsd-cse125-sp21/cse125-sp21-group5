#include "Server.h"

using namespace std;

void Server::do_read(int playerId) {
    boost::asio::async_read_until(connections[playerId]->getSocket(), bufs[playerId], "\r\n\r\n",
        boost::bind(&Server::handle_read,
            this,
            playerId,
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
        boost::asio::buffers_begin(bufs[playerId].data()),
        boost::asio::buffers_begin(bufs[playerId].data()) + bytes_transferred - 4 }; // -4 for \r\n\r\n
    bufs[playerId].consume(bytes_transferred);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_transferred);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> e;
    
    //Update camera position and create a game state to send back
    gm.handleEvent(e, playerId);

    do_read(playerId);
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
        for (int i = 0; i < NUM_PLAYERS; i++) {
            char hBuf[PACKET_SIZE];
            boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
            boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

            boost::archive::text_oarchive hAR(hSource);
            hAR << server.gm.getGameState(i);
            hSource << "\r\n\r\n";
            hSource << '\0';

            boost::asio::write(server.connections[i]->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);
        }
    }

    return 0;
}