#pragma once
#include "IObject.h"

class BaseUI : public IObject
{
public:
	BaseUI();
	~BaseUI(){}

	std::string GetJsonTag() { return jsonTag; }
	void Add()override{}
	void CreateMediator(ISoundPlayer& sound,
		IEffectManager& effect,
		Player& player,
		Camera& camera,
		std::vector<std::shared_ptr<BaseObject>>& collision)override{ }

protected:
	int x;
	int y;
	std::string jsonTag;

};

