#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../Shared/Event.h"
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/enable_shared_from_this.hpp>


using boost::asio::ip::tcp;
using namespace std;


class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_context& io_context, tcp::endpoint& endpoint) {
		return pointer(new tcp_connection(io_context, endpoint));
	}

	tcp::socket& getSocket() {
		return socket;
	}

private:
	tcp::socket socket;

	tcp_connection(boost::asio::io_context& io_context, tcp::endpoint& endpoint)
		: socket(io_context, endpoint.protocol()) {}
};

class Client {
	public:
		typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
		tcp_connection_ptr connection;

		void callServer(Event& e);
		Client() {
			cout << "CREATING NEW CLIENT OBJ" << endl;
			unsigned short port = 13;

			boost::asio::io_context io_context;
			tcp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), port);

			connection = tcp_connection::create(io_context, endpoint);
			connection->getSocket().connect(endpoint);
		}
private:
};