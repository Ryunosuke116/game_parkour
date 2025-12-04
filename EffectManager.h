#pragma once
#include "BaseGameObjectManager.h"
#include "EffectData.h"
#include "IWorldSubSystem.h"

class EffectManager :
	public IWorldSubSystem
{
public:
	EffectManager();
	~EffectManager();

	void Create(const std::string& sceneName)override;
	void Update()override;
	void Draw()override;
	void Shutdown()override;

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

	bool GetIsPlayEffect(const std::string& tag);

private:
	std::vector<std::shared_ptr<EffectData>> effectDatas;
};

