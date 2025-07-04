#include "common.h"
#include "HangringCalclation.h"
#include "Calclation.h"

void HangringCalclation::PositionCheck(const VECTOR& hangringPos,const VECTOR& playerPos,
	const VECTOR& playerPos_top, const VECTOR& playerPos_bottom)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hangring = hangringPos;

	Calclation::Leap(newPlayerPos, hangring, 0.1f);


}