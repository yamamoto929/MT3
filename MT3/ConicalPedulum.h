#pragma once
#include "Vector3.h"
struct ConicalPendulum{
	Vector3 anchor;
	float length;
	float halfApexAngle; // 円錐の頂角の半分
	float angle;
	float angularVelocity; // 角速度
};