#pragma once
#include "BaseObject.h"

class ObjectMediator;

class Shadow : public BaseObject
{
public:
	Shadow();
	~Shadow();
	
	void Create()								override	{}
	void Load(const nlohmann::json& jsonData)	override	{}
	void Initialize()							override;
	void Update()								override;
	void Draw()									override	{}
	void ResultCreate()							override;
	void ResultInitialize()						override;
	void ResultUpdate()							override;

	void LayoutUpdate(const VECTOR& pos);

	int GetShadowMapHandle()const { return shadowMapHandle; }

private:
	int shadowMapHandle;
	VECTOR lightDirection;

	static constexpr int shadowQuality = 2048;		//影のクオリティ
};

