#pragma once
#include "Vector3.h"

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return Vector3{ -vector.y,vector.x,0.0f };
	}
	return Vector3{ 0.0f,-vector.z,vector.y };
}