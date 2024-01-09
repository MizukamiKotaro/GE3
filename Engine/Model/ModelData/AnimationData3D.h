#pragma once

#include <vector>
#include <string>
#include "Matrix4x4.h"
#include "Quaternion.h"

struct NodeAnime
{
	struct Positions
	{
		float time;
		Vector3 position;
	};

	struct Rotates
	{
		float time;
		Quaternion rotate;
	};

	struct Scales
	{
		float time;
		Vector3 scale;
	};

	std::string name;
	std::vector<Positions> positions;
	std::vector<Rotates> rotates;
	std::vector<Scales> scales;
};

struct AnimationData
{
	float duration;
	float ticksPerSecond;
	std::vector<NodeAnime> channels;
};