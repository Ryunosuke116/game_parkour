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
	maxHeight(0.0f),
	minHeight(0.0f),
	cameraDistanceSize(0.0f),
	nowDegree(0.0f),
	newDegree(0.0f),
	angleRadian(0.0f),
	normalDistanceProgress(0.0f),
	normalCameraHeightProgress(0.0f),
	isResetAngle(false),
	isHitObject(false),
	isPutBackDistance(false),
	cameraDirection(VGet(0.0f, 0.0f, 0.0f)),
	normalCameraPosition(VGet(0.0f, 0.0f, 0.0f)),
	cameraPosition(VGet(0.0f, 0.0f, 0.0f)),
	centerPointSpherePos(VGet(0.0f, 0.0f, 0.0f)),
	screenCenterPosition(VGet(0.0f, 0.0f, 0.0f))
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
	screenCenterPosition = initializeSpherePos;
	nowDegree = initializeAngle;
	cameraDistanceSize = initializeDistance;
	normalDistanceProgress = initializeT;
	normalLinearProgress = initializeT;
	isResetAngle = false;
	isHitObject = false;
	normalCameraHeightProgress = 0.0f;

	SetCameraPositionAndTarget_UpVecY(cameraPosition, screenCenterPosition);
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	centerPointSpherePos = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPosition();
	centerPointSpherePos.y += addCenterPos;
	
	//注視する座標からplayerがずれたら修正する
	PosCalc();

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

	CaluclateCameraAndTargetDistanceSize();

	//座標更新
	SetCameraPositionAndTarget_UpVecY(cameraPosition, lookPosition);

	cameraDirection = VSub(screenCenterPosition, cameraPosition);
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
	centerPointSpherePos = playerPosition;
	centerPointSpherePos.y += 15.0f;

	DistanceUpdate();

	AngleUpdate(angle_player);

	RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(cameraPosition, screenCenterPosition);

	cameraDirection = VSub(screenCenterPosition, screenCenterPosition);

	cameraDirection = VNorm(cameraDirection);
}

/// <summary>
/// layout用カメラ
/// </summary>
void Camera::Update_layout()
{
	cameraDirection = VSub(screenCenterPosition, cameraPosition);
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
	screenCenterPosition = VAdd(screenCenterPosition, moveDirection);

	//y軸↑移動
	if (CheckHitKey(KEY_INPUT_UP) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPosition.y += 1.0f;
		screenCenterPosition.y += 1.0f;
	}
	//z軸↑移動
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		cameraPosition.z += 1.0f;
		screenCenterPosition.z += 1.0f;
	}

	//y軸↓移動
	if (CheckHitKey(KEY_INPUT_DOWN) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPosition.y -= 1.0f;
		screenCenterPosition.y -= 1.0f;
	}
	//z軸↓移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPosition.z -= 1.0f;
		screenCenterPosition.z -= 1.0f;
	}

	
	if (CheckHitKey(KEY_INPUT_9))
	{
		nowDegree = -177.55f;
	}

	//DistanceUpdate();

	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		cameraPosition.y += 1.0f;
		screenCenterPosition.y += 1.0f;
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		cameraPosition.y -= 1.0f;
		screenCenterPosition.y -= 1.0f;
	}

	AngleUpdate(1.0f);

	float angleRadian = nowDegree * DX_PI_F / 360.0f;
	this->angleRadian = angleRadian;

	cameraPosition.x = screenCenterPosition.x + cameraDistanceSize * cos(angleRadian);
	cameraPosition.z = screenCenterPosition.z + cameraDistanceSize * sin(angleRadian);

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(cameraPosition, centerPointSpherePos);
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{

	/*DrawSphere3D(screenCenterPosition, radius, 30, GetColor(0, 0, 0),
		    GetColor(255, 0, 0), FALSE);*/

	//printfDx("screenCenterPosition.x %f\n", screenCenterPosition.x);
	//printfDx("screenCenterPosition.y %f\n", screenCenterPosition.y);
	//printfDx("screenCenterPosition.z %f\n", screenCenterPosition.z);
	//printfDx("cameraPosition.x %f\n", cameraPosition.x);
	//printfDx("cameraPosition.y %f\n", cameraPosition.y);
	//printfDx("cameraPosition.z %f\n", cameraPosition.z);
}

