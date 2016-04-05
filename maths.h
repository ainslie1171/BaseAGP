#ifndef _MATHS_H_
#define _MATHS_H_

#include "vector.h"





struct Plane
{
	Vector4 data;
};

struct Triangle
{
	Vector4 vertexA;
	Vector4 vertexB;
	Vector4 vertexC;
};

float dot(const Vector4& vector1, const Vector4& vector2);
Vector4 cross(const Vector4& vector1, const Vector4& vector2);
Vector4 normal(const Triangle& triangle);
Plane calculatePlane(const Triangle& triangle);
int planeEquation(const Plane& plane, const Vector4& point);
int sign(float number);
bool planeIntersection(const Plane& plane, const Vector4& pointA, const Vector4& pointB, Vector4& intersection);//this is acommenr
bool planeRayIntersection(const Plane& plane, const Vector4& start, const Vector4& direction, Vector4& intersection);
bool trianglePointIntersection(const Triangle& triangle, const Vector4& point);
float distanceBetweenVectors(const Vector4& vector1, const Vector4& vector2);
float distanceBetweenVectorsSqr(const Vector4& vector1, const Vector4& vector2);
#endif // !_MATHS_H_
