#pragma once

class Calclation
{
private:

	Calclation(){}
	~Calclation(){}

	static Calclation* instance;



public:
	static void Initialize();

	static void UnInitialize();

	static float area(const VECTOR& a, const VECTOR& b, const VECTOR& c);

	static VECTOR NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
		 const VECTOR& point);

	static VECTOR Leap(const VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);
	static VECTOR SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground);
};