/// <summary>
/// カメラの距離更新
/// </summary>
void Camera::DistanceUpdate()
{
	const float addHighCenterPos = 40.0f;		//カメラが中心からどれだけ離れられるか
	const float addLowCenterPos = addCenterPos - 5.0f;
	minHeight = screenCenterPosition.y - addLowCenterPos;
	maxHeight = screenCenterPosition.y + addHighCenterPos;

	const float addNormalizedTime = 0.02f;
	const float minNormalizedTimeDistance = 0.0f;
	const float maxNormalizedTime = 1.0f;

	if (PadInput::GetJoyPad_y_right() > 0.0f)
	{
		normalLinearProgress -= addNormalizedTime;
		
		if (normalLinearProgress <= minNormalizedTimeDistance)
		{
			normalLinearProgress = minNormalizedTimeDistance;
		}
	}
	if (PadInput::GetJoyPad_y_right() < 0.0f)
	{
		normalLinearProgress += addNormalizedTime;

		if (normalLinearProgress >= maxNormalizedTime)
		{
			normalLinearProgress = maxNormalizedTime;
		}
	}

	DebugDrawer::Instance().InformationInput_string_float("normalDistanceProgress %f\n", normalDistanceProgress);
	DebugDrawer::Instance().InformationInput_string_float("normalCameraHeightProgress %f\n", normalCameraHeightProgress);

	//リニア値からカメラの距離を求める
	normalCameraDistanceSize = Calculation::InterpolationCalc(normalLinearProgress, maxDistanceSize, minDistanceSize);
}

/// <summary>
/// カメラの回転値更新
/// </summary>
void Camera::AngleUpdate(const float& angle_player)
{
	if (PadInput::GetJoyPad_x_right() < 0.0f)
	{
		nowDegree -= 1.5f;
	}
	else if (PadInput::GetJoyPad_x_right() > 0.0f)
	{
		nowDegree += 1.5f;
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
	
	//注視点をカメラと反対方向に位置するようにする
	const float lookPositionDegree = degreesForRdianConversion + 180.0f;
	const float lookPositionRadian = Calculation::DegToRad(lookPositionDegree);

	this->angleRadian = Calculation::DegToRad(degreesForRdianConversion);

	//仮のカメラ座標
	normalCameraPosition.x = screenCenterPosition.x + normalCameraDistanceSize * -cos(angleRadian);
	normalCameraPosition.z = screenCenterPosition.z + normalCameraDistanceSize * sin(angleRadian);
	
	//リニア値からカメラの高さを求める
	normalCameraPosition.y = Calculation::InterpolationCalc(normalLinearProgress, maxHeight, minHeight);

	//注視点はカメラとは反対方向に距離と高さを求める
	VECTOR dir = VSub(screenCenterPosition, normalCameraPosition);
	dir = VNorm(dir);
	VECTOR scale = VScale(dir, 40.0f);
	lookPosition = VAdd(screenCenterPosition, scale);
}


/// <summary>
/// カメラの位置調整
/// </summary>
/// <param name="mapHandle"></param>
bool Camera::CameraPosCalc(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects)
{
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
			normalCameraPosition, 
			screenCenterPosition,
			hitPoly))
		{
			cameraVelocity = VSub(hitPoly.HitPosition, normalCameraPosition);
			VECTOR cameraDirection = VNorm(cameraVelocity);

			//このままだと壁との接触座標に移動するので壁から少し離す
			const VECTOR addVelocity = VScale(cameraDirection, velocityScale);
			cameraVelocity = VAdd(cameraVelocity, addVelocity);

			//オブジェクトからカメラを押し戻す
			VECTOR newCameraPosition = VAdd(normalCameraPosition, cameraVelocity);
			cameraPosition = VGet(newCameraPosition.x, cameraPosition.y, newCameraPosition.z);

			subPosition = VGet(cameraPosition.x, centerPointSpherePos.y, cameraPosition.z);

			//中心からカメラまでの距離を求める
			VECTOR distanceCenterAndCamera = VSub(centerPointSpherePos, subPosition);

			//オブジェクトに当たっているときのカメラの距離を求める
			cameraDistanceWhenHittingObject = VSize(distanceCenterAndCamera);
			cameraDistanceSize = cameraDistanceWhenHittingObject;

			//当たっているので抜け出す
			return hitPoly.HitFlag;
		}
	}

	return hitPoly.HitFlag;
}

