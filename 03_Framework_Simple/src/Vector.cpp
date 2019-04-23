#include "Vector.h"


// 向量的长度
inline float Vector3::length()
{
	return (float)( x * x + y * y + z * z );
}

// 单位化一向量
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

// 点积
 float Vector3::dotProduct(const Vector3& v)
{
	return ( x * v.x + y * v.y + z * v.z );
}

// 叉积
Vector3 Vector3::crossProduct(const Vector3& v)
{
	Vector3 vec;

	vec.x = y * v.z - z * v.y;
	vec.y = z * v.x - x * v.z;
	vec.z = x * v.y - y * v.x;
	
	return vec;
}

/** 操作符 + */
 Vector3 Vector3::operator +(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
}

/** 操作符 - */
 Vector3 Vector3::operator -(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;

	return vec;
}

/** 操作符 * */
 Vector3 Vector3::operator *(float scale)
{
	Vector3 vec;

	vec.x = x * scale;
	vec.y = y * scale;
	vec.z = z * scale;
	
	return vec;
}

/** 操作符 / */
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

/** 负号 */
 Vector3 Vector3::operator -()
{
	Vector3 vec(-x, -y, -z);
	return vec;
}













