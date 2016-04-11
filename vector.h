#ifndef _VECTOR_H_
#define _VECTOR_H_

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>

struct Vector3;
struct Vector4;

struct Vector3
{
	float x, y, z;
	Vector3 operator+(const Vector3 &rhs);
	Vector3& operator+=(const Vector3 &rhs);
	Vector3 operator-(const Vector3 &rhs);
	Vector3 operator-(const Vector3 &rhs) const;
	Vector3& operator-=(const Vector3 &rhs);
	Vector3 operator*(const Vector3 &rhs);
	Vector3 operator*(const Vector3 &rhs) const;
	Vector3& operator*=(const Vector3 &rhs);
	Vector3 operator/(const Vector3 &rhs);
	Vector3 operator/(const Vector3 &rhs) const;
	Vector3& operator/=(const Vector3 &rhs);
	bool operator==(const Vector3 &rhs);
	bool operator==(const Vector3 &rhs) const;
	bool operator!=(const Vector3 &rhs);
	bool operator!=(const Vector3 &rhs) const;
	Vector3();
	Vector3(float xIn, float yIn, float zIn);
	Vector3(const XMVECTOR &rhs);
	Vector3(const Vector4 &rhs);
	Vector3(const XMFLOAT3 &rhs);
	Vector3(const float rhs);
	XMVECTOR getXMVector() const;
	Vector3 normalise();
};

struct Vector4
{
	float x, y, z, w;
	Vector4 operator+(const Vector4 &rhs);
	Vector4& operator+=(const Vector4 &rhs);
	Vector4 operator-(const Vector4 &rhs);
	Vector4 operator-(const Vector4 &rhs) const;
	Vector4& operator-=(const Vector4 &rhs);
	Vector4 operator*(const Vector4 &rhs);
	Vector4 operator*(const Vector4 &rhs) const;
	Vector4& operator*=(const Vector4 &rhs);
	Vector4 operator/(const Vector4 &rhs);
	Vector4& operator/=(const Vector4 &rhs);
	bool operator==(const Vector4 &rhs);
	Vector4();
	Vector4(float xIn, float yIn, float zIn);
	Vector4(float xIn, float yIn, float zIn, float wIn);
	Vector4(const XMVECTOR &rhs);
	Vector4(const XMFLOAT3 &rhs);
	Vector4(const XMFLOAT4 &rhs);
	Vector4(const Vector3 &rhs);
	Vector4(const float rhs);
	bool operator!=(const Vector4 &rhs);
	XMVECTOR getXMVector() const;
	XMFLOAT4 getXMFloat4() const;
};

static const Vector4 ZeroVector4 = { 0.0f, 0.0f, 0.0f, 0.0f };
static const Vector3 ZeroVector3 = { 0.0f, 0.0f, 0.0f };

#endif //_VECTOR_H_