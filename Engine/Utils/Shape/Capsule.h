#pragma once
#include "IShape.h"
#include "Vector3.h"
#include "Line.h"

class Capsule : public IShape {
public:

	float radius_;

	Segment segment_;

};