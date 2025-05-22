#include "Include.h"

/// <summary>
/// インストラクタ
/// </summary>
Camera::Camera()
{
	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(3.5f, 5000.0f);
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// 初期化
/// </summary>
void Camera::Initialize()
{
	aimPosition = VGet(30.0f, 15, -10);
	lookPosition = VGet(0.0f, 0, 20.0f);
	a = -177.55f;

	SetCameraPositionAndTarget_UpVecY(aimPosition, lookPosition);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const VECTOR& playerPosition)
{
	lookPosition = playerPosition;
	aimPosition.y = playerPosition.y + 20.0f;

	//カメラ移動処理
	if (CheckHitKey(KEY_INPUT_A))
	{
		a += 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		a -= 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		aimPosition.y += 0.5f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		aimPosition.y -= 0.5f;
	}

	RotateUpdate(playerPosition);
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{
	printfDx("lookPosition.x %f\n", lookPosition.x);
	printfDx("lookPosition.y %f\n", lookPosition.y);
	printfDx("lookPosition.z %f\n", lookPosition.z);
	printfDx("aimPosition.x %f\n", aimPosition.x);
	printfDx("aimPosition.y %f\n", aimPosition.y);
	printfDx("aimPosition.z %f\n", aimPosition.z);
}

void Camera::RotateUpdate(const VECTOR& playerPosition)
{
	float angle = a * DX_PI_F / 360.0f;
	this->angle = angle;

	//aimPosition = VAdd(aimPosition, addaimPosition);
	aimPosition.x = lookPosition.x + 60 * cos(angle);
	aimPosition.z = lookPosition.z + 60 * sin(angle);

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//中心からの距離を測る
	float r = VSize(VSub(playerPosition, lookPosition));


	//一定の距離に達したらそれ以上いけないようにする
	if (r >= maxRange_ || r <= -maxRange_)
	{
		Leap(lookPosition, playerPosition, cameraSpeed);
	}
	else if (r >= maxRange || r <= -maxRange)
	{
		Leap(lookPosition, playerPosition, cameraSpeed_);
	}

	SetCameraPositionAndTarget_UpVecY(aimPosition, lookPosition);

	cameraDirection = VSub(lookPosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);
}

/// <summary>
/// ラープ
/// </summary>
/// <param name="AimPosition"></param>
void Camera::Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed)
{
	VECTOR SubPosition = VSub(playerPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);
	changePosition = VAdd(changePosition, scalePosition);
}