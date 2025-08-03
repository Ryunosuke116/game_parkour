#pragma once

class Shadow
{
public:
	Shadow();
	~Shadow();
	
	void Initialize();
	void Update(const VECTOR& pos);
	int GetShadowMapHandle()const { return shadowMapHandle; }

private:
	int shadowMapHandle;
	VECTOR lightDirection;

	static constexpr int Shadow_Quality = 2048;		//影のクオリティ
};

