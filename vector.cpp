#include "vector.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////               Vector3                /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator + (const Vector3 &rhs)
{
	Vector3 result = *this;
	result += rhs;
	return result;
}

Vector3& Vector3::operator+=(const Vector3 &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3 &rhs)
{
	Vector3 result = *this;
	result -= rhs;
	return result;
}

Vector3 Vector3::operator-(const Vector3 &rhs) const
{
	Vector3 result = *this;
	result -= rhs;
	return result;
}

Vector3& Vector3::operator-=(const Vector3 &rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return *this;
}

Vector3 Vector3::operator*(const Vector3 &rhs)
{
	Vector3 result = *this;
	result *= rhs;
	return result;
}

Vector3 Vector3::operator*(const Vector3 &rhs) const
{
	Vector3 result = *this;
	result *= rhs;
	return result;
}

Vector3& Vector3::operator*=(const Vector3 &rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	this->z *= rhs.z;
	return *this;
}

Vector3 Vector3::operator/(const Vector3 &rhs)
{
	Vector3 result = *this;
	result /= rhs;
	return result;
}

Vector3 Vector3::operator/(const Vector3 &rhs) const
{
	Vector3 result = *this;
	result /= rhs;
	return result;
}

Vector3& Vector3::operator/=(const Vector3 &rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	this->z /= rhs.z;
	return *this;
}

bool Vector3::operator==(const Vector3 &rhs)
{
	return (this->x == rhs.x&&this->y == rhs.y&&this->z == rhs.z);
}

bool Vector3::operator==(const Vector3 &rhs) const
{
	return (this->x == rhs.x&&this->y == rhs.y&&this->z == rhs.z);
}

Vector3::Vector3()
: x(0.0f)
, y(0.0f)
, z(0.0f)
{};

Vector3::Vector3(float xIn, float yIn, float zIn)
: x(xIn)
, y(yIn)
, z(zIn)
{};

Vector3::Vector3(const XMVECTOR &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}
Vector3::Vector3(const Vector4 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

Vector3::Vector3(const XMFLOAT3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}
Vector3::Vector3(const float rhs)
{
	x = rhs;
	y = rhs;
	z = rhs;
}

bool Vector3::operator!=(const Vector3 &rhs)
{
	return !(*this == rhs);
}

bool Vector3::operator!=(const Vector3 &rhs) const
{
	return !(*this == rhs);
}

XMVECTOR Vector3::getXMVector() const
{
	XMVECTOR v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 1.0f;
	return v;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////               Vector4                /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::operator + (const Vector4 &rhs)
{
	Vector4 result = *this;
	result += rhs;
	return result;
}

Vector4& Vector4::operator+=(const Vector4 &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->w += rhs.w;
	return *this;
}

Vector4 Vector4::operator-(const Vector4 &rhs)
{
	Vector4 result = *this;
	result -= rhs;
	return result;
}
Vector4 Vector4::operator-(const Vector4 &rhs) const
{
	Vector4 result = *this;
	result -= rhs;
	return result;
}

Vector4& Vector4::operator-=(const Vector4 &rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	this->w -= rhs.w;
	return *this;
}

Vector4 Vector4::operator*(const Vector4 &rhs)
{
	Vector4 result = *this;
	result *= rhs;
	return result;
}

Vector4& Vector4::operator*=(const Vector4 &rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	this->z *= rhs.z;
	this->w *= rhs.w;
	return *this;
}

Vector4 Vector4::operator/(const Vector4 &rhs)
{
	Vector4 result = *this;
	result /= rhs;
	return result;
}

Vector4& Vector4::operator/=(const Vector4 &rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	this->z /= rhs.z;
	this->w /= rhs.w;
	return *this;
}

bool Vector4::operator==(const Vector4 &rhs)
{
	return (this->x == rhs.x&&this->y == rhs.y&&this->z == rhs.z&&this->w == rhs.w);
}

Vector4::Vector4()
: x(0.0f)
, y(0.0f)
, z(0.0f)
, w(0.0f)
{};

Vector4::Vector4(float xIn, float yIn, float zIn)
: x(xIn)
, y(yIn)
, z(zIn)
, w(0.0f)
{};

Vector4::Vector4(float xIn, float yIn, float zIn, float wIn)
: x(xIn)
, y(yIn)
, z(zIn)
, w(wIn)
{};

Vector4::Vector4(const XMVECTOR &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
}

Vector4::Vector4(const XMFLOAT4 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 0;
}

Vector4::Vector4(const XMFLOAT3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 0;
}

Vector4::Vector4(const Vector3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 0.0f;
}

bool Vector4::operator!=(const Vector4 &rhs)
{
	return !(*this == rhs);
}

XMVECTOR Vector4::getXMVector() const
{
	XMVECTOR v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

XMFLOAT4 Vector4::getXMFloat4() const
{
	XMFLOAT4 f;
	f.x = x;
	f.y = y;
	f.z = z;
	f.w = w;
	return f;
}