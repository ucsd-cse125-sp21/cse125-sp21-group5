#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../Shared/Header.h"
#include <string>
#include <boost/archive/binary_oarchive.hpp>

using boost::asio::ip::tcp;
using namespace std;

class Client {
	public:
		void callServer();
};
