#pragma once

#include <vector>
#include <string>
#include "Matrix4x4.h"

struct NodeData
{
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<NodeData> children;
};