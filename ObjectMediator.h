#pragma once
#include <iostream>
#include <vector>
//#include "ISoundPlayer.h"
//#include "IEffectManager.h"
//#include "Player.h"
//#include "Camera.h"

class IEffectManager;
class Player;
class Camera;
class ISoundPlayer;
class BaseObject;

class ObjectMediator
{
public:
	ObjectMediator(ISoundPlayer& sound,
		IEffectManager& effect,
		Player& player,
		Camera& camera,
		std::vector<std::shared_ptr<BaseObject>>& collision):
		soundPlayer(&sound),
		effectManager(&effect),
		player(&player),
		camera(&camera)
	{
		collisionObjects.reserve(collision.size());
		for (auto& object : collision)
		{
			//sharedÇweakÇ…ïœçX
			collisionObjects.emplace_back(object);
		}
	}

	~ObjectMediator() = default;

	ISoundPlayer* soundPlayer;
	IEffectManager* effectManager;
	Player* player;
	Camera* camera;
	std::vector<std::weak_ptr<BaseObject>> collisionObjects;

private:

};

