#include "Functions.h"
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (size_t ai = 0; ai < 4; ai++) {
		for (size_t bi = 0; bi < 4; bi++) {
			result.m[ai][bi] =
				m1.m[ai][0] * m2.m[0][bi] +
				m1.m[ai][1] * m2.m[1][bi] +
				m1.m[ai][2] * m2.m[2][bi] +
				m1.m[ai][3] * m2.m[3][bi];
		}
	}
	return result;
};

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = { {
		{1.0f, 0.0f,               0.0f,              0.0f},
		{0.0f, std::cosf(radian),  std::sinf(radian), 0.0f},
		{0.0f, -std::sinf(radian), std::cosf(radian), 0.0f},
		{0.0f, 0.0f,               0.0f,              1.0f}
	} };
	return result;
};

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = { {
		{std::cosf(radian), 0.0f, -std::sinf(radian), 0.0f},
		{0.0f,              1.0f, 0.0f,               0.0f},
		{std::sinf(radian), 0.0f, std::cosf(radian),  0.0f},
		{0.0f,              0.0f, 0.0f,               1.0f}
	} };
	return result;
};

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = { {
		{std::cosf(radian),  std::sinf(radian), 0.0f, 0.0f},
		{-std::sinf(radian), std::cosf(radian), 0.0f, 0.0f},
		{0.0f,               0.0f,              1.0f, 0.0f},
		{0.0f,               0.0f,              0.0f, 1.0f}
	} };
	return result;
};

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