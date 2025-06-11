#pragma once
class Game : public BaseScene
{
public:
	Game(SceneManager& manager);
	~Game();

	void Initialize();
	void Update();
	void Draw();
private:

	VECTOR lightPos;

	//’è‹`
	std::shared_ptr<Player> player = NULL;
	std::shared_ptr<Camera> camera = NULL;
	std::shared_ptr<Map> map = NULL;
	std::shared_ptr<Field> field = NULL;
	std::shared_ptr<PlayerManager> playerManager = NULL;
	std::shared_ptr<BaseManager> objectManager = NULL;
};

