#pragma once

#include <iostream>
#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../Shared/Event.h"
#include "../Shared/GameState.h"
#include "../Shared/MapState.h"
#include "../Shared/Header.h"
#include "../Shared/NetworkEvents.h"
#include "GameManager.h"
#include "Camera.h"
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include "Transform.h"
#include "Model.h"

#include <glm/gtx/string_cast.hpp>


using boost::asio::ip::tcp;
using namespace std;


class tcp_connection
	: public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_context& io_context)
	{
		return pointer(new tcp_connection(io_context));
	}

	tcp::socket& getSocket()
	{
		return socket_;
	}

private:
	tcp_connection(boost::asio::io_context& io_context)
		: socket_(io_context)
	{
	}

	void handle_write(const boost::system::error_code& /*error*/,
		size_t /*bytes_transferred*/)
	{
	}

	tcp::socket socket_;
	std::string message_;
};


class Client {
public:
	GameManager gm;

	typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
	tcp_connection_ptr connection;
	int clientId;
	vector<int> existing_IDs;

	void callServer();

	Client(boost::asio::io_context& ioContext, GLFWwindow* window);

	void start_client() {
		do_read_header();
	}

	void do_read_header();
	void handle_read_header(boost::system::error_code error, size_t bytes_read);
	void handle_read_clientID();
	void handle_read_client_connect_update();
	void handle_read_game_state();
	void handle_read_map_state_update();

private:
	boost::asio::streambuf buf;
	boost::asio::io_context& io_context_;
};