#pragma once
#include "BaseObject.h"

class Floor_sky : public BaseObject
{
public:
	Floor_sky(const int modelHandle, 
		const nlohmann::json& jsonData);
	~Floor_sky();

	void Load(const nlohmann::json& jsonData)override;
	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;


private:
	static constexpr float velocity = 0.5f;

	bool isUp;
	float moveAmount;
	VECTOR moveDirection;
	VECTOR position_Init;
	nlohmann::json stateData;
};

