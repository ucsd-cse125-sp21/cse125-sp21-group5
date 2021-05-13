#include "Client.h"

//Increase if too large
#define PACKET_SIZE 1024

Client::Client(boost::asio::io_context& ioContext, GLFWwindow* window)
    : io_context_(ioContext) , gm(window)
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

    start_client();

    cout << "FINISHED CREATING CLIENT OBJ" << endl;

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

    existing_IDs.clear();

    for (int i = 0; i < ev.ids.size(); i++) {
        if (ev.ids.at(i) != clientId) {
            existing_IDs.push_back(ev.ids.at(i));
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
    //cout << "Inside handle_read_game_state" << endl;
    size_t bytes_read = boost::asio::read_until(connection->getSocket(), buf, "\r\n\r\n");
    GameState gs;
    std::string s = std::string{
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + bytes_read - 4
    }; // -4 for \r\n\r\n
    buf.consume(bytes_read);

    boost::iostreams::stream<boost::iostreams::array_source> eSource(s.data(), bytes_read);
    boost::archive::text_iarchive eAR(eSource);
    eAR >> gs;

    // TODO: move to game manager
    gm.camera->update(gs.pos, gs.front);
    gm.playerT->setTranslate(gs.pos + glm::vec3(5.0f, 0.0f, 0.0f));
    
    //Update flag position
    Transform* newTrans = new Transform(gs.flag1Pos);
    gm.cubeT1->transform = newTrans->transform;
    delete newTrans;

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

    // Tell GameManager to add objects accordingly
    gm.updateMap(ms);

    do_read_header();
}

void Client::acquireGameInfo(MapState& ms) {
    // Create and move objects in scene graph accordingly
    gm.updateMap(ms);
}