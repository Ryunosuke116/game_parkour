#pragma once

class HitCheck;

class CollisionManager
{
public:
	void Update(Player& player, int modelHandle);
	bool WallCollisionCheck(Player& player, int modelHandle);
	void Draw();

private:
	HitCheck hitCheck;
	MV1_COLL_RESULT_POLY hitPoly_Ground;

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;

	VECTOR subPos;
	VECTOR normal;
};

