#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"
#include "IObjectManager.h"

class Camera : public BaseObject
{
public:
	Camera();
	~Camera();

	void Create()override {}
	void Load(const nlohmann::json& jsonData)override{}
	void Initialize()override;
	void Update(ObjectMediator& objectMediator)override;
	void Draw()override;

	void RotateUpdate();
	void AngleUpdate(const float& angle_player);
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
	float angle_radian;
	float distance;
	float angle_now;
	float angle_new;
	float min_t;
	float max_t;
	float t;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float radius = 1.0f;
	static constexpr float distance_max = 60.0f;
	static constexpr float initialize_angle = -177.55f;
	static constexpr float initialize_distance = 50.0f;
	static constexpr float initialize_t = 0.7f;
	
	const VECTOR initialize_aimPos = VGet(-0.169435501f, 53.7492065f, -1224.39844f);
	const VECTOR initialize_spherePos = VGet(-1.28232884f, 24.0028648f, -1172.35425f);

};

