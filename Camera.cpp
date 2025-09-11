#include "common.h"
#include <cmath>
#include "math.h"
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
	cameraDistance(0.0f),
	nowDegree(0.0f),
	newDegree(0.0f),
	angleRadian(0.0f),
	normalDistanceProgress(0.0f),
	isResetAngle(false),
	isHitObject(false),
	isPutBackDistance(false),
	cameraDirection(VGet(0.0f, 0.0f, 0.0f)),
	newCameraPosition(VGet(0.0f, 0.0f, 0.0f)),
	cameraPosition(VGet(0.0f, 0.0f, 0.0f)),
	centerPos(VGet(0.0f, 0.0f, 0.0f)),
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
	cameraPosition = initializeAimPos;
	lookPosition = initializeSpherePos;
	nowDegree = initializeAngle;
	cameraDistance = initializeDistance;
	normalDistanceProgress = initializeT;
	isResetAngle = false;
	isHitObject = false;

	SetCameraPositionAndTarget_UpVecY(cameraPosition, lookPosition);
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	const float addCenterPos = 15.0f;		//playerPosのY軸に加算してカメラと注視点の間の座標を求める
	centerPos = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPosition();
	centerPos.y += addCenterPos;
	
	//カメラの中心からの距離を更新
	DistanceUpdate();

	//カメラの回転値を更新する
	AngleUpdate(WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetDegree());

	//カメラを回転させる
	RotateUpdate();

	//カメラがオブジェクトに埋まらないようにする
	isHitObject = CameraPosCalc(
		WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects());
	
	AdjustCameraPosition();

	//注視する座標からplayerがずれたら修正する
	PosCalc();

	//座標更新
	SetCameraPositionAndTarget_UpVecY(cameraPosition, lookPosition);

	cameraDirection = VSub(lookPosition, cameraPosition);
	cameraDirection = VNorm(cameraDirection);
}

/// <summary>
/// スタート時用の更新処理
/// </summary>
/// <param name="timer"></param>
/// <param name="playerPosition"></param>
/// <param name="angle_player"></param>
void Camera::Update_start(const float& timer,
	const VECTOR& playerPosition,
	const float& angle_player)
{
	centerPos = playerPosition;
	centerPos.y += 15.0f;

	DistanceUpdate();

	AngleUpdate(angle_player);

	RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(cameraPosition, lookPosition);

	cameraDirection = VSub(lookPosition, cameraPosition);

	cameraDirection = VNorm(cameraDirection);
}

/// <summary>
/// layout用カメラ
/// </summary>
void Camera::Update_layout()
{
	cameraDirection = VSub(lookPosition, cameraPosition);
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

	cameraPosition = VAdd(cameraPosition, moveDirection);
	lookPosition = VAdd(lookPosition, moveDirection);

	//y軸↑移動
	if (CheckHitKey(KEY_INPUT_UP) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPosition.y += 1.0f;
		lookPosition.y += 1.0f;
	}
	//z軸↑移動
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		cameraPosition.z += 1.0f;
		lookPosition.z += 1.0f;
	}

	//y軸↓移動
	if (CheckHitKey(KEY_INPUT_DOWN) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPosition.y -= 1.0f;
		lookPosition.y -= 1.0f;
	}
	//z軸↓移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPosition.z -= 1.0f;
		lookPosition.z -= 1.0f;
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
	//	cameraPosition.x += 1.0f;
	//	lookPosition.x += 1.0f;
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
	//	cameraPosition.x -= 1.0f;
	//	lookPosition.x -= 1.0f;
	//}

	if (CheckHitKey(KEY_INPUT_9))
	{
		nowDegree = -177.55f;
	}

	//DistanceUpdate();

	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		cameraPosition.y += 1.0f;
		lookPosition.y += 1.0f;
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		cameraPosition.y -= 1.0f;
		lookPosition.y -= 1.0f;
	}

	AngleUpdate(1.0f);

	float angleRadian = nowDegree * DX_PI_F / 360.0f;
	this->angleRadian = angleRadian;

	cameraPosition.x = lookPosition.x + cameraDistance * cos(angleRadian);
	cameraPosition.z = lookPosition.z + cameraDistance * sin(angleRadian);

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(cameraPosition, centerPos);
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{

	/*DrawSphere3D(lookPosition, radius, 30, GetColor(0, 0, 0),
		    GetColor(255, 0, 0), FALSE);*/

	//printfDx("lookPosition.x %f\n", lookPosition.x);
	//printfDx("lookPosition.y %f\n", lookPosition.y);
	//printfDx("lookPosition.z %f\n", lookPosition.z);
	//printfDx("cameraPosition.x %f\n", cameraPosition.x);
	//printfDx("cameraPosition.y %f\n", cameraPosition.y);
	//printfDx("cameraPosition.z %f\n", cameraPosition.z);
}

