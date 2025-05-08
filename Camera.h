#pragma once
class Camera
{
private:
	VECTOR aimPosition;
	VECTOR lookPosition;
	VECTOR keepPlayerPosition;
	VECTOR keepEnemyPosition;
	VECTOR cameraDirection;
	float angle;
	float a;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;

public:
	Camera();
	~Camera();

	void Initialize();
	void Update(const VECTOR& playerPosition);
	void Draw();
	void RotateUpdate(const VECTOR& playerPosition);
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);
	void LockOnCamera();
	bool isObjectVisible();

	///////////////////////////////////////////////////////
	// ÉQÉbÉ^Å[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
};

