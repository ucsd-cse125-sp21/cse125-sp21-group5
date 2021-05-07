#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>

using namespace std;
class MapState
{
public:
	// TODO: how to send floats?
	vector<vector<float>> transforms;

	MapState();

	void add(glm::mat4 transform);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& transforms;
	}
};

