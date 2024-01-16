#include "Vector3.h"

#include <cmath>
#include <calc.h>

const Vector3 Vector3::zero = {};
const Vector3 Vector3::one = { 1.f,1.f,1.f };
const Vector3 Vector3::right = { .x = 1.f };
const Vector3 Vector3::up = { .y = 1.f };
const Vector3 Vector3::front = { .z = 1.f };

float Vector3::Length() const {
	float result = static_cast<float>(sqrt(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2)));
	return result;
}

float Vector3::Length(const Vector3 &pos) const {
	float result = static_cast<float>(sqrt(powf(this->x - pos.x, 2) + powf(this->y - pos.y, 2) + powf(this->z - pos.z, 2)));
	return result;
}

Vector3 Vector3::Normalize() const {
	float len = Length();

	Vector3 result = {};

	if (len != 0) {
		result = { this->x / len, this->y / len, this->z / len };
	}

	return result;
}

 Vector3 Vector3::Reflect(const Vector3 &normal) const { return (*this) - normal * (2.f * Calc::Dot((*this), normal)); }


Vector3 operator+(const Vector3 &obj1, const Vector3 &obj2) {
	Vector3 tmp = {};
	tmp.x = obj1.x + obj2.x;
	tmp.y = obj1.y + obj2.y;
	tmp.z = obj1.z + obj2.z;
	return tmp;
}


//減算
Vector3 operator-(const Vector3 &obj1, const Vector3 &obj2) {
	Vector3 tmp = {};
	tmp.x = obj1.x - obj2.x;
	tmp.y = obj1.y - obj2.y;
	tmp.z = obj1.z - obj2.z;
	return tmp;
}

//スカラー

//乗算
Vector3 operator*(float a, const Vector3 &obj) {
	Vector3 tmp = {};

	tmp.x = obj.x * a;
	tmp.y = obj.y * a;
	tmp.z = obj.z * a;
	return tmp;
}

Vector3 operator*(const Vector3 &obj, float a) {
	Vector3 tmp = {};

	tmp.x = obj.x * a;
	tmp.y = obj.y * a;
	tmp.z = obj.z * a;
	return tmp;
}

//除算
Vector3 operator/(const Vector3 &obj, float a) {

	if (a == 0) {
		return {};
	}
	Vector3 tmp = {};

	tmp.x = obj.x / a;
	tmp.y = obj.y / a;
	tmp.z = obj.z / a;
	return tmp;
}

Vector3 operator+(const Vector3 &obj) {
	return obj;
}

Vector3 operator-(const Vector3 &obj) {
	return { -obj.x,-obj.y,-obj.z };
}