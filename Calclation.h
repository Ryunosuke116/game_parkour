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

};


