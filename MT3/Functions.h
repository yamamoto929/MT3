#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <Novice.h>
static const inline int kColumnWidth = 60;
static const inline int kRowHeight = 20;

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);