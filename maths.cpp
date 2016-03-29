#include "maths.h"

float dot(const Vector4& vector1, const Vector4& vector2)
{
	return ((vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z));
}

Vector4 cross(const Vector4& vector1, const Vector4& vector2)
{
	Vector4 result;
	result.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
	result.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
	result.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);
	return result;
}

Vector4 normal(const Triangle& triangle)
{
	//fix
	Vector4 vector1, vector2, result;
	vector1.x = (triangle.vertexB.x - triangle.vertexA.x);
	vector1.y = (triangle.vertexB.y - triangle.vertexA.y);
	vector1.z = (triangle.vertexB.z - triangle.vertexA.z);
	vector2.x = (triangle.vertexC.x - triangle.vertexA.x);
	vector2.y = (triangle.vertexC.y - triangle.vertexA.y);
	vector2.z = (triangle.vertexC.z - triangle.vertexA.z);

	result = cross(vector1, vector2);
	float resultLength = (result.x*result.x) + (result.y*result.y) + (result.z*result.z);
	result.x = (result.x*result.x) / resultLength;
	result.y = (result.y*result.y) / resultLength;
	result.z = (result.z*result.z) / resultLength;

	return result;
}

Plane calculatePlane(const Triangle& triangle)
{
	Plane result;
	result.data = normal(triangle);
	result.data.w = -(dot(triangle.vertexA, result.data));
	return result;
}

int planeEquation(const Plane& plane, const Vector4& point)
{
	float ax = (plane.data.x*point.x);
	float by = (plane.data.y*point.y);
	float cz = (plane.data.z*point.z);
	return sign(ax + by + cz + plane.data.w);
}

int sign(float number) // returns 1 or -1 depending on sign of number, or 0 if zero
{
	return (number < 0.0f ? -1 : (number > 0.0f ? 1 : 0));
}

bool planeIntersection(const Plane& plane, const Vector4& pointA, const Vector4& pointB, Vector4& intersection)//its over here
{
	int pointAcheck = planeEquation(plane, pointA);
	int pointBcheck = planeEquation(plane, pointB);
	if (pointAcheck == pointBcheck)
		return false;

	Vector4 ray = pointB - pointA;

	float t = (-plane.data.w - dot(plane.data, pointA)) / dot(plane.data, ray);

	intersection.x = pointA.x + (ray.x * t);
	intersection.y = pointA.y + (ray.y * t);
	intersection.z = pointA.z + (ray.z * t);

	return true;
}

bool trianglePointIntersection(const Triangle& triangle, const Vector4& point)
{
	//cross AP and AB
	Vector4 AP = point - triangle.vertexA;
	Vector4 AB = triangle.vertexB - triangle.vertexA;
	Vector4 APxAB = cross(AP, AB);
	//cross BP and BC
	Vector4 BP = point - triangle.vertexB;
	Vector4 BC = triangle.vertexC - triangle.vertexB;
	Vector4 BPxBC = cross(BP, BC);
	//cross CP and CA
	Vector4 CP = point - triangle.vertexC;
	Vector4 CA = triangle.vertexA - triangle.vertexC;
	Vector4 CPxCA = cross(CP, CA);

	int AxB = sign(dot(APxAB, BPxBC));
	int BxC = sign(dot(BPxBC, CPxCA));
	int CxA = sign(dot(CPxCA, APxAB));

	return (AxB == BxC && AxB == CxA);

	//return !(AxB < 0 || BxC < 0 || CxA < 0);
}

float distanceBetweenVectors(const Vector4& vector1, const Vector4& vector2)
{
	Vector4 differenc = vector1 - vector2;
	return sqrtf((differenc.x*differenc.x) + (differenc.y * differenc.y) + (differenc.z*differenc.z));
}

float distanceBetweenVectorsSqr(const Vector4& vector1, const Vector4& vector2)
{
	Vector4 differenc = vector1 - vector2;
	return (differenc.x*differenc.x) + (differenc.y * differenc.y) + (differenc.z*differenc.z);
}