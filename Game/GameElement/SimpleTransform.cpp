#include "SimpleTransform.h"

void SimpleTransform::CalcTransMat() {
	worldMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, translate_);
}
