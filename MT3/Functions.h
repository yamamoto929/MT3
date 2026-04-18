#pragma once
#include "Matrix4x4.h"
#include "Matrix4x4Function.h"
#include "Vector3.h"
#include "Vector3Operator.h"
#include <cmath>
#include <Novice.h>
static const inline int kColumnWidth = 60;
static const inline int kRowHeight = 20;

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovy, float aspectRatio, float nearClip, float farClip);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

float Cot(float a);