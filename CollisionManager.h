#pragma once

class HitCheck;

class CollisionManager
{
public:
	void Update(Player& player, int modelHandle);
	bool WallCollisionCheck(Player& player, int modelHandle);
	void Draw();

	VECTOR ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c);

	bool TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q);

private:
	HitCheck hitCheck;
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY* floor[100];	// 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	MV1_COLL_RESULT_POLY* wall[100];	// 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly_Ground_sphere;

	VECTOR subPos;
	VECTOR normal;
	VECTOR hitPos_wall;
	VECTOR hitPos_ground;
	VECTOR playerCenterPos;
	VECTOR oldPos;
	VECTOR newPos;
};

