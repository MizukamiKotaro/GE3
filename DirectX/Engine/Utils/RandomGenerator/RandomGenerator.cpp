#include "RandomGenerator.h"

std::random_device RandomGenerator::seedGenerator_;
std::mt19937_64 RandomGenerator::randomEngine_(seedGenerator_());

RandomGenerator* RandomGenerator::GetInstance()
{
	static RandomGenerator instance;
	return &instance;
}

int RandomGenerator::RandInt(int min, int max)
{
	std::uniform_real_distribution<> distribution(min, max);
	return static_cast<int>(distribution(randomEngine_));
}

float RandomGenerator::RandFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomEngine_);
}

Vector2 RandomGenerator::RandVector2(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return Vector2{ distribution(randomEngine_),distribution(randomEngine_) };
}

Vector3 RandomGenerator::RandVector3(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return Vector3{ distribution(randomEngine_),distribution(randomEngine_),distribution(randomEngine_) };
}

Vector3 RandomGenerator::RandVector3(const Vector3& min, const Vector3& max)
{
	return Vector3(RandFloat(min.x,max.x), RandFloat(min.y, max.y), RandFloat(min.z, max.z));
}