/// <summary>
/// 最終的なカメラの座標を求める
/// </summary>
void Camera::AdjustCameraPosition()
{
	if (!isHitObject)
	{
		if (PadInput::GetJoyPad_y_right() != 0.0f)
		{
			cameraDistanceSize = Calculation::LeapFloat(cameraDistanceSize, normalCameraDistanceSize, 0.2f);
		}
		else if (PadInput::GetJoyPad_x_left() != 0.0f ||
			PadInput::GetJoyPad_y_left() != 0.0f ||
			PadInput::GetJoyPad_x_right() != 0.0f)
		{
			cameraDistanceSize = Calculation::LeapFloat(cameraDistanceSize, normalCameraDistanceSize, 0.02f);
		}
	}

	//カメラの距離の進行値を求める
	progressCameraPosition = Calculation::CalculateBackProgress(maxDistanceSize, minDistanceSize, cameraDistanceSize);

	//カメラ座標
	cameraPosition.x = screenCenterPosition.x + cameraDistanceSize * -cos(angleRadian);
	cameraPosition.z = screenCenterPosition.z + cameraDistanceSize * sin(angleRadian);
	cameraPosition.y = normalCameraPosition.y;

	DebugDrawer::Instance().InformationInput_string_float("cameraDistanceSize %f\n", cameraDistanceSize);
	DebugDrawer::Instance().InformationInput_string_float("normalCameraDistanceSize %f\n", normalCameraDistanceSize);
	DebugDrawer::Instance().InformationInput_string_float("progressCameraPosition %f\n", progressCameraPosition);
	DebugDrawer::Instance().InformationInput_string_VECTOR("normalCameraPosition %f %f %f\n", normalCameraPosition);
	DebugDrawer::Instance().InformationInput_string_VECTOR("cameraPosition %f %f %f\n", cameraPosition);
	DebugDrawer::Instance().InformationInput_string_VECTOR("screenCenterPosition %f %f %f\n", screenCenterPosition);
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

void Camera::PosCalc()
{
	static constexpr float normalCameraSpeed = 0.05f;
	const float outSphereCameraSpeed = 0.1f;

	//lookPosが球の外側にいった場合球の中心座標をずらす
	if (!HitCheck::HitConfirmation(screenCenterPosition, centerPointSpherePos, lookRadius, 0.5f))
	{
		screenCenterPosition = Calculation::Leap(screenCenterPosition, centerPointSpherePos, outSphereCameraSpeed);
	}
	else
	{
		screenCenterPosition = Calculation::Leap(screenCenterPosition, centerPointSpherePos, normalCameraSpeed);
	}
}

/// <summary>
/// カメラと目標の大きさを求める
/// </summary>
void Camera::CaluclateCameraAndTargetDistanceSize()
{
	//高さは合わせる
	VECTOR cameraPositionForCalculation = VGet(cameraPosition.x, centerPointSpherePos.y, cameraPosition.z);
	
	VECTOR cameraAndTargetDistance = VSub(cameraPositionForCalculation, centerPointSpherePos);

	cameraAndTargetDistanceSize = VSize(cameraAndTargetDistance);
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Camera::ResultInitialize()
{
	Initialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Camera::ResultUpdate()
{
	centerPointSpherePos = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPosition();
	centerPointSpherePos.y += addCenterPos;
	
	//注視する座標からplayerがずれたら修正する
	PosCalc();
	
	AdjustCameraPosition();

	CaluclateCameraAndTargetDistanceSize();

	//座標更新
	SetCameraPositionAndTarget_UpVecY(cameraPosition, lookPosition);
}