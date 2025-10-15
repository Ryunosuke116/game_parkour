#include "Common.h"
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
	isPushRT(false),
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
	cameraPosition = kInitializeAimPos;
	screenCenterPosition = kInitializeSpherePos;
	nowDegree = kInitializeAngle;
	cameraDistanceSize = kInitializeDistance;
	normalCameraDistanceSize = kInitializeDistance;
	normalDistanceProgress = kInitializeProgress;
	normalLinearProgress = kInitializeProgress;
	isResetAngle = false;
	isHitObject = false;
	isPushRT = false;
	isPush = false;
	normalCameraHeightProgress = 0.0f;

	SetCameraPositionAndTarget_UpVecY(cameraPosition, screenCenterPosition);
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	centerPointSpherePos = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPosition();
	centerPointSpherePos.y += kAddCenterPosY;
	
	//注視する座標からplayerがずれたら修正する
	PosCalc();

	//カメラの中心からの距離を更新
	DistanceUpdate();

	//カメラの回転値を更新する
	AngleUpdate(WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetDegree());

	//カメラを回転させる
	RotateUpdate();

	//カメラがオブジェクトに埋まらないようにする
	if(CheckHitKey(KEY_INPUT_4))
	{
		if (!isPush)
		{
			isPush = true;
			isHitObject = false;
		}
	}
	else
	{
		isHitObject = CameraPosCalc(WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects());
		isPush = false;
	}
	
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
/// <param name="playerAngle"></param>
void Camera::StartUpdate(const float& timer,
	const VECTOR& playerPosition,
	const float playerAngle)
{
	const float kAddCenterPointSpherePosY = 15.0f;
	centerPointSpherePos = playerPosition;
	centerPointSpherePos.y += kAddCenterPointSpherePosY;

	DistanceUpdate();

	AngleUpdate(playerAngle);

	RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(cameraPosition, screenCenterPosition);

	cameraDirection = VSub(screenCenterPosition, screenCenterPosition);

	cameraDirection = VNorm(cameraDirection);
}

/// <summary>
/// layout用カメラ
/// </summary>
void Camera::LayOutUpdate()
{
	const float kAddCameraPos = 1.0f;
	const float kNormalAddDegree = 2.5f;
	const float kSpecialAddDegree = 30.0f;

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
	moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPadXLeft()),
		VScale(upMove, -PadInput::GetJoyPadYLeft()));

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
		cameraPosition.y += kAddCameraPos;
		screenCenterPosition.y += kAddCameraPos;
	}
	//z軸↑移動
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		cameraPosition.z += kAddCameraPos;
		screenCenterPosition.z += kAddCameraPos;
	}

	//y軸↓移動
	if (CheckHitKey(KEY_INPUT_DOWN) &&
		CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPosition.y -= kAddCameraPos;
		screenCenterPosition.y -= kAddCameraPos;
	}
	//z軸↓移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPosition.z -= kAddCameraPos;
		screenCenterPosition.z -= kAddCameraPos;
	}

	if (PadInput::GetJoyPadYRight() > 0.0f)
	{
		cameraPosition.y += kAddCameraPos;
		screenCenterPosition.y += kAddCameraPos;
	}
	if (PadInput::GetJoyPadYRight() < 0.0f)
	{
		cameraPosition.y -= kAddCameraPos;
		screenCenterPosition.y -= kAddCameraPos;
	}

	if (PadInput::GetJoyPadXRight() > 0.0f)
	{
		if (PadInput::IsPushRT())
		{
			if (!isPushRT)
			{
				nowDegree -= kSpecialAddDegree;
				isPushRT = true;
			}
		}
		else
		{
			nowDegree -= kNormalAddDegree;
		}
	}
	else if (PadInput::GetJoyPadXRight() < 0.0f)
	{
		if (PadInput::IsPushRT())
		{
			if (!isPushRT)
			{
				nowDegree += kSpecialAddDegree;
				isPushRT = true;
			}
		}
		else
		{
			nowDegree += kNormalAddDegree;
		}
	}
	else
	{
		isPushRT = false;
	}

	if (PadInput::IsPushR())
	{
		nowDegree = -90.0f;
	}

	//180度以上はマイナス角度として扱う
	if (nowDegree >= 180.0f)
	{
		nowDegree -= 360.0f;
	}

	float angleRadian = Calculation::DegToRad(nowDegree);
	this->angleRadian = angleRadian;

	

	cameraPosition.x = screenCenterPosition.x + cameraDistanceSize * cos(angleRadian);
	cameraPosition.z = screenCenterPosition.z + cameraDistanceSize * sin(angleRadian);

	SetCameraPositionAndTarget_UpVecY(cameraPosition, screenCenterPosition);
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{
}

