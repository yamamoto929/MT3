#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cassert>
#include <Novice.h>

static const inline int kRowHeight = 20;
static const inline int kColumnWidth = 60;

// 1. 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//2.拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 3. 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
