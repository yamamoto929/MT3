#include "Functions.h"
#define NOMINMAX
#include <algorithm> 
#include <Novice.h>
#include <numbers>
#include <cmath>
#include <cassert>
#include "Matrix4x4.h"
#include "Line.h"

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovy, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 result = { {
		{(1.0f / aspectRatio) * Cot(fovy / 2.0f),0.0f,            0.0f,                                        0.0f},
		{0.0f,                                   Cot(fovy / 2.0f),0.0f,                                        0.0f},
		{0.0f,                                   0.0f,            farClip / (farClip - nearClip),              1.0f},
		{0.0f,                                   0.0f,            (-nearClip * farClip) / (farClip - nearClip),0.0f}
	} };

	return result;
}
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = { {
		{2.0f / (right - left),           0.0f,                           0.0f,                           0.0f},
		{0.0f,                            2.0f / (top - bottom),          0.0f,                           0.0f},
		{0.0f,                            0.0f,                           1.0f / (farClip - nearClip),    0.0f},
		{ (left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1.0f}
	} };

	return result;
}
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = { {
		{width / 2.0f,       0.0f,               0.0f,               0.0f},
		{0.0f,               -height / 2.0f,     0.0f,               0.0f},
		{0.0f,               0.0f,               maxDepth - minDepth,0.0f},
		{left + width / 2.0f,top + height / 2.0f,minDepth,           1.0f}
	} };

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = { {
		{1.0f, 0.0f,               0.0f,              0.0f},
		{0.0f, std::cos(radian),  std::sin(radian), 0.0f},
		{0.0f, -std::sin(radian), std::cos(radian), 0.0f},
		{0.0f, 0.0f,               0.0f,              1.0f}
	} };
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = { {
		{std::cos(radian), 0.0f, -std::sin(radian), 0.0f},
		{0.0f,              1.0f, 0.0f,               0.0f},
		{std::sin(radian), 0.0f, std::cos(radian),  0.0f},
		{0.0f,              0.0f, 0.0f,               1.0f}
	} };
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = { {
		{std::cos(radian),  std::sin(radian), 0.0f, 0.0f},
		{-std::sin(radian), std::cos(radian), 0.0f, 0.0f},
		{0.0f,               0.0f,              1.0f, 0.0f},
		{0.0f,               0.0f,              0.0f, 1.0f}
	} };
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result{};
	//============================================================
	// 拡縮
	//============================================================
	Matrix4x4 scaleMatrix4x4 = { {
		{scale.x, 0.0f,    0.0f,    0.0f},
		{0.0f,    scale.y, 0.0f,    0.0f},
		{0.0f,    0.0f,    scale.z, 0.0f},
		{0.0f,    0.0f,    0.0f,    1.0f}
	} };

	//============================================================
	// 回転
	//============================================================
	Matrix4x4 rotateMatrix4x4{};
	rotateMatrix4x4 = Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));

	//============================================================
	// 移動
	//============================================================
	Matrix4x4 translateMatrix4x4 = { {
		{1.0f,        0.0f,        0.0f,        0.0f},
		{0.0f,        1.0f,        0.0f,        0.0f},
		{0.0f,        0.0f,        1.0f,        0.0f},
		{translate.x, translate.y, translate.z, 1.0f}
	} };

	//============================================================
	// W=SRT
	//============================================================
	result = Multiply(Multiply(scaleMatrix4x4, rotateMatrix4x4), translateMatrix4x4);
	return result;
}

