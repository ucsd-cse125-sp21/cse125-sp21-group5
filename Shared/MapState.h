#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>

using namespace std;
class MapState
{
public:
	vector<float[16]> transforms;

	MapState();

	void add(glm::mat4 transform);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& transforms;
	}
};

