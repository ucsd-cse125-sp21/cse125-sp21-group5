#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../Shared/Header.h"
#include "../Shared/Event.h"
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/stream.hpp>

using boost::asio::ip::tcp;
using namespace std;

class Client {
	public:
		void callServer(Event& e);
};
