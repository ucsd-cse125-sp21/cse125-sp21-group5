#pragma once

#ifndef SERIALIZATION_STOCK_HPP
#define SERIALIZATION_STOCK_HPP

class Header {
public:
	int length = 0;
	float dirX, dirY, dirZ;
	float speed;
	float posX, posY, posZ;

	Header(int l) {
		length = l;
		dirX = 15;
		dirY = 15;
		dirZ = 15;
		speed = 1;
		posX = 15;
		posY = 15;
		posZ = 15;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& length;

		ar& dirX;
		ar& dirY;
		ar& dirZ;
		ar& speed;
		ar& posX;
		ar& posY;
		ar& posZ;

	}
};

#endif // SERIALIZATION_STOCK_HPP