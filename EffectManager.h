#pragma once
#include "BaseGameObjectManager.h"
#include "EffectData.h"

class EffectManager : public BaseGameObjectManager
{
public:
	EffectManager();
	~EffectManager();

	void Create()override{}
	void Initialize()override{}
	void Update()override{}
	void Draw()override;
	void Add()override{}

	void Add(const char* path, std::string& setTag,
		const float& scale);
	void PlayEffectUpdate();
	void SetPosition(const VECTOR setPosition,
		const std::string& tag);
	void SetRotation(const VECTOR& setPosition, const std::string& tag);
	void SetScale(const VECTOR& scale, const std::string& tag);
	void SetSpeed(const float& speed, const std::string& tag);
	void PlayEffect(const std::string& tag);
	void StopEffect(const std::string& tag);

private:
	std::vector<std::shared_ptr<EffectData>> effectDatas;
};

