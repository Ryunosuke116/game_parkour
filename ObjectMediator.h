#pragma once
//#include "ISoundPlayer.h"
//#include "IEffectManager.h"
//#include "Player.h"
//#include "Camera.h"

class IEffectManager;
class Player;
class Camera;
class ISoundPlayer;

class ObjectMediator
{
public:
	ObjectMediator(ISoundPlayer& sound,
		IEffectManager& effect,
		Player& player,
		Camera& camera):
		soundPlayer(&sound),
		effectManager(&effect),
		player(&player),
		camera(&camera)
	{ }

	~ObjectMediator() = default;

	ISoundPlayer* soundPlayer;
	IEffectManager* effectManager;
	Player* player;
	Camera* camera;

private:

};

