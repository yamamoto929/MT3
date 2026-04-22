#include "Functions.h"
#include <algorithm> 

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	// 内積 
	float dotProduct =
		v1.x * v2.x +
		v1.y * v2.y +
		v1.z * v2.z;

	// targetの長さの2乗 
	float lengthSquared =
		v2.x * v2.x +
		v2.y * v2.y +
		v2.z * v2.z;

	// ゼロ除算の回避
	if (lengthSquared == 0.0f) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	// 投影ベクトルの計算
	float scale = dotProduct / lengthSquared;
	return v2 * scale;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	// 始点から点へのベクトル
	Vector3 originToPoint = point - segment.origin;

	// 内積
	float dotProduct =
		originToPoint.x * segment.diff.x +
		originToPoint.y * segment.diff.y +
		originToPoint.z * segment.diff.z;

	// 線分方向ベクトル(diff)の長さの2乗
	float lengthSquared =
		segment.diff.x * segment.diff.x +
		segment.diff.y * segment.diff.y +
		segment.diff.z * segment.diff.z;

	// ゼロ除算の回避
	if (lengthSquared == 0.0f) {
		return segment.origin;
	}

	// t を計算
	float t = dotProduct / lengthSquared;

	// tを 0.0 ～ 1.0 に制限
	t = std::clamp(t, 0.0f, 1.0f);

	return segment.origin + t * segment.diff;
}