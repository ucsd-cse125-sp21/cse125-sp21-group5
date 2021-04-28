#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "../Shared/Event.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind/bind.hpp>


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

	void start() {
		do_read();
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

	void do_read();
	void handle_read(boost::system::error_code error, size_t bytes_read);

	tcp::socket socket_;
	boost::asio::streambuf buf;

};


class Server : public boost::enable_shared_from_this<Server> {
public:
	typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
	tcp_connection_ptr connection;

	Server(boost::asio::io_context& ioContext) {
		boost::asio::ip::address_v4 addrV4(boost::asio::ip::address_v4::loopback());
		tcp::acceptor acceptor(ioContext, tcp::endpoint(addrV4, 13));

		cout << "Starting server on local port 13" << endl;

		connection = tcp_connection::create(ioContext);
		acceptor.accept(connection->getSocket());
		cout << "Accepting new connection from " << connection->getSocket().remote_endpoint().address().to_string() << endl;

		// start reading from client
		connection->start();
	}
};
