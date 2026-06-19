#include <Novice.h>
#include <imgui.h>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Functions.h"
#include <cmath>

const char kWindowTitle[] = "LE2B_30_ヤマモト_ルナ_MT3_03_00";

void MoveCamera(char keys[], Vector3& rotate, Vector3& translate);
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 controlPoints[3] = {
		{-0.8f, 0.58f, 1.0f },
		{1.76f, 1.0f, -0.3f},
		{0.94f, -0.7f, 2.3f },
	};

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		MoveCamera(keys, cameraRotate, cameraTranslate);

		ImGui::Begin("Debug");
		ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x,0.01f);
		ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x,0.01f);
		ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x,0.01f);
		ImGui::End();

		Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		Sphere sphere[3];
		for (int index = 0; index < 3; ++index) {
			sphere[index].center = controlPoints[index];
			sphere[index].radius = 0.01f;
			DrawSphere(sphere[index], viewProjectionMatrix, viewportMatrix, 0x000000FF);
		}
		DrawBezier(sphere[0].center, sphere[1].center, sphere[2].center, viewProjectionMatrix, viewportMatrix, 0x0000FFFF, 1024);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

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
