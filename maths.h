#ifndef _MATHS_H_
#define _MATHS_H_

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>


struct Vector4
{
	float x, y, z, w;

	Vector4 operator+(const Vector4 &rhs)
	{
		Vector4 result = *this;
		result += rhs;
		return result;
	}

	Vector4& operator+=(const Vector4 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}

	Vector4 operator-(const Vector4 &rhs)
	{
		Vector4 result = *this;
		result -= rhs;
		return result;
	}
	Vector4 operator-(const Vector4 &rhs) const
	{
		Vector4 result = *this;
		result -= rhs;
		return result;
	}

	Vector4& operator-=(const Vector4 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}

	Vector4 operator*(const Vector4 &rhs)
	{
		Vector4 result = *this;
		result *= rhs;
		return result;
	}

	Vector4& operator*=(const Vector4 &rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;
		this->w *= rhs.w;
		return *this;
	}

	Vector4 operator/(const Vector4 &rhs)
	{
		Vector4 result = *this;
		result /= rhs;
		return result;
	}

	Vector4& operator/=(const Vector4 &rhs)
	{
		this->x /= rhs.x;
		this->y /= rhs.y;
		this->z /= rhs.z;
		this->w /= rhs.w;
		return *this;
	}

	bool operator==(const Vector4 &rhs)
	{
		return (this->x == rhs.x&&this->y == rhs.y&&this->z == rhs.z&&this->w == rhs.w);
	}

	Vector4()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, w(0.0f)
	{};

	Vector4(float xIn, float yIn, float zIn)
		: x(xIn)
		, y(yIn)
		, z(zIn)
		, w(0.0f)
	{};

	Vector4(float xIn, float yIn, float zIn, float wIn)
		: x(xIn)
		, y(yIn)
		, z(zIn)
		, w(wIn)
	{};

	Vector4(const XMVECTOR &rhs) 
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}

	Vector4(const XMFLOAT3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 0;
	}

	bool operator!=(const Vector4 &rhs)
	{
		return !(*this == rhs);
	}

	XMVECTOR getXMVector() const
	{
		XMVECTOR v;
		v.x = x;
		v.y = y;
		v.z = z;
		v.w = w;
		return v;
	}
};

static const Vector4 ZeroVector4 = { 0.0f, 0.0f, 0.0f, 0.0f };

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
