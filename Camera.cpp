#include "common.h"

#include "EffekseerForDXLib.h"
#include "HitCheck.h"
#include "PadInput.h"
#include "BaseObject.h"
#include "Camera.h"
#include "Calculation.h"
#include "DebugDrawer.h"


/// <summary>
/// インストラクタ
/// </summary>
Camera::Camera():
	max_t(0.0f),
	min_t(0.0f),
	distance(0.0f),
	angle_now(0.0f),
	angle_new(0.0f),
	angle_radian(0.0f),
	t(0.0f),
	cameraDirection(VGet(0.0f, 0.0f, 0.0f)),
	aimPosition_usual(VGet(0.0f, 0.0f, 0.0f)),
	aimPosition(VGet(0.0f, 0.0f, 0.0f)),
	centerPos(VGet(0.0f, 0.0f, 0.0f)),
	spherePosition(VGet(0.0f, 0.0f, 0.0f)),
	direction(VGet(0.0f, 0.0f, 0.0f)),
	lookPosition(VGet(0.0f, 0.0f, 0.0f))

{
	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(3.5f, 5000.0f);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	
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
	aimPosition = initialize_aimPos;
	spherePosition = initialize_spherePos;
	angle_now = initialize_angle;
	distance = initialize_distance;
	t = initialize_t;

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const VECTOR& playerPosition,
	const float& angle_player,
	const std::vector<std::shared_ptr<BaseObject>>& collisionObjects)
{
	centerPos = playerPosition;
	centerPos.y += 15.0f;

	DistanceUpdate();

	AngleUpdate(angle_player);

	RotateUpdate();

	for (const auto& fieldObject : collisionObjects)
	{
		CameraPosCalc(fieldObject->GetModelHandle());
	}

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	cameraDirection = VSub(spherePosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);

	////DebugDrawer::Instance().InformationInput_string_VECTOR("aimPos x.%f y.%f z.%f\n", aimPosition);
	////DebugDrawer::Instance().InformationInput_string_VECTOR("spherePos x.%f y.%f z.%f\n", spherePosition);
}

void Camera::Update_start(const float& timer,
	const VECTOR& playerPosition,
	const float& angle_player)
{
	centerPos = playerPosition;
	centerPos.y += 15.0f;

	DistanceUpdate();

	AngleUpdate(angle_player);

	RotateUpdate();


	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	cameraDirection = VSub(spherePosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);
}

/// <summary>
/// layout用カメラ
/// </summary>
void Camera::Update_layout()
{
	cameraDirection = VSub(spherePosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);

	VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

	VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

	//正規化
	rightMove = VNorm(rightMove);
	VECTOR upMove = VNorm(cameraDirection);

	upMove.y = 0.0f;
	rightMove.y = 0.0f;

	//パッド or arrowキーの入力方向で計算
	moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
		VScale(upMove, -PadInput::GetJoyPad_y_left()));

	//0でなければ正規化
	if (VSize(moveDirection) != 0)
	{
		moveDirection = VNorm(moveDirection);
	}

	aimPosition = VAdd(aimPosition, moveDirection);
	spherePosition = VAdd(spherePosition, moveDirection);

	//y軸↑移動
	if (CheckHitKey(KEY_INPUT_UP) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		aimPosition.y += 1.0f;
		spherePosition.y += 1.0f;
	}
	//z軸↑移動
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		aimPosition.z += 1.0f;
		spherePosition.z += 1.0f;
	}

	//y軸↓移動
	if (CheckHitKey(KEY_INPUT_DOWN) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		aimPosition.y -= 1.0f;
		spherePosition.y -= 1.0f;
	}
	//z軸↓移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		aimPosition.z -= 1.0f;
		spherePosition.z -= 1.0f;
	}

	////右回転
	//if (CheckHitKey(KEY_INPUT_RIGHT) &&
	//	CheckHitKey(KEY_INPUT_LSHIFT))
	//{
	//	angle_now += 2.0f;
	//}
	////x軸右移動
	//else if (CheckHitKey(KEY_INPUT_RIGHT) ||
	//	PadInput::GetJoyPad_x_right() < 0.0f)
	//{
	//	aimPosition.x += 1.0f;
	//	spherePosition.x += 1.0f;
	//}

	////左回転
	//if (CheckHitKey(KEY_INPUT_LEFT) &&
	//	CheckHitKey(KEY_INPUT_LSHIFT))
	//{
	//	angle_now -= 2.0f;
	//}
	////x軸左移動
	//else if (CheckHitKey(KEY_INPUT_LEFT) ||
	//	PadInput::GetJoyPad_x_right() > 0.0f)
	//{
	//	aimPosition.x -= 1.0f;
	//	spherePosition.x -= 1.0f;
	//}

	if (CheckHitKey(KEY_INPUT_9))
	{
		angle_now = -177.55f;
	}

	//DistanceUpdate();

	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		aimPosition.y += 1.0f;
		spherePosition.y += 1.0f;
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		aimPosition.y -= 1.0f;
		spherePosition.y -= 1.0f;
	}

	AngleUpdate(1.0f);

	float angle_radian = angle_now * DX_PI_F / 360.0f;
	this->angle_radian = angle_radian;

	aimPosition.x = spherePosition.x + distance * cos(angle_radian);
	aimPosition.z = spherePosition.z + distance * sin(angle_radian);

	//aimPosition = aimPosition_usual;

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{

	/*DrawSphere3D(spherePosition, radius, 30, GetColor(0, 0, 0),
		    GetColor(255, 0, 0), FALSE);*/

	//printfDx("lookPosition.x %f\n", lookPosition.x);
	//printfDx("lookPosition.y %f\n", lookPosition.y);
	//printfDx("lookPosition.z %f\n", lookPosition.z);
	//printfDx("aimPosition.x %f\n", aimPosition.x);
	//printfDx("aimPosition.y %f\n", aimPosition.y);
	//printfDx("aimPosition.z %f\n", aimPosition.z);
}

