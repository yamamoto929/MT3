#include "Projection.h"
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return Dot(v1, v2) / Length(v2) * v2;
}