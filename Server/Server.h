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
#include "../Shared/Camera.h"
#include "../Shared/GameState.h"
#include "ServerGameManager.h"

using boost::asio::ip::tcp;

using namespace std;
#define NUM_PLAYERS 2

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


class Server{
public:
	typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
	vector<tcp_connection_ptr> connections;
	ServerGameManager gm;

	Server(boost::asio::io_context& ioContext) : 
		connections(NUM_PLAYERS), 
		bufs(NUM_PLAYERS) 
	{
		boost::asio::ip::address_v4 addrV4(boost::asio::ip::address_v4::loopback());
		tcp::acceptor acceptor(ioContext, tcp::endpoint(addrV4, 13));

		cout << "Starting server on local port 13" << endl;

		for (int i = 0; i < NUM_PLAYERS; i++) {
			connections[i] = tcp_connection::create(ioContext);
			acceptor.accept(connections[i]->getSocket());
			cout << "Accepting new connection from " << connections[i]->getSocket().remote_endpoint().address().to_string() << endl;

		}

		// start reading from client
		start_server();
	}

	void start_server() {
		for (int i = 0; i < NUM_PLAYERS; i++) {
			do_read(i);
		}
	}

	void do_read(int playerId);
	void handle_read(int playerId, boost::system::error_code error, size_t bytes_read);

private:
	vector<boost::asio::streambuf> bufs;
};