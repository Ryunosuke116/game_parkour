#pragma once
#include "DxLib.h"

struct BoundaryRange
{
	static constexpr VECTOR min = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR max = { 4800.0f, 4800.0f, 4800.0f };
	static constexpr VECTOR parallelVelocity = { 2200.0f, 0.0f, 1900.0f };
};