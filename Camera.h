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
	void PosCalc();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);

	void Update_start(const float& timer,
		const VECTOR& playerPosition,
		const float& angle_player);
	void Update_layout();
	void CameraPosCalc(const int& mapHandle);

	///////////////////////////////////////////////////////
	// ÉQÉbÉ^Å[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetSpherePosition() { return spherePosition; }

private:
	VECTOR aimPosition;
	VECTOR aimPosition_usual;
	VECTOR lookPosition;
	VECTOR cameraDirection;
	VECTOR spherePosition;
	VECTOR centerPos;
	VECTOR direction;
	float angleRadian;
	float distance;
	float nowDegree;
	float newDegree;
	float min_t;
	float max_t;
	float t;

	bool isResetAngle;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float radius = 1.0f;
	static constexpr float maxDistance = 60.0f;
	static constexpr float initializeAngle = -177.55f;
	static constexpr float initializeDistance = 50.0f;
	static constexpr float initializeT = 0.7f;
	
	const VECTOR initializeAimPos = VGet(-0.169435501f, 53.7492065f, -1224.39844f);
	const VECTOR initializeSpherePos = VGet(-1.28232884f, 24.0028648f, -1172.35425f);

};

