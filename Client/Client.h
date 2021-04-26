#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../Shared/Event.h"
#include <string>

using boost::asio::ip::tcp;
using namespace std;

class Client
{
public:
	const unsigned int id;

	Client();
	Client(const unsigned int& id);
	~Client();

	void callServer();
	void callFakeServer(const Event& e);
};

// TODO: Create fake server class to "handle" requests
class FakeServer
{

};
