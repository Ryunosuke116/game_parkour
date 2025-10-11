#pragma once
#include "BaseObject.h"

class SkyFloor : public BaseObject
{
public:
	SkyFloor(const int modelHandle, 
		const nlohmann::json& jsonData);
	~SkyFloor();

	void Load(const nlohmann::json& jsonData)override;
	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

private:
	static constexpr float kVelocity = 0.5f;

	bool isUp;
	float moveAmount;
	VECTOR moveDirection;
	VECTOR kInitPosition;
	nlohmann::json stateData;
};

