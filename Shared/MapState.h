#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

class MapPiece
{
public:
	glm::vec3 scale, rotation, translation;

	MapPiece();
	MapPiece(const glm::vec3& scale,
			 const glm::vec3& rotation,
			 const glm::vec3& translation);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		cout << "SCALE " << glm::to_string(scale) << endl;
		ar& scale.x;
		ar& scale.y;
		ar& scale.z;
		cout << glm::to_string(rotation) << endl;

		ar& rotation.x;
		ar& rotation.y;
		ar& rotation.z;
		cout << glm::to_string(translation) << endl;
		ar& translation.x;
		ar& translation.y;
		ar& translation.z;
	}
};

class MapState
{
public:
	vector<MapPiece> pieces;

	void addPiece(MapPiece& mp);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& pieces;
	}
};

