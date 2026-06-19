#pragma once
struct Vector3 {
	float x;
	float y;
	float z;

	// 複合代入演算子
	Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
};
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
float Dot(const Vector3& v1, const Vector3& v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(const float& s, const Vector3& v);


//=============================================
// 二項演算子
//=============================================
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
inline Vector3 operator*(const float& s, const Vector3& v) { return Multiply(s, v); }
inline Vector3 operator*(const Vector3& v, const float& s) { return s * v; }
inline Vector3 operator/(const Vector3& v, const float& s) { return Multiply(1.0f / s, v); }

//=============================================
// 単項演算子
//=============================================
inline Vector3 operator-(const Vector3& v) { return { -v.x,-v.y,-v.z }; }
inline Vector3 operator+(const Vector3& v) { return v; }

