#pragma once

class HitCheck;

class CollisionManager
{
public:
	void Update(Player& player, int modelHandle);

private:
	HitCheck hitCheck;
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	std::vector<MV1_COLL_RESULT_POLY> hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly;

};

