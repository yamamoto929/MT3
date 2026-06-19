#include "Vector3.h"
#include <cmath>

float Length(const Vector3& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	if (length == 0.0f) { return v; }
	return Vector3{ v.x / length,v.y / length, v.z / length, };
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x
		+ v1.y * v2.y
		+ v1.z * v2.z;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x=v1.x + v2.x;
	result.y=v1.y + v2.y;
	result.z=v1.z + v2.z;
	return result;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Multiply(const float& s, const Vector3& v) {
	Vector3 result{};
	result.x = s * v.x;
	result.y = s * v.y;
	result.z = s * v.z;
	return result;
}

