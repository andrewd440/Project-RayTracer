#pragma once

#include "Vector3.h"

struct AABB
{
	Vector3f min, max;

	Vector3f GetCenter() const
	{
		return (min + max) * 0.5f;
	}
};
