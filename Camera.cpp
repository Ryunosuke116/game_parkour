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
	spherePosition = lookPosition;
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

	centerPos = playerPosition;
	centerPos.y += 14.0f;

	aimPosition.y = spherePosition.y + 20.0f;

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

	DrawSphere3D(spherePosition, radius, 30, GetColor(0, 0, 0),
		    GetColor(255, 0, 0), FALSE);

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

	aimPosition.x = spherePosition.x + 60 * cos(angle);
	aimPosition.z = spherePosition.z + 60 * sin(angle);

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	PosCalc();

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	cameraDirection = VSub(spherePosition, aimPosition);
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

void Camera::LeapCalc_single(float& changePos, const float targetPos, const float speed)
{
	float sub = targetPos - changePos;
	float scale = sub * speed;

	changePos = changePos + scale;
}

void Camera::PosCalc()
{
	//lookPosが球の外側にいった場合球の中心座標をずらす
	if (!HitCheck::HitConfirmation(spherePosition, centerPos, radius, 0.5f))
	{
		Leap(spherePosition, centerPos, 0.2f);
	}
	else
	{
		Leap(spherePosition, centerPos, 0.05f);
	}
	
}