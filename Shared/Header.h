#pragma once

#ifndef SERIALIZATION_STOCK_HPP
#define SERIALIZATION_STOCK_HPP

enum HeaderType : int {
	NewClientID,
	ClientConnectUpdate,
	GameStateUpdate,
	MapStateUpdate
};

//template <typename Archive>
//void serialize(Archive& ar, HeaderType & h, const unsigned int version) {
//	ar& h.msgType;
//}

class Header {
public:
	HeaderType msgType;
	Header() {}
	Header(HeaderType msgType) {	
		this->msgType = msgType;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& msgType;
	}
};

#endif // SERIALIZATION_STOCK_HPP