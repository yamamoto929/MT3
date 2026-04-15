#include "Vector3.h"

//============================================
// たしざん
//============================================
Vector3 operator+=(Vector3& lhs, const Vector3& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
};

Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
	Vector3 result = lhs;
	result += rhs;
	return result;
};

//============================================
// ひきざん
//============================================
Vector3 operator-=(Vector3& lhs, const Vector3& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
};

Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
	Vector3 result = lhs;
	result -= rhs;
	return lhs;
};

//============================================
// かけざん
//============================================
Vector3 operator*=(Vector3& lhs, const Vector3& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	return lhs;

};

Vector3 operator*(const Vector3& lhs, const Vector3& rhs) {
	Vector3 result = lhs;
	result *= rhs;
	return result;
};

Vector3 operator*(const Vector3& lhs, const float& rhs) {
	Vector3 result = lhs;
	result.x *= rhs;
	result.y *= rhs;
	result.z *= rhs;
	return result;
};

//============================================
// わりざん
//============================================
Vector3 operator/=(Vector3& lhs, const Vector3& rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	return lhs;

};

Vector3 operator/(const Vector3& lhs, const Vector3& rhs) {
	Vector3 result = lhs;
	result /= rhs;
	return result;
};

Vector3 operator/(const Vector3& lhs, const float& rhs) {
	Vector3 result = lhs;
	result.x /= rhs;
	result.y /= rhs;
	result.z /= rhs;
	return result;

};