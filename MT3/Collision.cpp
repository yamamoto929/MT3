#include "Collision.h"
#include "Capsule.h"
#include <algorithm>
#include <cmath>
bool IsCollision(const Sphere& sphere, const Plane& plane, const Vector3& velocity) {
	Capsule capsule;
	capsule.radius = sphere.radius;
	capsule.segment.diff = velocity;
	capsule.segment.origin = sphere.center;

	float dotA = Dot(capsule.segment.origin, plane.normal);

	float dotB = Dot(capsule.segment.origin + capsule.segment.diff, plane.normal);

	if (dotA * dotB < 0.0f) {
		return true;
	}

	float distanceA = std::abs(dotA - plane.distance);

	float distanceB = std::abs(dotB - plane.distance);

	float minDistance = std::min(distanceA, distanceB);
	if (minDistance <= sphere.radius) {
		return true;
	}

	return false;
}