#pragma once

#ifndef SERIALIZATION_STOCK_HPP
#define SERIALIZATION_STOCK_HPP

class Header {
public:
	int length = 0;

	Header(int l) {
		length = l;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& length;
	}
};

#endif // SERIALIZATION_STOCK_HPP