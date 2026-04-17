#pragma once
struct Vector3 {
	float x;
	float y;
	float z;
};

//============================================
// たしざん
//============================================
Vector3 operator+=(Vector3& lhs, const Vector3& rhs);

Vector3 operator+(const Vector3& lhs, const Vector3& rhs);

//============================================
// ひきざん
//============================================
Vector3 operator-=(Vector3& lhs, const Vector3& rhs);

Vector3 operator-(const Vector3& lhs, const Vector3& rhs);

//============================================
// かけざん
//============================================
Vector3 operator*(const Vector3& v, const float& s);

//============================================
// わりざん
//============================================
Vector3 operator/(const Vector3& v, const float& s);