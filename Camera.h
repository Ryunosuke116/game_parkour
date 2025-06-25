#pragma once
class Camera
{
private:
	VECTOR aimPosition;
	VECTOR lookPosition;
	VECTOR keepPlayerPosition;
	VECTOR keepEnemyPosition;
	VECTOR cameraDirection;
	VECTOR spherePosition;
	VECTOR centerPos;
	float angle;
	float a;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float radius = 2.0f;

public:
	Camera();
	~Camera();

	void Initialize();
	void Update(const VECTOR& playerPosition);
	void Draw();
	void RotateUpdate(const VECTOR& playerPosition);
	void PosCalc();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);
	void LeapCalc_single(float& changePos, const float targetPos, const float speed);
	void LockOnCamera();
	bool isObjectVisible();

	///////////////////////////////////////////////////////
	// ÉQÉbÉ^Å[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
};