bool IsCollision(const AABB& aabb, const Segment& segment) {
	
	Vector3 min = {
		(aabb.min.x - segment.origin.x) /segment.diff.x,
		(aabb.min.y - segment.origin.y) / segment.diff.y,
		(aabb.min.z - segment.origin.z) / segment.diff.z,
	};

	Vector3 max = {
		(aabb.max.x - segment.origin.x) / segment.diff.x,
		(aabb.max.y - segment.origin.y) / segment.diff.y,
		(aabb.max.z - segment.origin.z) / segment.diff.z
	};

	Vector3 tNear;
	tNear.x = std::min(min.x,max.x);
	tNear.y = std::min(min.y, max.y);
	tNear.z = std::min(min.z, max.z);
	

	Vector3 tFar;
	tFar.x = std::max(min.x, max.x);
	tFar.y = std::max(min.y, max.y);
	tFar.z = std::max(min.z, max.z);

	float tmin = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tmax = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tmin <= tmax) {
		return true;
	}

	return false;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	// Gridの半分の幅
	const uint32_t kSubdivision = 10;
	// 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);
	// 1つ分の長さ

	// 奥から手前への線を順々に引いていく（Z軸に平行な線）
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// 現在のX座標を求める
		float x = -kGridHalfWidth + (xIndex * kGridEvery);

		// ワールド座標系上の始点と終点
		Vector3 start{ x, 0.0f, kGridHalfWidth };
		Vector3 end{ x, 0.0f, -kGridHalfWidth };

		// スクリーン座標系まで変換をかける (World -> NDC -> Screen)
		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		// 原点を通る軸（X=0）だけ色を濃く（黒に）する
		unsigned int color = (x == 0.0f) ? 0x222222FF : 0xAAAAAAFF;

		Novice::DrawLine(
			int(startScreen.x), int(startScreen.y),
			int(endScreen.x), int(endScreen.y),
			color
		);
	}

	// 左から右へ線を順々に引いていく（X軸に平行な線）
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// 現在のZ座標を求める
		float z = -kGridHalfWidth + (zIndex * kGridEvery);

		// ワールド座標系上の始点と終点
		Vector3 start{ -kGridHalfWidth, 0.0f, z };
		Vector3 end{ kGridHalfWidth, 0.0f, z };

		// スクリーン座標系まで変換をかける (World -> NDC -> Screen)
		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		// 原点を通る軸（Z=0）だけ色を濃く（黒に）する
		unsigned int color = (z == 0.0f) ? 0x222222FF : 0xAAAAAAFF;

		Novice::DrawLine(
			int(startScreen.x), int(startScreen.y),
			int(endScreen.x), int(endScreen.y),
			color
		);
	}
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertices[8];
	vertices[0] = aabb.min;
	vertices[1] = Vector3{ aabb.max.x,aabb.min.y,aabb.min.z };
	vertices[2] = Vector3{ aabb.min.x,aabb.max.y,aabb.min.z };
	vertices[3] = Vector3{ aabb.max.x,aabb.max.y,aabb.min.z };
	vertices[4] = Vector3{ aabb.min.x,aabb.min.y,aabb.max.z };
	vertices[5] = Vector3{ aabb.max.x,aabb.min.y,aabb.max.z };
	vertices[6] = Vector3{ aabb.min.x,aabb.max.y,aabb.max.z };
	vertices[7] = aabb.max;

	Vector3 screenVertices[8];
	for (int i = 0;i < 8;++i) {
		screenVertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), color);
	Novice::DrawLine(int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[3].x), int(screenVertices[3].y), color);
	Novice::DrawLine(int(screenVertices[3].x), int(screenVertices[3].y), int(screenVertices[2].x), int(screenVertices[2].y), color);
	Novice::DrawLine(int(screenVertices[2].x), int(screenVertices[2].y), int(screenVertices[0].x), int(screenVertices[0].y), color);
	Novice::DrawLine(int(screenVertices[4].x), int(screenVertices[4].y), int(screenVertices[5].x), int(screenVertices[5].y), color);
	Novice::DrawLine(int(screenVertices[5].x), int(screenVertices[5].y), int(screenVertices[7].x), int(screenVertices[7].y), color);
	Novice::DrawLine(int(screenVertices[7].x), int(screenVertices[7].y), int(screenVertices[6].x), int(screenVertices[6].y), color);
	Novice::DrawLine(int(screenVertices[6].x), int(screenVertices[6].y), int(screenVertices[4].x), int(screenVertices[4].y), color);

	Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[4].x), int(screenVertices[4].y), color);
	Novice::DrawLine(int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[5].x), int(screenVertices[5].y), color);
	Novice::DrawLine(int(screenVertices[2].x), int(screenVertices[2].y), int(screenVertices[6].x), int(screenVertices[6].y), color);
	Novice::DrawLine(int(screenVertices[3].x), int(screenVertices[3].y), int(screenVertices[7].x), int(screenVertices[7].y), color);

}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(segment.origin + segment.diff, viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}


void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

float Cot(float a) {
	return 1.0f / std::tan(a);
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return Vector3{ -vector.y,vector.x,0.0f };
	}
	return Vector3{ 0.0f,-vector.z,vector.y };
}

