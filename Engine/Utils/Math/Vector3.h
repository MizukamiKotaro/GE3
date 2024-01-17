#pragma once
struct Vector3
{
public:

	bool operator==(const Vector3 &obj) const {
		if (x == obj.x && y == obj.y && z == obj.z) {
			return true;
		}
		return false;
	}

	//変換
	Vector3 &operator=(const Vector3 &obj) {
		x = obj.x;
		y = obj.y;
		z = obj.z;
		return *this;
	}

	//加算
	void operator+=(const Vector3 &obj) {
		this->x = this->x + obj.x;
		this->y = this->y + obj.y;
		this->z = this->z + obj.z;
	}

	//減算
	void  operator-=(const Vector3 &obj) {
		this->x -= obj.x;
		this->y -= obj.y;
		this->z -= obj.z;
	}

	//スカラー

	//乗算
	void operator*=(float a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
	}

	//除算
	void operator/=(float a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
	}

	float Length() const;

	float Length(const Vector3 &pos)const;

	Vector3 Normalize() const;

	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 front;

	Vector3 Reflect(const Vector3 &normal, const float coefficient = 1.f) const;

public:
	float x;
	float y;
	float z;
};

Vector3 operator+(const Vector3 &obj1, const Vector3 &obj2);

//減算
Vector3 operator-(const Vector3 &obj1, const Vector3 &obj2);

//スカラー
//乗算
Vector3 operator*(float a, const Vector3 &obj);

Vector3 operator*(const Vector3 &obj, float a);

//除算
Vector3 operator/(const Vector3 &obj, float a);

Vector3 operator+(const Vector3 &obj);

Vector3 operator-(const Vector3 &obj);