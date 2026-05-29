#pragma once
#include "Vector3.h"

struct OBB {
	Vector3 center;
	Vector3 orientations[3]; // 正規化直交必須
	Vector3 size;
};