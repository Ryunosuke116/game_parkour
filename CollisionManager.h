#pragma once
class HitCheck;

class CollisionManager
{
public:
	bool GroundCollisionCheck(int modelHandle, VECTOR& newPos, float addTopPos, float radius, float addBottomPos, bool isJump);
	bool WallCollisionCheck(int modelHandle, VECTOR& newPos, VECTOR& oldPos, float radius, float addTopPos, float addBottomPos);
	//bool Update(Player& player, int modelHandle);
	void Draw();

	std::tuple<bool, bool, VECTOR> Update(int modelHandle, const VECTOR& playerPos, const VECTOR& moveVec, VECTOR& moveDirection, float radius, float addTopPos, float addBottomPos, bool isJump);

	VECTOR ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c);

	bool TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q, const float radius);

	void CliffGrabbing(int modelHandle);

private:
	HitCheck hitCheck;
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY oldPoly;
	

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly_Ground_sphere;

	VECTOR subPos;
	VECTOR normal;
	VECTOR hitPos_wall;
	VECTOR hitPos_ground;
	VECTOR playerCenterPos;
	VECTOR oldPolyPos;
};

