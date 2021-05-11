#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>

#include "../Shared/Event.h"
#include "../Shared/GameState.h"
#include "../Shared/MapState.h"
#include "../Shared/Header.h"
#include "../Shared/NetworkEvents.h"
#include "ServerGameManager.h"

using boost::asio::ip::tcp;

using namespace std;
#define NUM_PLAYERS 1
#define PACKET_SIZE 1024


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
};


class Server
{
public:
	typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
	vector<tcp_connection_ptr> connections;
	ServerGameManager gm;
	MapState ms;
	std::shared_ptr<tcp::acceptor> acceptor;

	Server(boost::asio::io_context& ioContext);

	void start_server();
	void do_read(int playerId);
	void handle_accept(int playerId, boost::system::error_code error);
	void handle_read(int playerId, boost::system::error_code error, size_t bytes_read);
	void broadcast_send(ClientConnectEvent ev, int ignore_clientID = -1);

private:
	vector<boost::asio::streambuf> bufs;
};