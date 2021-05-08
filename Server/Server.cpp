#include "Server.h"
#include "glm/gtx/string_cast.hpp"

using namespace std;

Server::Server(boost::asio::io_context& ioContext) :
    connections(NUM_PLAYERS),
    bufs(NUM_PLAYERS)
{
    boost::asio::ip::address_v4 addrV4(boost::asio::ip::address_v4::loopback());
    acceptor = std::make_shared< tcp::acceptor>(ioContext, tcp::endpoint(addrV4, 13));

    cout << "Starting server on local port 13" << endl;

    for (int i = 0; i < NUM_PLAYERS; i++) {
        connections[i] = tcp_connection::create(ioContext);
        acceptor->async_accept(connections[i]->getSocket(), boost::bind(&Server::handle_accept, this, i, boost::asio::placeholders::error));
        //cout << "Accepting new connection from " << connections[i]->getSocket().remote_endpoint().address().to_string() << endl;
    }

    // start reading from client
    start_server();
}

void Server::start_server() {
    // Generate Map
    this->ms = gm.generateMap();
}

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
    
    // Update camera position and create a game state to send back
    gm.handleEvent(e, playerId);
    cerr << "pos: " << to_string(e.pos) << endl;
    cerr << "pitch: " << to_string(e.pitch) << endl;
    cerr << "yaw: " << to_string(e.yaw) << endl;

    do_read(playerId);
}

void Server::handle_accept(int playerId, boost::system::error_code error) {
    if (error) {
        cout << error.message() << endl;
    }
    else {
        cout << "Accepting new connection from " << connections[playerId]->getSocket().remote_endpoint().address().to_string() << endl;

        /* Sending Client ID */
        Header head(HeaderType::NewClientID);
        char headBuf[PACKET_SIZE];
        boost::iostreams::basic_array_sink<char> headSink(headBuf, PACKET_SIZE);
        boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> headSource(headSink);

        boost::archive::text_oarchive headAR(headSource);
        headAR << head;
        headSource << "\r\n\r\n";
        headSource << '\0';

        boost::asio::write(connections[playerId]->getSocket(), boost::asio::buffer(headBuf, strlen(headBuf)), error);

        ClientIDEvent id(playerId);
        char hBuf[PACKET_SIZE];
        boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
        boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

        boost::archive::text_oarchive hAR(hSource);
        hAR << id;
        hSource << "\r\n\r\n";
        hSource << '\0';

        boost::asio::write(connections[playerId]->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);

        /* Sending Map State */
        Header msHead(HeaderType::MapStateUpdate);
        char msHeadBuf[PACKET_SIZE];
        boost::iostreams::basic_array_sink<char> msHeadSink(msHeadBuf, PACKET_SIZE);
        boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> msHeadSource(msHeadSink);

        boost::archive::text_oarchive msHeadAR(msHeadSource);
        msHeadAR << msHead;
        msHeadSource << "\r\n\r\n";
        msHeadSource << '\0';

        boost::asio::write(connections[playerId]->getSocket(), boost::asio::buffer(msHeadBuf, strlen(msHeadBuf)), error);

        char mshBuf[PACKET_SIZE];
        boost::iostreams::basic_array_sink<char> mshSink(mshBuf, PACKET_SIZE);
        boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> mshSource(mshSink);

        boost::archive::text_oarchive mshAR(mshSource);
        mshAR << this->ms;
        mshSource << "\r\n\r\n";
        mshSource << '\0';

        boost::asio::write(connections[playerId]->getSocket(), boost::asio::buffer(mshBuf, strlen(mshBuf)), error);

        vector<int> ids;

        for (int i = 0; i < connections.size(); i++) {
            ids.push_back(i);
        }

        ClientConnectEvent ev(ids);

        broadcast_send(ev);

        do_read(playerId);
    }

    
}

void Server::broadcast_send(ClientConnectEvent ev, int ignore_clientID) {
    boost::system::error_code error;

    Header head(HeaderType::ClientConnectUpdate);
    char headBuf[PACKET_SIZE];
    boost::iostreams::basic_array_sink<char> headSink(headBuf, PACKET_SIZE);
    boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> headSource(headSink);

    boost::archive::text_oarchive headAR(headSource);
    headAR << head;
    headSource << "\r\n\r\n";
    headSource << '\0';

    char hBuf[PACKET_SIZE];
    boost::iostreams::basic_array_sink<char> hSink(hBuf, PACKET_SIZE);
    boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

    boost::archive::text_oarchive hAR(hSource);
    hAR << ev;
    hSource << "\r\n\r\n";
    hSource << '\0';

    for (int playerId = 0; playerId < connections.size(); playerId++) {
        if (playerId == ignore_clientID) {
            continue;
        }
        boost::asio::write(connections[playerId]->getSocket(), boost::asio::buffer(headBuf, strlen(headBuf)), error);
        boost::asio::write(connections[playerId]->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);
    }
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

            Header head(HeaderType::GameStateUpdate);
            char headBuf[PACKET_SIZE];
            boost::iostreams::basic_array_sink<char> headSink(headBuf, PACKET_SIZE);
            boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> headSource(headSink);

            boost::archive::text_oarchive headAR(headSource);
            headAR << head;
            headSource << "\r\n\r\n";
            headSource << '\0';

            boost::asio::write(server.connections[i]->getSocket(), boost::asio::buffer(headBuf, strlen(headBuf)), error);

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