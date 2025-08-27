#pragma once
#include "BaseGameObjectManager.h"
#include "EffectData.h"
#include "IEffectManager.h"

class EffectManager :
	public BaseGameObjectManager,
	public IEffectManager
{
public:
	EffectManager();
	~EffectManager();

	void Create()override;
	void Initialize()override{}
	void Update(ObjectMediator& objectMediator)override{}
	void Draw()override;
	void Add()override{}

	void Add(const char* path, std::string& setTag,
		const float& scale)override;
	void PlayEffectUpdate()override;
	void SetPosition(const VECTOR setPosition,
		const std::string& tag)override;
	void SetRotation(const VECTOR& setPosition, const std::string& tag)override;
	void SetScale(const VECTOR& scale, const std::string& tag)override;
	void SetSpeed(const float& speed, const std::string& tag)override;
	void PlayEffect(const std::string& tag)override;
	void StopEffect(const std::string& tag)override;

private:
	std::vector<std::shared_ptr<EffectData>> effectDatas;
};

