#include "AltVector.h"

float KVector3::dot(const ME &o) const
{
	return v[0] * o[0] + v[1] * o[1] + v[2] * o[2];
}

KVector3::Vector(float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}