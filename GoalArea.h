#pragma once
#include "BaseObject.h"
#include "CollisionData.h"

class GoalArea : public BaseObject
{
public:
	GoalArea();
	~GoalArea();

	void Initialize()override;
	void Update()override;
	bool Draw()override { return false; }

	AABB GetGoalArea() { return goalArea; }
private:
	AABB goalArea;
};

