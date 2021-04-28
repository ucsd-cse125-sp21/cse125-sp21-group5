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
#include <boost/bind.hpp>


using boost::asio::ip::tcp;
using namespace std;


class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_context& io_context) {
		return pointer(new tcp_connection(io_context));
	}

	tcp::socket& getSocket() {
		return socket;
	}

private:
	tcp::socket socket;

	tcp_connection(boost::asio::io_context& io_context)
		: socket(io_context) {}
};

class Client {
	public:
		typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
		tcp_connection_ptr connection;

		void callServer(Event& e);
		void processRead(
			boost::system::error_code error, 
			size_t bytes_read
		);

		Client() {
			cout << "CREATING NEW CLIENT OBJ" << endl;
			string port = "13";
			string host = "192.168.56.1";

			boost::asio::io_context io_context;
			boost::asio::ip::tcp::resolver resolver(io_context);
			boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(
				host,
				port
			);

			connection = tcp_connection::create(io_context);
			boost::asio::connect(connection->getSocket(), endpoints);

			boost::asio::async_read_until(connection->getSocket(), buf, "\r\n\r\n",
				boost::bind(
					&Client::processRead,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}
private:
	boost::asio::streambuf buf;
};