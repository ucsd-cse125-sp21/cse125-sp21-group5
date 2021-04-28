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


class Client {
	public:
		typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
		tcp_connection_ptr connection;

		void callServer(Event& e);
		void processRead(
			boost::system::error_code error, 
			size_t bytes_read
		);

		Client(boost::asio::io_context& ioContext)
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

			cout << "FINISHED CREATING CLIENT OBJ" << endl;

			cout << "WRITING TO SERVER" << endl;
			char hBuf[4096];

			//Header
			boost::iostreams::basic_array_sink<char> hSink(hBuf, 4096);
			boost::iostreams::stream<boost::iostreams::basic_array_sink<char>> hSource(hSink);

			boost::archive::text_oarchive hAR(hSource);
			hAR << Event(glm::vec3(1,2,3), 1, glm::vec3(1,2,3));
			hSource << "\r\n\r\n";
			hSource << '\0';


			boost::system::error_code error;
			boost::asio::write(connection->getSocket(), boost::asio::buffer(hBuf, strlen(hBuf)), error);
		}
private:
	boost::asio::streambuf buf;
	boost::asio::io_context& io_context_;
};