/// <summary>
/// カメラの回転更新
/// </summary>
void Camera::RotateUpdate()
{
	float angle_radian = angle_now * DX_PI_F / 360.0f;
	this->angle_radian = angle_radian;

	////DebugDrawer::Instance().InformationInput_string_float("angle_camera %f\n", angle_radian);

	aimPosition_usual.x = spherePosition.x + distance * cos(angle_radian);
	aimPosition_usual.z = spherePosition.z + distance * sin(angle_radian);

	aimPosition = aimPosition_usual;

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//注視する座標からplayerがずれたら修正する
	PosCalc();
}

/// <summary>
/// カメラの距離更新
/// </summary>
void Camera::DistanceUpdate()
{
	min_t = spherePosition.y;
	max_t = spherePosition.y + 40.0f;
	float min_distance = 15.0f;
	float max_distance = 70.0f;

	float easedT = Calculation::EaseOutQuad(t);

	aimPosition_usual.y = min_t + (max_t - min_t) * easedT;
	distance = min_distance + (max_distance - min_distance) * easedT;

	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		t -= 0.02f;
		if (t <= 0.0f)
		{
			t = 0.0f;
		}
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		t += 0.02f;
		if (t >= 1.0f)
		{
			t = 1.0f;
		}
	}
}

/// <summary>
/// カメラの回転値更新
/// </summary>
void Camera::AngleUpdate(const float& angle_player)
{

	////カメラ移動処理
	//if ((CheckHitKey(KEY_INPUT_A) ||
	//	PadInput::GetJoyPad_x_left() < 0.0f) &&
	//	PadInput::GetJoyPad_x_right() == 0.0f)
	//{
	//	angle_now += 2.0f;
	//}
	//if ((CheckHitKey(KEY_INPUT_D) ||
	//	PadInput::GetJoyPad_x_left() > 0.0f) &&
	//	PadInput::GetJoyPad_x_right() == 0.0f)
	//{
	//	angle_now -= 2.0f;
	//}

	if (PadInput::GetJoyPad_x_right() < 0.0f)
	{
		angle_now += 3.0f;
	}
	else if (PadInput::GetJoyPad_x_right() > 0.0f)
	{
		angle_now -= 3.0f;
	}

	if (PadInput::IsPush_R())
	{
		angle_now = angle_player;
	}

}

/// <summary>
/// カメラの位置調整
/// </summary>
/// <param name="mapHandle"></param>
void Camera::CameraPosCalc(const int& mapHandle)
{
	MV1_COLL_RESULT_POLY hitPoly;
	VECTOR addPos;
	
	//rayが当たっている場合カメラの位置をいじる
	if (HitCheck::RayHitJudge(mapHandle, -1, spherePosition, aimPosition, hitPoly))
	{		
		addPos = VSub(hitPoly.HitPosition, aimPosition_usual);
		direction = VNorm(addPos);
		
		aimPosition = VAdd(aimPosition_usual, addPos);
	}

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



void Camera::PosCalc()
{
	//lookPosが球の外側にいった場合球の中心座標をずらす
	if (!HitCheck::HitConfirmation(spherePosition, centerPos, radius, 0.5f))
	{
		Leap(spherePosition, centerPos, 0.1f);
	}
	else
	{
		Leap(spherePosition, centerPos, 0.05f);
	}
	
}