/// <summary>
/// カメラの距離更新
/// </summary>
void Camera::DistanceUpdate()
{
	const float kAddHighCenterPos = 40.0f;		//カメラが中心からどれだけ離れられるか
	const float kAddNormalizedTime = 0.02f;
	const float kMinNormalizedTimeDistance = 0.0f;
	const float kMaxNormalizedTime = 1.0f;
	const float addLowCenterPos = kAddCenterPosY - 5.0f;

	minHeight = screenCenterPosition.y - addLowCenterPos;
	maxHeight = screenCenterPosition.y + kAddHighCenterPos;

	if (PadInput::GetJoyPadYRight() > 0.0f)
	{
		normalLinearProgress -= kAddNormalizedTime;
		
		if (normalLinearProgress <= kMinNormalizedTimeDistance)
		{
			normalLinearProgress = kMinNormalizedTimeDistance;
		}
	}
	if (PadInput::GetJoyPadYRight() < 0.0f)
	{
		normalLinearProgress += kAddNormalizedTime;

		if (normalLinearProgress >= kMaxNormalizedTime)
		{
			normalLinearProgress = kMaxNormalizedTime;
		}
	}

	DebugDrawer::GetInstance().InformationInput_string_float("normalDistanceProgress %f\n", normalDistanceProgress);
	DebugDrawer::GetInstance().InformationInput_string_float("normalCameraHeightProgress %f\n", normalCameraHeightProgress);

	//リニア値からカメラの距離を求める
	normalCameraDistanceSize = Calculation::InterpolationCalc(normalLinearProgress, kMaxDistanceSize, kMinDistanceSize);
}

