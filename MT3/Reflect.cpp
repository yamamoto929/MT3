#include "Reflect.h"
Vector3 Reflect(const Vector3& input, const Vector3& normal) {
	Vector3 reflectVector =
		input - 2.0f * Dot(input, normal) * normal;
	return reflectVector;
}