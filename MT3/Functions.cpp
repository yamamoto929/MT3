#include "Functions.h"
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

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(segment.origin + segment.diff, viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertices[3];
	vertices[0] = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	vertices[1] = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	vertices[2] = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);
	Novice::DrawTriangle(int(vertices[0].x), int(vertices[0].y),
		int(vertices[1].x), int(vertices[1].y),
		int(vertices[2].x), int(vertices[2].y),
		color,
		kFillModeWireFrame);
}

bool IsCollision(const Segment& segment, const Triangle& triangle) {

	Vector3 v1 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v2 = triangle.vertices[2] - triangle.vertices[0];

	Vector3 n = Normalize(Cross(v1, v2));

	float d = Dot(triangle.vertices[0], n);

	Vector3 b = segment.diff;

	float dot = Dot(b, n);

	if (std::fabs(dot) < 0.0001f) {
		return false;
	}

	float t = (d - Dot(segment.origin, n)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 p = segment.origin + b * t;

	Vector3 cross01 = Cross(
		triangle.vertices[1] - triangle.vertices[0],
		p - triangle.vertices[0]);

	Vector3 cross12 = Cross(
		triangle.vertices[2] - triangle.vertices[1],
		p - triangle.vertices[1]);

	Vector3 cross20 = Cross(
		triangle.vertices[0] - triangle.vertices[2],
		p - triangle.vertices[2]);

	if (Dot(cross01, n) >= 0.0f &&
		Dot(cross12, n) >= 0.0f &&
		Dot(cross20, n) >= 0.0f) {
		return true;
	}

	return false;
}// ===================================================================
// 直線
// ===================================================================
bool IsCollision(const Line& line, const Triangle& triangle) {

	line;
	triangle;
	return true;
}
// ===================================================================
// 半直線
// ===================================================================
bool IsCollision(const Ray& ray, const Triangle& triangle) {
	ray;
	triangle;
	return false;
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

