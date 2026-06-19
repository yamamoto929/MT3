#pragma once
#include "Vector3.h"
struct Spring {
	Vector3 anchor; // 固定された端の位置
	float naturalLength; // 自然長
	float stiffness; // 剛性 バネ定数k
	float dampingCoefficient; // 減衰係数
};