/// <summary>
/// カメラの回転値更新
/// </summary>
void Camera::AngleUpdate(const float& playerAngle)
{
	const float kAddDegree = 1.5f;

	if (PadInput::GetJoyPadXRight() < 0.0f)
	{
		nowDegree -= kAddDegree;
	}
	else if (PadInput::GetJoyPadXRight() > 0.0f)
	{
		nowDegree += kAddDegree;
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

	ResetAngle(playerAngle);
}

/// <summary>
/// カメラの回転更新
/// </summary>
void Camera::RotateUpdate()
{
	//キャラとカメラの正面方向が違うためplayerの正面方向に合わせる
	float degreesForRdianConversion = nowDegree;
	degreesForRdianConversion -= 90.0f;
	const float kLookDirectionScale = 40.0f;
	const float kStartRayDirectionScale = -10.0f;
	
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
	VECTOR add = VScale(dir, kLookDirectionScale);
	lookPosition = VAdd(screenCenterPosition, add);

	//rayCast用の座標を算出
	VECTOR addStartRayCast = VScale(dir, kStartRayDirectionScale);
	rayCastStartPosition = VAdd(normalCameraPosition, addStartRayCast);
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
	const float kVelocityScale = 2.0f;
	const int kFrameIndex = -1;

	for (const auto& fieldObject : collisionObjects)
	{
		const auto collisionObject = fieldObject.lock();
	
		//rayが当たっている場合カメラの位置をいじる
		if (HitCheck::RayHitJudge(
			collisionObject->GetModelHandle(),
			kFrameIndex,
			screenCenterPosition,
			rayCastStartPosition,
			hitPoly))
		{
			//カメラの移動量を算出
			cameraVelocity = VSub(hitPoly.HitPosition, rayCastStartPosition);
			VECTOR cameraDirection = VNorm(cameraVelocity);

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
	const float kLeapSpeed = 0.2f;
	const float kSlowLeapSpeed = 0.02f;
	
	if (!isHitObject)
	{
		if (PadInput::GetJoyPadYRight() != 0.0f)
		{
			cameraDistanceSize = Calculation::Leap(cameraDistanceSize, normalCameraDistanceSize, kLeapSpeed);
		}
		else if (PadInput::GetJoyPadXLeft() != 0.0f ||
			PadInput::GetJoyPadYLeft() != 0.0f ||
			PadInput::GetJoyPadXRight() != 0.0f)
		{
			cameraDistanceSize = Calculation::Leap(cameraDistanceSize, normalCameraDistanceSize, kSlowLeapSpeed);
		}
	}

	//カメラの距離の進行値を求める
	progressCameraPosition = Calculation::CalculateBackProgress(kMaxDistanceSize, kMinDistanceSize, cameraDistanceSize);

	//カメラ座標
	cameraPosition.x = screenCenterPosition.x + cameraDistanceSize * -cos(angleRadian);
	cameraPosition.z = screenCenterPosition.z + cameraDistanceSize * sin(angleRadian);
	cameraPosition.y = normalCameraPosition.y;

	DebugDrawer::GetInstance().InformationInput_string_float("cameraDistanceSize %f\n", cameraDistanceSize);
	DebugDrawer::GetInstance().InformationInput_string_float("normalCameraDistanceSize %f\n", normalCameraDistanceSize);
	DebugDrawer::GetInstance().InformationInput_string_float("progressCameraPosition %f\n", progressCameraPosition);
	DebugDrawer::GetInstance().InformationInput_string_VECTOR("normalCameraPosition %f %f %f\n", normalCameraPosition);
	DebugDrawer::GetInstance().InformationInput_string_VECTOR("cameraPosition %f %f %f\n", cameraPosition);
	DebugDrawer::GetInstance().InformationInput_string_VECTOR("screenCenterPosition %f %f %f\n", screenCenterPosition);
}

/// <summary>
/// カメラの角度をプレイヤーの向いている方向に合わせる
/// </summary>
/// <param name="angle_player"></param>
void Camera::ResetAngle(const float& angle_player)
{
	const float kRotationSpeed = 6.0f;

	if (PadInput::IsPushR())
	{
		newDegree = angle_player;
		if (newDegree <= 0.0f)
		{
			newDegree += 360.0f;
		}
		isResetAngle = true;
	}

	if (isResetAngle &&
		PadInput::GetJoyPadXRight() == 0.0f)
	{
		nowDegree = Calculation::RotationAngleDegree(newDegree, nowDegree, kRotationSpeed);

		if (nowDegree == newDegree)
		{
			isResetAngle = false;
		}
	}
}

void Camera::PosCalc()
{
	static constexpr float kNormalCameraSpeed = 0.05f;
	const float kOutSphereCameraSpeed = 0.1f;
	const float kCenterPointSphereRadius = 0.5f;

	//lookPosが球の外側にいった場合球の中心座標をずらす
	if (!HitCheck::HitConfirmation(screenCenterPosition, centerPointSpherePos, kLookRadius, kCenterPointSphereRadius))
	{
		screenCenterPosition = Calculation::LeapVector(screenCenterPosition, centerPointSpherePos, kOutSphereCameraSpeed);
	}
	else
	{
		screenCenterPosition = Calculation::LeapVector(screenCenterPosition, centerPointSpherePos, kNormalCameraSpeed);
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
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void Camera::ResultCreate()
{
	Create();
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Camera::ResultInitialize()
{
	const float kInitNormalLinearProgress = 0.5f;
	const float kInitNormalCameraDistanceSize = 3.0f;

	Initialize();
	nowDegree = kResultInitializeAngle;
	normalLinearProgress = kInitNormalLinearProgress;
	normalCameraDistanceSize = kInitNormalCameraDistanceSize;
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Camera::ResultUpdate()
{
	const float kAddHighCenterPos = 40.0f;		//カメラが中心からどれだけ離れられるか
	const float kAddLowCenterPos = kAddCenterPosY - 5.0f;
	const float kAddCenterPosX = -3.0f;

	minHeight = screenCenterPosition.y - kAddLowCenterPos;
	maxHeight = screenCenterPosition.y + kAddHighCenterPos;

	centerPointSpherePos = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPosition();
	centerPointSpherePos.y += kAddCenterPosY;
	centerPointSpherePos.x += kAddCenterPosX;
	
	//注視する座標からplayerがずれたら修正する
	PosCalc();

	RotateUpdate();
	
	//カメラ座標
	cameraPosition = normalCameraPosition;

	CaluclateCameraAndTargetDistanceSize();

	//座標更新
	SetCameraPositionAndTarget_UpVecY(cameraPosition, lookPosition);
}