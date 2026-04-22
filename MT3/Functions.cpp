#include "Functions.h"
#include <cmath>
#include <algorithm> // std::clamp用

// ベクトル(vector)をターゲット(target)に投影したベクトルを返す
Vector3 Project(const Vector3& v1, const Vector3& v2) {
    // 内積 (Dot Product)
    float dotProduct =
        v1.x * v2.x +
        v1.y * v2.y +
        v1.z * v2.z;

    // targetの長さの2乗 (Length Squared)
    float lengthSquared =
        v2.x * v2.x +
        v2.y * v2.y +
        v2.z * v2.z;

    // ゼロ除算の回避
    if (lengthSquared == 0.0f) {
        return Vector3{ 0.0f, 0.0f, 0.0f }; // Vector3のコンストラクタに合わせて適宜変更してください
    }

    // 投影ベクトルの計算
    float scale = dotProduct / lengthSquared;
    return v2 * scale;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
    // 始点から点へのベクトル
    Vector3 originToPoint = point - segment.origin;

    // 内積
    float dotProduct =
        originToPoint.x * segment.diff.x +
        originToPoint.y * segment.diff.y +
        originToPoint.z * segment.diff.z;

    // 線分方向ベクトル(diff)の長さの2乗
    float lengthSquared =
        segment.diff.x * segment.diff.x +
        segment.diff.y * segment.diff.y +
        segment.diff.z * segment.diff.z;

    // ゼロ除算の回避
    if (lengthSquared == 0.0f) {
        return segment.origin;
    }

    // パラメータ t を計算
    float t = dotProduct / lengthSquared;

    // 線分(Segment)の範囲内に収めるため、tを 0.0 ～ 1.0 に制限 (クランプ)
    t = std::clamp(t, 0.0f, 1.0f);

    return segment.origin + t * segment.diff;
}