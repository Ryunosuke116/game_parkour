#pragma once

class HitCheck;

class CollisionManager
{
public:
	void Update(Player& player, int modelHandle);
	bool CalculateIntersection(const VECTOR& rayStartPos, const VECTOR& rayEndPos,
		const VECTOR& planePoint, const VECTOR& planeNormal, VECTOR& intersection);

private:
	HitCheck hitCheck;
	MV1_COLL_RESULT_POLY hitPoly_Ground;
	std::vector<MV1_COLL_RESULT_POLY> hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly;

};

