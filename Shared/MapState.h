#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>

using namespace std;
class MapState
{
public:
	// TODO: how to send floats?
	//vector<vector<float>> transforms;
	vector<float> transform1;
	vector<float> transform2;
	vector<float> transform3;

	MapState();

	void add(glm::mat4 transform);

	template <typename Archive>
	/*
	void serialize(Archive& ar, const unsigned int version) {
		ar& transforms;
	}
	*/
	void serialize(Archive& ar, const unsigned int version) {
		ar& transform1;
		ar& transform2;
		ar& transform3;
	}
};

