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
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);


Matrix4x4 MakePerspectiveFovMatrix(float fovy, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
float Cot(float a);