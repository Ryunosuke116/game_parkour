#pragma once
#include "PlayerData.h"

class PlayerStateObserver
{
public:
	virtual void OnChangeState(const PlayerData& playerData)abstract;
	virtual ~PlayerStateObserver() = default;

};

