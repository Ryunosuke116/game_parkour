#pragma once
#include "BaseUI.h"
#include "PlayerStateObserver.h"

class UI_controlManual : public BaseUI, public PlayerStateObserver
{
public:
	UI_controlManual();
	~UI_controlManual();

	void Create()override {}
	void Add()override{}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void OnChangeState(const PlayerData& playerData)override;

private:
	std::unordered_map<std::string, int> uiHandles;
	std::vector<int> draw_UIs;
	int stateNumber;

	PlayerData data;

};

