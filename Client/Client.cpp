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
        cout << "Received GameStateUpdate header" << endl;
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

    //camera->pos = gs.pos;
    //camera->front = gs.front;
    camera->update(gs.pos, gs.front);
    playerT->setTranslate(gs.pos + glm::vec3(5.0f, 0.0f, 0.0f));

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

    acquireGameInfo(ms);
    do_read_header();
}

void Client::acquireGameInfo(MapState& ms) {
    // Create and move objects in scene graph accordingly

    for (float t : ms.transform1)
    {
        //cerr << "CLIENT MAP STATE TRANSFORM" << endl;
        cerr << t << endl;
        /*cerr << t[1] << endl;
        cerr << t[2] << endl;
        cerr << t[3] << endl;
        cerr << t[4] << endl;
        cerr << t[5] << endl;
        cerr << t[6] << endl;
        cerr << t[7] << endl;
        cerr << t[8] << endl;
        cerr << t[9] << endl;
        cerr << t[10] << endl;
        cerr << t[11] << endl;
        cerr << t[12] << endl;
        cerr << t[13] << endl;
        cerr << t[14] << endl;
        cerr << t[15] << endl;*/ 
    }
    for (float t : ms.transform2)
    {
        cerr << t << endl;
    }    for (float t : ms.transform3)
    {
        cerr << t << endl;
    }

    Transform* cubeT1 = new Transform(ms.transform1);
    Transform* cubeT2 = new Transform(ms.transform2);
    Transform* cubeT3 = new Transform(ms.transform3);

    Model* cubeM = new Model("res/models/head2.dae");

    cubeT1->add_child(cubeM);
    cubeT2->add_child(cubeM);
    cubeT3->add_child(cubeM);

    worldT->add_child(cubeT1);
    worldT->add_child(cubeT2);
    worldT->add_child(cubeT3);
}