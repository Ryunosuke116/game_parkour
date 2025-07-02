#pragma once
class HitCheck;

class CollisionManager
{
public:
	bool GroundCollisionCheck(int modelHandle, const VECTOR& oldPos, VECTOR& newPos, float addTopPos, float radius, float addBottomPos, bool isJump);
	bool HeadCollisionCheck(int modelHandle, VECTOR& newPos, float addTopPos, float radius, float addBottomPos);
	bool WallCollisionCheck(int modelHandle, VECTOR& newPos, VECTOR& oldPos, float radius, float addTopPos, float addBottomPos);
	//bool Update(Player& player, int modelHandle);
	bool Draw();

	std::pair<bool, VECTOR> Update(int modelHandle, const VECTOR& playerPos, 
		const VECTOR& moveVec, VECTOR& moveDirection, float radius, 
		float addTopPos, float addBottomPos, bool isJump, bool isFalling);

	bool TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q, const float radius);

	void CliffGrabbing(int modelHandle, 
		const VECTOR& topPosition, const VECTOR& moveDirection, const bool isFalling);
	VECTOR PushBackCalclation_sphere_mesh(const MV1_COLL_RESULT_POLY& poly, const VECTOR& bottomPos, const VECTOR& newPlayerPos, const float& radius);
	VECTOR CalcPushBack_SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground, const VECTOR& bottomPos, const float& radius);

private:
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY oldPoly;
	

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly_Ground_sphere;

	VECTOR hitPos_head;
	VECTOR subPos;
	VECTOR normal;
	VECTOR hitPos_wall;
	VECTOR hitPos_ground;
	VECTOR playerCenterPos;
	VECTOR oldPolyPos;
	VECTOR nearestPoint;
	VECTOR hitSphere;
	VECTOR topPos_ray;
	VECTOR bottomPos_ray;
	VECTOR hitHangringPos;

	bool isHitHangring;
};

