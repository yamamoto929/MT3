#include "Functions.h"
#define NOMINMAX
#include <algorithm> 
#include <Novice.h>
#include <numbers>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include "AABB.h"
#include "Matrix4x4.h"

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

bool IsCollision(const OBB& obb1, const OBB& obb2) {

	Vector3 obbAxis[15];
	// 面法線の判定
	obbAxis[0] = obb1.orientations[0];
	obbAxis[1] = obb1.orientations[1];
	obbAxis[2] = obb1.orientations[2];
	obbAxis[3] = obb2.orientations[0];
	obbAxis[4] = obb2.orientations[1];
	obbAxis[5] = obb2.orientations[2];
	// 各辺の組み合わせのクロス積
	obbAxis[6] = Cross(obb1.orientations[0], obb2.orientations[0]);
	obbAxis[7] = Cross(obb1.orientations[0], obb2.orientations[1]);
	obbAxis[8] = Cross(obb1.orientations[0], obb2.orientations[2]);
	obbAxis[9] = Cross(obb1.orientations[1], obb2.orientations[0]);
	obbAxis[10] = Cross(obb1.orientations[1], obb2.orientations[1]);
	obbAxis[11] = Cross(obb1.orientations[1], obb2.orientations[2]);
	obbAxis[12] = Cross(obb1.orientations[2], obb2.orientations[0]);
	obbAxis[13] = Cross(obb1.orientations[2], obb2.orientations[1]);
	obbAxis[14] = Cross(obb1.orientations[2], obb2.orientations[2]);

	Vector3 ex1 = obb1.orientations[0] * obb1.size.x;
	Vector3 ey1 = obb1.orientations[1] * obb1.size.y;
	Vector3 ez1 = obb1.orientations[2] * obb1.size.z;

	Vector3 vertices1[8];
	vertices1[0] = obb1.center - ex1 - ey1 - ez1;
	vertices1[1] = obb1.center + ex1 - ey1 - ez1;
	vertices1[2] = obb1.center - ex1 + ey1 - ez1;
	vertices1[3] = obb1.center + ex1 + ey1 - ez1;

	vertices1[4] = obb1.center - ex1 - ey1 + ez1;
	vertices1[5] = obb1.center + ex1 - ey1 + ez1;
	vertices1[6] = obb1.center - ex1 + ey1 + ez1;
	vertices1[7] = obb1.center + ex1 + ey1 + ez1;

	Vector3 ex2 = obb2.orientations[0] * obb2.size.x;
	Vector3 ey2 = obb2.orientations[1] * obb2.size.y;
	Vector3 ez2 = obb2.orientations[2] * obb2.size.z;

	Vector3 vertices2[8];
	vertices2[0] = obb2.center - ex2 - ey2 - ez2;
	vertices2[1] = obb2.center + ex2 - ey2 - ez2;
	vertices2[2] = obb2.center - ex2 + ey2 - ez2;
	vertices2[3] = obb2.center + ex2 + ey2 - ez2;

	vertices2[4] = obb2.center - ex2 - ey2 + ez2;
	vertices2[5] = obb2.center + ex2 - ey2 + ez2;
	vertices2[6] = obb2.center - ex2 + ey2 + ez2;
	vertices2[7] = obb2.center + ex2 + ey2 + ez2;

	for (int i = 0; i < 15; i++) {
		float min1;
		float min2;
		float max1;
		float max2;

		float dotVertices1[8];
		float dotVertices2[8];
		for (int j = 0; j < 8; j++) {
			dotVertices1[j] = Dot(vertices1[j], obbAxis[i]);
			dotVertices2[j] = Dot(vertices2[j], obbAxis[i]);
		}
		min1 = dotVertices1[0];
		min2 = dotVertices2[0];
		max1 = dotVertices1[0];
		max2 = dotVertices2[0];
		for (int j = 0; j < 7; j++) {
			min1 = (std::min)(min1, dotVertices1[j + 1]);
			max1 = (std::max)(max1, dotVertices1[j + 1]);
			min2 = (std::min)(min2, dotVertices2[j + 1]);
			max2 = (std::max)(max2, dotVertices2[j + 1]);
		}

		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);

		if (sumSpan < longSpan) {
			return false;
		}
	}

	return true;
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

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 extX = { obb.size.x * obb.orientations[0].x,obb.size.y * obb.orientations[0].y, obb.size.z * obb.orientations[0].z, };
	Vector3 extY = { obb.size.x * obb.orientations[1].x,obb.size.y * obb.orientations[1].y, obb.size.z * obb.orientations[1].z, };
	Vector3 extZ = { obb.size.x * obb.orientations[2].x,obb.size.y * obb.orientations[2].y, obb.size.z * obb.orientations[2].z, };

	Vector3 vertices[8];
	vertices[0] = obb.center - extX - extY - extZ;
	vertices[1] = obb.center + extX - extY - extZ;
	vertices[2] = obb.center - extX - extY + extZ;
	vertices[3] = obb.center + extX - extY + extZ;
	vertices[4] = obb.center - extX + extY - extZ;
	vertices[5] = obb.center + extX + extY - extZ;
	vertices[6] = obb.center - extX + extY + extZ;
	vertices[7] = obb.center + extX + extY + extZ;

	Vector3 screenVertices[8];
	for (int i = 0; i < 8; ++i) {
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

Vector3 MultiplyVec3Mat4x4(const Vector3& v, const Matrix4x4& m) {
	Matrix4x4 vMat{};
	vMat.m[0][0] = v.x;
	vMat.m[1][0] = v.y;
	vMat.m[2][0] = v.z;
	vMat.m[3][0] = 1.0f;

	Matrix4x4 resultMat = Multiply(vMat, m);

	assert(resultMat.m[3][0] != 0.0f);
	Vector3 resultVec3 = { resultMat.m[0][0] / resultMat.m[3][0],
							resultMat.m[1][0] / resultMat.m[3][0],
							resultMat.m[2][0] / resultMat.m[3][0], };
	return resultVec3;
}

void MakeOBBOrientation(const Vector3& rotate, OBB& obb) {
	Matrix4x4 rotationMatrix = Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));

	obb.orientations[0].x = rotationMatrix.m[0][0];
	obb.orientations[0].y = rotationMatrix.m[0][1];
	obb.orientations[0].z = rotationMatrix.m[0][2];
	obb.orientations[1].x = rotationMatrix.m[1][0];
	obb.orientations[1].y = rotationMatrix.m[1][1];
	obb.orientations[1].z = rotationMatrix.m[1][2];
	obb.orientations[2].x = rotationMatrix.m[2][0];
	obb.orientations[2].y = rotationMatrix.m[2][1];
	obb.orientations[2].z = rotationMatrix.m[2][2];
}