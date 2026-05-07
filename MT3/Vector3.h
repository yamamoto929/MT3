#pragma once
struct Vector3 {
	float x;
	float y;
	float z;
};
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
float Dot(const Vector3& v1, const Vector3& v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);

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

Vector3 operator*(const float& s, const Vector3& v);

//============================================
// わりざん
//============================================
Vector3 operator/(const Vector3& v, const float& s);

Vector3 operator/(const float& s, const Vector3& v);