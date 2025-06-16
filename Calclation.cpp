#include <iostream>
#include <cassert>
#include <unordered_map>
#include <string_view>
#include <array>
#include <vector>
#include <math.h>    
#include <cmath>
#include <memory>
#include "DxLib.h"
#include "Calclation.h"


float Calclation::area(const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	VECTOR AB = VSub(b, a);
	VECTOR AC = VSub(c, a);
	VECTOR cross = VCross(AB, AC);

	float area = 0.5f * VSize(cross);
	return 0.5f * VSize(cross);
}

void Calclation::Initialize()
{
	if (instance != nullptr)
	{
		return;
	}

	instance = new Calclation;
}

void Calclation::UnInitialize()
{
	if (instance)
	{
		delete instance;

		instance = nullptr;
	}
}

Calclation* Calclation::instance = nullptr;