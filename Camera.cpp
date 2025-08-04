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
	distance(0.0f),
	aimPosition_usual(VGet(0.0f, 0.0f, 0.0f))
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
	spherePosition = VGet(0.0f, 20.0f, 20.0f);
	angle_now = -177.55f;
	distance = 50.0f;
	t = 0;

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const VECTOR& playerPosition,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
	centerPos = playerPosition;
	centerPos.y += 15.0f;

	aimPosition_usual.y = spherePosition.y + 15.0f;
	min = spherePosition.y;
	max = spherePosition.y + 40.0f;
	float min_distance = 15.0f;
	float max_distance = 60.0f;

	float easedT = Calculation::EaseOutExpo(t);

	aimPosition_usual.y = min + (max - min) * easedT;
	distance = min_distance + (max_distance - min_distance) * easedT;

	//カメラ移動処理
	if (CheckHitKey(KEY_INPUT_A) ||
		PadInput::GetJoyPad_x_left() < 0.0f ||
		PadInput::GetJoyPad_x_right() < 0.0f)
	{
		angle_now += 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_D) ||
		PadInput::GetJoyPad_x_left() > 0.0f ||
		PadInput::GetJoyPad_x_right() > 0.0f)
	{
		angle_now -= 2.0f;
	}


	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		t -= 0.01f;
		if (t <= 0.0f)
		{
			t = 0.0f;
		}
		
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		t += 0.01f;
		if (t >= 1.0f)
		{
			t = 1.0f;
		}
	
	}

	RotateUpdate();

	for (const auto& fieldObject : fieldObjects)
	{
		CameraPosCalc(fieldObject->GetModelHandle());
	}

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	cameraDirection = VSub(spherePosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);

	DebugDrawer::Instance().InformationInput_string_VECTOR("aimPos x.%f y.%f z.%f\n", aimPosition);
	DebugDrawer::Instance().InformationInput_string_VECTOR("spherePos x.%f y.%f z.%f\n", spherePosition);
}

/// <summary>
/// layout用カメラ
/// </summary>
void Camera::Update_layout()
{
	//y軸↑移動
	if (CheckHitKey(KEY_INPUT_UP) && CheckHitKey(KEY_INPUT_LCONTROL))
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
	if (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_LCONTROL))
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

	//右回転
	if (CheckHitKey(KEY_INPUT_RIGHT) &&
		CheckHitKey(KEY_INPUT_LSHIFT))
	{
		angle_now += 2.0f;
	}
	//x軸右移動
	else if (CheckHitKey(KEY_INPUT_RIGHT) ||
		PadInput::GetJoyPad_x_right() < 0.0f)
	{
		aimPosition.x += 1.0f;
		spherePosition.x += 1.0f;
	}

	//左回転
	if (CheckHitKey(KEY_INPUT_LEFT) &&
		CheckHitKey(KEY_INPUT_LSHIFT))
	{
		angle_now -= 2.0f;
	}
	//x軸左移動
	else if (CheckHitKey(KEY_INPUT_LEFT) ||
		PadInput::GetJoyPad_x_right() > 0.0f)
	{
		aimPosition.x -= 1.0f;
		spherePosition.x -= 1.0f;
	}

	if (CheckHitKey(KEY_INPUT_9))
	{
		angle_now = -177.55f;
	}

	//RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);
}

/// <summary>
/// 描画
/// </summary>
bool Camera::Draw()
{

	/*DrawSphere3D(spherePosition, radius, 30, GetColor(0, 0, 0),
		    GetColor(255, 0, 0), FALSE);*/

	//printfDx("lookPosition.x %f\n", lookPosition.x);
	//printfDx("lookPosition.y %f\n", lookPosition.y);
	//printfDx("lookPosition.z %f\n", lookPosition.z);
	//printfDx("aimPosition.x %f\n", aimPosition.x);
	//printfDx("aimPosition.y %f\n", aimPosition.y);
	//printfDx("aimPosition.z %f\n", aimPosition.z);
	return true;
}

void Camera::RotateUpdate()
{
	float angle_radian = angle_now * DX_PI_F / 360.0f;
	this->angle_radian = angle_radian;

	aimPosition_usual.x = spherePosition.x + distance * cos(angle_radian);
	aimPosition_usual.z = spherePosition.z + distance * sin(angle_radian);

	aimPosition = aimPosition_usual;

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//注視する座標からplayerがずれたら修正する
	PosCalc();
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
	if (HitCheck::HitRayJudge(mapHandle, -1, spherePosition, aimPosition, hitPoly))
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