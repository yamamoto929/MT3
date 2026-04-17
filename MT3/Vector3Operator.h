#pragma once
#include "Vector3.h"
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
Vector3 operator*=(Vector3& lhs, const Vector3& rhs);

Vector3 operator*(const Vector3& lhs, const Vector3& rhs);

Vector3 operator*(const Vector3& lhs, const float& rhs);

//============================================
// わりざん
//============================================
Vector3 operator/=(Vector3& lhs, const Vector3& rhs);

Vector3 operator/(const Vector3& lhs, const Vector3& rhs);

Vector3 operator/(const Vector3& lhs, const float& rhs);

