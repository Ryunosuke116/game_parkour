#pragma once
class Camera
{
private:
	VECTOR aimPosition;
	VECTOR aimPosition_usual;
	VECTOR lookPosition;
	VECTOR keepPlayerPosition;
	VECTOR keepEnemyPosition;
	VECTOR cameraDirection;
	VECTOR spherePosition;
	VECTOR centerPos;
	float angle;
	float distance;
	float a;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float radius = 4.0f;

	static constexpr float distance_max = 60.0f;

public:
	Camera();
	~Camera();

	void Initialize();
	void Update(const VECTOR& playerPosition, const int& mapHandle);
	bool Draw();
	void RotateUpdate(const VECTOR& playerPosition);
	void PosCalc();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);


	void CameraPosCalc(const int& mapHandle);

	///////////////////////////////////////////////////////
	// ÉQÉbÉ^Å[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
};

