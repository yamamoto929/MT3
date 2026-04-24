#include "Functions.h"
#include <algorithm> 
#include <Novice.h>
#include <numbers>
#include <cmath>
#include <cassert>
#include "Matrix4x4Function.h"

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

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 10;

	float pi = std::numbers::pi_v<float>;

	const float kLonEvery = (2.0f * pi) / kSubdivision;

	const float kLatEvery = pi / kSubdivision;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {

		float lat = -pi / 2.0f + kLatEvery * latIndex; // 現在の緯度
		float cosLat = std::cos(lat);
		float sinLat = std::sin(lat);
		float cosLatNext = std::cos(lat + kLatEvery);
		float sinLatNext = std::sin(lat + kLatEvery);

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度

			float cosLon = std::cos(lon);
			float sinLon = std::sin(lon);
			float cosLonNext = std::cos(lon + kLonEvery);
			float sinLonNext = std::sin(lon + kLonEvery);

			// world座標系でのa, b, cを求める
			Vector3 a, b, c;
			a = { cosLat * cosLon, sinLat, cosLat * sinLon };
			b = { cosLatNext * cosLon, sinLatNext, cosLatNext * sinLon };
			c = { cosLat * cosLonNext, sinLat, cosLat * sinLonNext };

			a = { a.x * sphere.radius + sphere.center.x, a.y * sphere.radius + sphere.center.y, a.z * sphere.radius + sphere.center.z };
			b = { b.x * sphere.radius + sphere.center.x, b.y * sphere.radius + sphere.center.y, b.z * sphere.radius + sphere.center.z };
			c = { c.x * sphere.radius + sphere.center.x, c.y * sphere.radius + sphere.center.y, c.z * sphere.radius + sphere.center.z };

			// a, b, cをScreen座標系まで変換...
			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);
			// ab, bcで線を引く
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix4x4, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = plane.distance*plane.normal;
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = Vector3{-perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z};
	perpendiculars[2] = Cross(plane.normal,perpendiculars[0]);
	perpendiculars[3] = Vector3{ -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };
}

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	float distance = Length(s1.center - s2.center);
	if (distance <= s1.radius + s2.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

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

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}