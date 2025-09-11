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
	void LookPosCalc();

	float CalculateBackEaseValue(float maxValue, float minValue, float nowValue);

	float InterpolationCalc(float easedValue, float maxValue, float minValue);
	float EaseOutQuad(float normalDistanceProgress);
	float CalculateBackEaseOutValue(float nowValue);

	///////////////////////////////////////////////////////
	// ゲッター
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetLookPosition() { return lookPosition; }

private:
	VECTOR cameraPosition;
	VECTOR newCameraPosition;
	VECTOR tentativeCameraPosition;
	VECTOR lookPosition;
	VECTOR cameraDirection;
	VECTOR centerPos;
	float angleRadian;
	float cameraDistanceWhenHittingObject;	//オブジェクトに当たっているときのカメラの距離
	float normalCameraDistance;				//通常時のカメラ距離
	float cameraDistance;					//カメラの距離
	float nowDegree;
	float newDegree;
	float min_t;
	float max_t;
	float normalDistanceProgress;			//カメラの距離の最小値から最大値までの進行度(0～1)
	float nowDistanceProgress;
	float easedCameraPosition;
	float normaleasedCameraPosition;

	bool isResetAngle;
	bool isHitObject;
	bool isPutBackDistance;					//通常時のカメラ距離に戻すか

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float lookRadius = 1.0f;
	static constexpr float cameraRadius = 4.0f;
	static constexpr float maxDistance = 70.0f;
	static constexpr float minDistance = 10.0f;
	static constexpr float initializeAngle = -177.55f;
	static constexpr float initializeDistance = 50.0f;
	static constexpr float initializeT = 0.7f;
	
	const VECTOR initializeAimPos = VGet(-0.169435501f, 53.7492065f, -1224.39844f);
	const VECTOR initializeSpherePos = VGet(-1.28232884f, 24.0028648f, -1172.35425f);

};