/// <summary>
/// カメラの距離更新
/// </summary>
void Camera::DistanceUpdate()
{
	const float addCenterPos = 40.0f;		//カメラが中心からどれだけ離れられるか
	min_t = lookPosition.y;
	max_t = lookPosition.y + addCenterPos;

	const float addNormalizedTime = 0.02f;
	const float minNormalizedTime = 0.0f;
	const float maxNormalizedTime = 1.0f;

	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		normalDistanceProgress -= addNormalizedTime;
		if (normalDistanceProgress <= minNormalizedTime)
		{
			normalDistanceProgress = minNormalizedTime;
		}
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		normalDistanceProgress += addNormalizedTime;
		if (normalDistanceProgress >= maxNormalizedTime)
		{
			normalDistanceProgress = maxNormalizedTime;
		}
	}

	normaleasedCameraPosition = EaseOutQuad(normalDistanceProgress);

	normalCameraDistance = InterpolationCalc(normaleasedCameraPosition, maxDistance, minDistance);

	//イージング値から座標を求める
	newCameraPosition.y = InterpolationCalc(normaleasedCameraPosition, max_t, min_t);
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

/// <summary>
/// カメラの回転更新
/// </summary>
void Camera::RotateUpdate()
{
	//キャラとカメラの正面方向が違うためplayerの正面方向に合わせる
	float degreesForRdianConversion = nowDegree;
	degreesForRdianConversion -= 90.0f;
	
	//180度以上はマイナス角度として扱う
	if (degreesForRdianConversion >= 180.0f)
	{
		degreesForRdianConversion -= 360.0f;
	}

	this->angleRadian = Calculation::DegToRad(degreesForRdianConversion);

	//仮のカメラ座標
	newCameraPosition.x = lookPosition.x + normalCameraDistance * -cos(angleRadian);
	newCameraPosition.z = lookPosition.z + normalCameraDistance * sin(angleRadian);

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;
}


/// <summary>
/// カメラの位置調整
/// </summary>
/// <param name="mapHandle"></param>
bool Camera::CameraPosCalc(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects)
{
	tagMV1_COLL_RESULT_POLY_DIM hitPolySphere;
	MV1_COLL_RESULT_POLY hitPoly;
	VECTOR cameraVelocity;
	VECTOR subPosition;
	const float velocityScale = 2.0f;

	for (const auto& fieldObject : collisionObjects)
	{
		const auto collisionObject = fieldObject.lock();
	
		//rayが当たっている場合カメラの位置をいじる
		if (HitCheck::RayHitJudge(
			collisionObject->GetModelHandle(),
			-1,
			lookPosition,
			newCameraPosition, 
			hitPoly))
		{
			cameraVelocity = VSub(hitPoly.HitPosition, newCameraPosition);
			VECTOR cameraDirection = VNorm(cameraVelocity);

			//このままだと壁との接触座標に移動するので壁から少し離す
			const VECTOR addVelocity = VScale(cameraDirection, velocityScale);
			cameraVelocity = VAdd(cameraVelocity, addVelocity);

			cameraPosition = VAdd(newCameraPosition, cameraVelocity);

			subPosition = VGet(cameraPosition.x, centerPos.y, cameraPosition.z);

			VECTOR distanceCenterAndCamera = VSub(centerPos, subPosition);

			//オブジェクトに当たっているときのカメラの距離を求める
			cameraDistanceWhenHittingObject = VSize(distanceCenterAndCamera);
			cameraDistance = cameraDistanceWhenHittingObject;

			//当たっているので抜け出す
			return hitPoly.HitFlag;
		}

		//if (HitCheck::SphereHitJudge(collisionObject->GetModelHandle(),
		//	-1,
		//	cameraRadius,
		//	newCameraPosition,
		//	hitPolySphere))
		//{
		//	VECTOR addPos;

		//	for (int i = 0; i < hitPolySphere.HitNum; i++)
		//	{
		//		MV1_COLL_RESULT_POLY poly = MV1CollCheck_GetResultPoly(hitPolySphere, i);
		//		
		//		//面の交差座標の計算
		//		VECTOR hitPos = HitCheck::ClosestPtToPointTriangle(
		//			newCameraPosition,
		//			poly.Position[0],
		//			poly.Position[1],
		//			poly.Position[2]);

		//		//球の中心から三角形の接触座標までの方向
		//		VECTOR hitDirection = VSub(newCameraPosition, hitPos);
		//		hitDirection = VNorm(hitDirection);
		//		VECTOR addSphereHitPos = VScale(hitDirection, cameraRadius);

		//		//接触座標までの方向に球の中心から半径分を加算して
		//		// 球の表面の座標を求める
		//		VECTOR sphereHitPos = VAdd(newCameraPosition, addSphereHitPos);

		//		//押し戻し量を計算
		//		VECTOR addPos = VSub(hitPos, sphereHitPos);

		//		cameraPosition = VAdd(newCameraPosition, addPos);
		//	}
		//	// 検出したプレイヤーの周囲のポリゴン情報を開放する
		//	MV1CollResultPolyDimTerminate(hitPolySphere);

		//	return true;
		//}
	}

	return hitPoly.HitFlag;
}

