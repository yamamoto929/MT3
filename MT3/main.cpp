#include <Novice.h>
#include <cmath>
#include <imgui.h>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Draw.h"
#include "Collision.h"
#include "Plane.h"
#include "Ball.h"
#include "Reflect.h"
#include "Projection.h"
#include "Camera.h"

const int kWindowWidth = 1280;
const int kWindowHeight = 720;
const char kWindowTitle[] = "LE2B_30_ヤマモト_ルナ_MT3_04_04";
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float deltaTime = 1.0f / 60.0f;

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Sphere sphere;
	sphere.radius = 0.05f;
	sphere.center = {};

	Plane plane;
	plane.normal = Normalize({ -0.2f, 0.9f, -0.3f });
	plane.distance = 0.0f;

	Ball ball{};
	ball.position = { 0.7f, 1.5f, 0.2f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;
	Vector3 centerPos = {};
	Vector3 setBallPos = { 0.7f, 1.5f, 0.2f };

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
		ImGui::Begin("Debug");
		ImGui::DragFloat3("initBallPos", &setBallPos.x, 0.01f);
		if (ImGui::Button("Reset Ball")) {
			ball.velocity = {};
			ball.acceleration = {};
			ball.position = setBallPos;
		}
		MoveCamera(keys, cameraRotate, cameraTranslate);
		
		float e = 1.0f;
		ball.acceleration = { 0.0f,-9.8f,0.0f };
		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;
		if (IsCollision(Sphere{ ball.radius,ball.position }, plane,ball.velocity*deltaTime)) {
			Vector3 reflected = Reflect(ball.velocity, plane.normal);
			Vector3 projectToNormal = Project(reflected, plane.normal);
			Vector3 movingDirection = reflected - projectToNormal;
			ball.velocity = projectToNormal * e + movingDirection;
		}

		sphere.center = ball.position;
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
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix,0xFFFFFFFF);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, ball.color);
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


