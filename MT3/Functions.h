#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "AABB.h"
#include "OBB.h"
#include "Line.h"
#include <cstdint>
const int kColumnWidth = 20;
const int kRowHeight = 60;

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color);
void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MakeOBBOrientation(const Vector3& rotate, OBB& obb);
Matrix4x4 MakePerspectiveFovMatrix(float fovy, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
bool IsCollision(const AABB& aabb, const Segment& segment);
bool IsCollision(const AABB& aabb, const Line& line);
bool IsCollision(const AABB& aabb, const Ray& ray);
bool IsCollision(const OBB& obb, const Segment& segment);
bool IsCollision(const OBB& obb, const Line& line);
bool IsCollision(const OBB& obb, const Ray& ray);

float Cot(float a);
Vector3 Perpendicular(const Vector3& vector);
Vector3 MultiplyVec3Mat4x4(const Vector3& v, const Matrix4x4& m);