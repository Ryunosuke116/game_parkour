#pragma once
#include "BaseObject.h"

class ObjectMediator;

class Shadow : public BaseObject
{
public:
	Shadow();
	~Shadow();
	
	void Create()override {}
	void Load(const nlohmann::json& jsonData)override {}
	void Initialize()override;
	void Update()override;
	void Draw()override {}
	void ResultInitialize()override;
	void ResultUpdate()override;

	int GetShadowMapHandle()const { return shadowMapHandle; }

private:
	int shadowMapHandle;
	VECTOR lightDirection;

	static constexpr int Shadow_Quality = 2048;		//影のクオリティ
};

