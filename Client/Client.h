#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../Shared/Event.h"
#include <string>

using boost::asio::ip::tcp;
using namespace std;

class Client {
	public:
		void callServer();
};
