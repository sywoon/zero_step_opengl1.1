#include "Vector.h"

Vector3::Vector3() 
	:x(0.0)
	,y(0.0) 
	,z(0.0)
{ 
}

Vector3::Vector3( float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

Vector3::Vector3(const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


inline float Vector3::length()
{
	return (float)( x * x + y * y + z * z );
}

Vector3& Vector3::normalize()
{
	float len = length(); 
	if( len == 0 )
		len = 1;
	x = x / len;
	y = y / len;
	z = z / len;

	return *this;
}

 float Vector3::dotProduct(const Vector3& v)
{
	return ( x * v.x + y * v.y + z * v.z );
}

Vector3 Vector3::crossProduct(const Vector3& v)
{
	Vector3 vec;

	vec.x = y * v.z - z * v.y;
	vec.y = z * v.x - x * v.z;
	vec.z = x * v.y - y * v.x;
	
	return vec;
}

 Vector3 Vector3::operator +(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
}

 Vector3 Vector3::operator -(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;

	return vec;
}

 Vector3 Vector3::operator *(float scale)
{
	Vector3 vec;

	vec.x = x * scale;
	vec.y = y * scale;
	vec.z = z * scale;
	
	return vec;
}

 Vector3 Vector3::operator /(float scale)
{
	if (scale == 0.0)
		return Vector3(0.0f, 0.0f, 0.0f);

	Vector3 vec;
	vec.x = x / scale;
	vec.y = y / scale;
	vec.z = z / scale;
	return vec;
}

// 负号
 Vector3 Vector3::operator -()
{
	Vector3 vec(-x, -y, -z);
	return vec;
}













