#pragma once
#include <random>
#include "Vector2.h"
#include "Vector3.h"

class RandomGenerator 
{
public:

	static RandomGenerator* GetInstance();

	int RandInt(int min, int max);

	float RandFloat(float min, float max);

	Vector2 RandVector2(float min, float max);

	Vector3 RandVector3(float min, float max);

	Vector3 RandVector3(const Vector3& min, const Vector3& max);

private:
	RandomGenerator() = default;
	~RandomGenerator() = default;
	RandomGenerator(const RandomGenerator&) = delete;
	RandomGenerator& operator=(const RandomGenerator&) = delete;

private:
	static std::random_device seedGenerator_;
	static std::mt19937_64 randomEngine_;

};