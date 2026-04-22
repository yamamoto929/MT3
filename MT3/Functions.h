#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Sphere.h"
#include "Line.h"
#include <cstdint>
const int kColumnWidth = 20;
const int kRowHeight = 60;

Vector3 Project(const Vector3& v1, const Vector3& v2);
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
