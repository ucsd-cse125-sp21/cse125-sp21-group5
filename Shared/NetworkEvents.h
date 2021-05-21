#pragma once

#include <vector>

#include <boost/serialization/vector.hpp>

class ClientIDEvent {
public:
	int clientID;
	ClientIDEvent() {
		clientID = -1;
	}
	ClientIDEvent(int id) {
		clientID = id;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& clientID;
	}

	~ClientIDEvent() {}

};

class ClientConnectEvent {
public:
	std::vector<int> ids;
	ClientConnectEvent() {}
	ClientConnectEvent(std::vector<int> ids) {
		for (int i = 0; i < ids.size(); i++) {
			this->ids.push_back(ids.at(i));
		}
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& ids;
	}

	~ClientConnectEvent() {
		ids.clear();
	}
};