#include "Camera.h"
#include <Novice.h>
#include <cmath>
void MoveCamera(char keys[], Vector3& rotate, Vector3& translate) {

	static int mouseX = 0;
	static int mouseY = 0;
	if (Novice::IsPressMouse(1)) {
		int curMouseX;
		int curMouseY;


		Novice::GetMousePosition(&curMouseX, &curMouseY);
		rotate.x += float(mouseY - curMouseY) / 1000.0f;
		rotate.y += float(mouseX - curMouseX) / 1000.0f;
		mouseX = curMouseX;
		mouseY = curMouseY;
	} else {
		Novice::GetMousePosition(&mouseX, &mouseY);
	}

	float speed = 0.1f;

	float yaw = rotate.y;


	Vector3 forward = { std::sin(yaw), 0.0f, std::cos(yaw) };

	Vector3 right = { std::cos(yaw), 0.0f, -std::sin(yaw) };

	if (keys[DIK_W]) {
		translate.x += forward.x * speed;
		translate.z += forward.z * speed;
	} else if (keys[DIK_S]) {
		translate.x -= forward.x * speed;
		translate.z -= forward.z * speed;
	}

	if (keys[DIK_A]) {
		translate.x -= right.x * speed;
		translate.z -= right.z * speed;
	} else if (keys[DIK_D]) {
		translate.x += right.x * speed;
		translate.z += right.z * speed;
	}

	if (keys[DIK_SPACE]) {
		translate.y += speed;
	} else if (keys[DIK_LSHIFT]) {
		translate.y -= speed;
	}
};