void Camera::AdjustCameraPosition()
{
	if (!isHitObject)
	{
		if (PadInput::GetJoyPad_y_right() != 0.0f)
		{
			cameraDistance = Calculation::LeapFloat(cameraDistance, normalCameraDistance, 0.2f);
		}
		else if (PadInput::GetJoyPad_x_left() != 0.0f ||
			PadInput::GetJoyPad_y_left() != 0.0f ||
			PadInput::GetJoyPad_x_right() != 0.0f)
		{
			cameraDistance = Calculation::LeapFloat(cameraDistance, normalCameraDistance, 0.02f);
		}
	}

	easedCameraPosition = CalculateBackEaseValue(maxDistance, minDistance, cameraDistance);

	//カメラ座標
	cameraPosition.x = lookPosition.x + cameraDistance * -cos(angleRadian);
	cameraPosition.z = lookPosition.z + cameraDistance * sin(angleRadian);
	cameraPosition.y = InterpolationCalc(easedCameraPosition, max_t, min_t);


	DebugDrawer::Instance().InformationInput_string_float("cameraDistance %f\n", cameraDistance);
	DebugDrawer::Instance().InformationInput_string_float("normalCameraDistance %f\n", normalCameraDistance);
	DebugDrawer::Instance().InformationInput_string_VECTOR("cameraPosition %f %f %f\n", cameraPosition);
	DebugDrawer::Instance().InformationInput_string_VECTOR("newCameraPosition %f %f %f\n", newCameraPosition);
	DebugDrawer::Instance().InformationInput_string_VECTOR("lookPosition %f %f %f\n", lookPosition);
}

/// <summary>
/// カメラの角度をプレイヤーの向いている方向に合わせる
/// </summary>
/// <param name="angle_player"></param>
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

void Camera::LookPosCalc()
{
	//視点座標を求める
	VECTOR dir = VSub(centerPos, cameraPosition);
	dir = VNorm(dir);
	VECTOR scale = VScale(dir, 40.0f);
	lookPosition = VAdd(centerPos, scale);
}

void Camera::PosCalc()
{
	//lookPosが球の外側にいった場合球の中心座標をずらす
	if (!HitCheck::HitConfirmation(lookPosition, centerPos, lookRadius, 0.5f))
	{
		lookPosition = Calculation::Leap(lookPosition, centerPos, 0.1f);
	}
	else
	{
		lookPosition = Calculation::Leap(lookPosition, centerPos, 0.05f);
	}
}

/// <summary>
/// イージング値に基づいて結果を計算
/// </summary>
/// <param name="easedValue"></param>
/// <param name="maxValue"></param>
/// <param name="minValue"></param>
/// <returns></returns>
float Camera::InterpolationCalc(
	float easedValue,
	float maxValue, 
	float minValue)
{
	return minValue + (maxValue - minValue) * easedValue;
}

/// <summary>
/// イージング値を現在の値から求める
/// </summary>
/// <param name="maxValue"></param>
/// <param name="minValue"></param>
/// <param name="nowValue"></param>
/// <returns></returns>
float Camera::CalculateBackEaseValue(
	float maxValue,
	float minValue, 
	float nowValue)
{
	if (nowValue < minValue) return 0.0f;
	if (nowValue > maxValue) return 1.0f;

	return (nowValue - minValue) / (maxValue - minValue);
}

/// <summary>
/// イースアウト
/// </summary>
/// <param name="normalDistanceProgress"></param>
/// <returns></returns>
float Camera::EaseOutQuad(float normalDistanceProgress)
{
	const float maxSize = 1.0f;

	//2乗する
	const float subSize = pow((maxSize - normalDistanceProgress), 2.0f);

	return maxSize - subSize;
}

float Camera::CalculateBackEaseOutValue(float nowValue)
{
	const float maxSize = 1.0f;

	return maxSize - (maxSize - pow(maxSize, nowValue));
}