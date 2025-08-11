#pragma once
#include "BaseObject.h"
#include "CollisionData.h"
#include <nlohmann/json.hpp>

class GoalArea : public BaseObject
{
public:
	GoalArea();
	~GoalArea();

	void Load(const nlohmann::json& jsonData)override{}
	void Initialize()override;
	void Update()override;
	void Draw()override {}

	AABB GetGoalArea() { return goalArea; }
private:
	AABB goalArea;
};

