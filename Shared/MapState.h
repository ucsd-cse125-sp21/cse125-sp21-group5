#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>

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
		ar& scale.x;
		ar& scale.y;
		ar& scale.z;
		ar& rotation.x;
		ar& rotation.y;
		ar& rotation.z;
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

