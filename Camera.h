#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"

class Camera :
	public BaseObject,
	public std::enable_shared_from_this<Camera>
{
public:
	Camera();
	~Camera();

	void Create()override;
	void Load(const nlohmann::json& jsonData)override{}
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

	void RotateUpdate();
	void AngleUpdate(const float& angle_player);
	void ResetAngle(const float& angle_player);
	void DistanceUpdate();
	void AdjustCameraPosition();
	void PosCalc();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);

	void Update_start(const float& timer,
		const VECTOR& playerPosition,
		const float& angle_player);
	void Update_layout();
	bool CameraPosCalc(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects);
	void CaluclateCameraAndTargetDistanceSize();

	///////////////////////////////////////////////////////
	// ゲッター
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetLookPosition() { return screenCenterPosition; }
	float GetCameraDistance() { return cameraDistanceSize; }
	float GetCameraAndTargetDistanceSize() { return cameraAndTargetDistanceSize; }

private:
	VECTOR cameraPosition;
	VECTOR normalCameraPosition;
	VECTOR cameraDirection;
	VECTOR centerPointSpherePos;			//カメラの中間地点にある球の中心
	VECTOR screenCenterPosition;			//画面の中心座標
	VECTOR lookPosition;					//注視点
	float angleRadian;
	float cameraDistanceWhenHittingObject;	//オブジェクトに当たっているときのカメラの距離
	float normalCameraDistanceSize;				//通常時のカメラ距離
	float cameraDistanceSize;					//カメラの距離
	float cameraAndTargetDistanceSize;			//カメラと目標との距離
	float nowDegree;
	float newDegree;
	float minHeight;
	float maxHeight;
	float normalDistanceProgress;			//カメラの距離の最小値から最大値までの進行度(0～1)
	float normalCameraHeightProgress;		//カメラの高さの最小値から最大値までの進行度(0～1)
	float nowDistanceProgress;
	float progressCameraPosition;
	float normalLinearProgress;

	bool isResetAngle;
	bool isHitObject;
	bool isPutBackDistance;					//通常時のカメラ距離に戻すか

	static constexpr float lookRadius = 1.0f;
	static constexpr float cameraRadius = 4.0f;
	static constexpr float maxDistanceSize = 70.0f;
	static constexpr float minDistanceSize = 15.0f;
	static constexpr float initializeAngle = -177.55f;
	static constexpr float initializeDistance = 50.0f;
	static constexpr float initializeT = 0.7f;
	static constexpr float addCenterPos = 15.0f;					//playerPosのY軸に加算してカメラと注視点の間の座標を求める
	
	const VECTOR initializeAimPos = VGet(-0.169435501f, 53.7492065f, -1224.39844f);
	const VECTOR initializeSpherePos = VGet(-1.28232884f, 24.0028648f, -1172.35425f);

};

