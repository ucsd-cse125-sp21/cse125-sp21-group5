#include "Client.h"

//Increase if too large
#define PACKET_SIZE 1024

Client::Client(boost::asio::io_context& ioContext, GLFWwindow* window, boost::asio::ip::address_v4 inputIp)
    : io_context_(ioContext) , gm(window)
{
    string port = "13";
    string host = inputIp.to_string();


    cout << "Connecting client to: " << host << endl;
    boost::asio::ip::tcp::resolver resolver(ioContext);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(
        host,
        port
    );

    connection = tcp_connection::create(ioContext);
    boost::asio::connect(connection->getSocket(), endpoints);

    cout << "Connected to server!" << endl;
    start_client();
}

void Client::callServer()
{
    char hBuf[PACKET_SIZE];

    Event e = gm.update();

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

void Client::do_read_header() {
    boost::asio::async_read_until(connection->getSocket(), buf, "\r\n\r\n",
        boost::bind(&Client::handle_read_header, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void Client::handle_read_header(boost::system::error_code error, size_t bytes_read) {
    if (error) {
        cout << error.message() << endl;
    }

    Header head;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    };
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> head;

    switch (head.msgType) {
    case HeaderType::NewClientID:
        cout << "Received NewClientID header" << endl;
        handle_read_clientID();
        break;
    case HeaderType::ClientConnectUpdate:
        cout << "Received ClientConnectUpdate header" << endl;
        handle_read_client_connect_update();
        break;
    case HeaderType::GameStateUpdate:
        //cout << "Received GameStateUpdate header" << endl;
        handle_read_game_state();
        break;
    case HeaderType::MapStateUpdate:
        cout << "Received MapStateUpdate header" << endl;
        handle_read_map_state_update();
        break;
    default:
        cout << "Cannot parse Header of Type: " << int(head.msgType) << endl;
    }
}

void Client::handle_read_clientID() {
    cout << "Inside handle_read_clientID" << endl;
    size_t bytes_read = boost::asio::read_until(connection->getSocket(), buf, "\r\n\r\n");
    ClientIDEvent id;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> id;

    clientId = id.clientID;
    cout << "\t\tReceived client Id from server is " << clientId << endl;

    // Update local player ID in GameManager
    gm.addPlayer(id.clientID, nullptr);
    gm.setLocalPlayerID(id.clientID);
    
    do_read_header();
}

void Client::handle_read_client_connect_update() {
    cout << "Inside handle_read_client_connect_update" << endl;
    size_t bytes_read = boost::asio::read_until(connection->getSocket(), buf, "\r\n\r\n");
    ClientConnectEvent ev;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> ev;

    // Add all player ids
    existing_IDs.clear();
    for (int i = 0; i < ev.ids.size(); i++) 
    {
        if (ev.ids.at(i) != clientId)
        {
            existing_IDs.push_back(ev.ids.at(i));
            gm.addPlayer(ev.ids.at(i), gm.playerModel);
        }
    }
    
    cout << "Client received the following client IDs from the server." << endl;
    for (int i = 0; i < existing_IDs.size(); i++) {
        cout << existing_IDs.at(i) << "\t";
    }

    cout << endl;
    do_read_header();
}

void Client::handle_read_game_state() {
    boost::system::error_code error;
    size_t bytes_read = boost::asio::read_until(connection->getSocket(), buf, "\r\n\r\n", error);
    
    if (error) {
        cout << "Error when reading game state message:\t";
        cout << error.message() << endl;
        return;
    }

    GameState gs;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    try {
        boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
        boost::archive::text_iarchive eAR(eSource);
        eAR >> gs;
    }
    catch (exception e) {
        cout << "Error when serializing game state:\t";
        cout << e.what() << endl;
    }

    gm.updateGameState(gs);

    do_read_header();
}

void Client::handle_read_map_state_update()
{
    size_t bytes_read = boost::asio::read_until(connection->getSocket(), buf, "\r\n\r\n");
    MapState ms;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> ms;

    gm.updateMap(ms);
    do_read_header();
}