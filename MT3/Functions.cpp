#include "Functions.h"
// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovy, float aspectRatio, float nearClip, float farClip) {
	
	Matrix4x4 result = { {
		{(1.0f / aspectRatio)*Cot(fovy/2.0f),0.0f,            0.0f,                                    0.0f},
		{0.0f,                               Cot(fovy / 2.0f),0.0f,                                    0.0f},
		{0.0f,                               0.0f,            farClip/(farClip-nearClip),              1.0f},
		{0.0f,                               0.0f,            (-nearClip * farClip)/(farClip-nearClip),0.0f}
	} };

	return result;
}
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = { {
		{2.0f / (right - left),0.0f,0.0f,0.0f},
		{0.0f,2.0f / (top - bottom),0.0f,0.0f},
		{0.0f,0.0f,1.0f / (farClip - nearClip),0.0f},
		{ (left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (farClip - nearClip),1.0f}
	} };

	return result;
}
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = { {
		{width/2.0f,         0.0f,           0.0f,             0.0f},
		{0.0f,               -height/2.0f,   0.0f,             0.0f},
		{0.0f,               0.0f,           maxDepth-minDepth,0.0f},
		{left + width / 2.0f,top+height/2.0f,minDepth,         1.0f}
	} };

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
	return 1.0f / std::tanf(a);
}

