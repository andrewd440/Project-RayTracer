#pragma once
#include "Vector3.h"

#define _PI 3.14159265f
#define _EPSILON 0.00001f

/**
* Checks the bounds of a vector against current min and max bounds.
* The min and max bounds are updated if the new point is outside the current
* bounds.
* @param CurrentMin The current min bounds.
* @param CurrentMax The current max bounds.
* @param NewPoint The point to check.
*/
inline void UpdateBounds(Vector3f& CurrentMin, Vector3f& CurrentMax, const Vector3f& NewPoint)
{
	for (int i = 0; i < 3; i++)
	{
		if (CurrentMax[i] < NewPoint[i])
		{
			CurrentMax[i] = NewPoint[i];
		}
		else if (CurrentMin[i] > NewPoint[i])
		{
			CurrentMin[i] = NewPoint[i];
		}
	}
}