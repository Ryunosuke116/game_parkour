#include "common.h"
#include "EffekseerForDXLib.h"
#include "HitCheck.h"
#include "PadInput.h"
#include "BaseObject.h"
#include "Camera.h"
#include "Calculation.h"
#include "DebugDrawer.h"
#include "WorldSubSystem.h"
#include "PlayerManager.h"

/// <summary>
/// インストラクタ
/// </summary>
Camera::Camera():
	max_t(0.0f),
	min_t(0.0f),
	distance(0.0f),
	nowDegree(0.0f),
	newDegree(0.0f),
	angleRadian(0.0f),
	t(0.0f),
	isResetAngle(false),
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
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}

void Camera::Create()
{
	const auto self = shared_from_this();
	//サブシステムに追加
	WorldSubSystem::GetInstance().AddSubSystem<Camera>(self);
}

/// <summary>
/// 初期化
/// </summary>
void Camera::Initialize()
{
	aimPosition = initializeAimPos;
	spherePosition = initializeSpherePos;
	nowDegree = initializeAngle;
	distance = initializeDistance;
	t = initializeT;
	isResetAngle = false;

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	centerPos = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPosition();
	centerPos.y += 15.0f;

	DistanceUpdate();

	AngleUpdate(WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetDegree());

	RotateUpdate();

	for (const auto& fieldObject : WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects())
	{
		auto collisionObject = fieldObject.lock();
		CameraPosCalc(collisionObject->GetModelHandle());
	}

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	cameraDirection = VSub(spherePosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);
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
	//	nowDegree += 2.0f;
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
	//	nowDegree -= 2.0f;
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
		nowDegree = -177.55f;
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

	float angleRadian = nowDegree * DX_PI_F / 360.0f;
	this->angleRadian = angleRadian;

	aimPosition.x = spherePosition.x + distance * cos(angleRadian);
	aimPosition.z = spherePosition.z + distance * sin(angleRadian);

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
	float degreesForRdianConversion = nowDegree;
	degreesForRdianConversion -= 90.0f;
	
	if (degreesForRdianConversion >= 180.0f)
	{
		degreesForRdianConversion -= 360.0f;
	}

	this->angleRadian = Calculation::DegToRad(degreesForRdianConversion);

	DebugDrawer::Instance().InformationInput_string_float("nowDegree %f\n", degreesForRdianConversion);

	aimPosition_usual.x = spherePosition.x + distance * -cos(angleRadian);
	aimPosition_usual.z = spherePosition.z + distance * sin(angleRadian);

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
	if (PadInput::GetJoyPad_x_right() < 0.0f)
	{
		nowDegree -= 3.0f;
	}
	else if (PadInput::GetJoyPad_x_right() > 0.0f)
	{
		nowDegree += 3.0f;
	}

	//0°～360°にしかならないように調整
	if (nowDegree >= 360.0f)
	{
		nowDegree -= 360.0f;
	}
	else if (nowDegree <= 0.0f)
	{
		nowDegree += 360.0f;
	}

	ResetAngle(angle_player);
}

void Camera::ResetAngle(const float& angle_player)
{
	const float rotationSpeed = 6.0f;

	if (PadInput::IsPush_R())
	{
		newDegree = angle_player;
		if (newDegree <= 0.0f)
		{
			newDegree += 360.0f;
		}
		isResetAngle = true;
	}

	if (isResetAngle &&
		PadInput::GetJoyPad_x_right() == 0.0f)
	{
		nowDegree = Calculation::RotationAngleDegree(newDegree, nowDegree, rotationSpeed);

		if (nowDegree == newDegree)
		{
			isResetAngle = false;
		}
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