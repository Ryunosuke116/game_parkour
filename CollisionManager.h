#pragma once

class HitCheck;

class CollisionManager
{
public:
	bool GroundCollisionCheck(Player& player, int modelHandle, VECTOR& newPos);
	bool WallCollisionCheck(Player& player, int modelHandle, VECTOR& newPos, VECTOR& oldPos);
	//bool Update(Player& player, int modelHandle);
	void Draw();

	std::pair<bool, VECTOR> Update(Player& player, int modelHandle);

	VECTOR ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c);

	bool TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q, const float radius);

	void CliffGrabbing(Player& player,int modelHandle);

private:
	HitCheck hitCheck;
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly_Ground_sphere;

	VECTOR subPos;
	VECTOR normal;
	VECTOR hitPos_wall;
	VECTOR hitPos_ground;
	VECTOR playerCenterPos;
	VECTOR oldPolyPos;
};

