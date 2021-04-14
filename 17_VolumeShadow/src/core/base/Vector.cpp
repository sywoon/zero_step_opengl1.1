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




 Vector4::Vector4()
 {

	 x = y = z = 0.0;
 }


 Vector4::Vector4(float X, float Y, float Z)
 {

	 x = X;
	 y = Y;
	 z = Z;
 }


 Vector4::Vector4(float X, float Y, float Z, float W)
 {
	 x = X;
	 y = Y;
	 z = Z;
	 w = W;
 }


 Vector4::Vector4(const Vector4 &v)
 {

	 x = v.x;
	 y = v.y;
	 z = v.z;
	 w = v.w;
 }

 void Vector4::operator =(Vector4 v)
 {
	 x = v.x;
	 y = v.y;
	 z = v.z;
	 w = v.w;
 }


 Vector4 Vector4::operator -(Vector4 v)
 {

	 return Vector4(x - v.x, y - v.y, z - v.z);
 }


 Vector4 Vector4::operator +(Vector4 v)
 {

	 return Vector4(x + v.x, y + v.y, z + v.z);
 }


 Vector4 Vector4::operator *(Vector4 v)
 {

	 return Vector4(x * v.x, y * v.y, z * v.z);
 }


 Vector4 Vector4::operator /(Vector4 v)
 {

	 return Vector4(x / v.x, y / v.y, z / v.z);
 }


 Vector4 Vector4::operator +(float f)
 {

	 return Vector4(x + f, y + f, z + f);
 }


 Vector4 Vector4::operator -(float f)
 {

	 return Vector4(x - f, y - f, z - f);
 }


 Vector4 Vector4::operator *(float f)
 {

	 return Vector4(x * f, y * f, z * f);
 }


 Vector4 Vector4::operator /(float f)
 {

	 f = 1 / f;

	 return Vector4(x * f, y * f, z * f);
 }

 /** 将顶点沿某光源方向延伸 */
 void Vector4::extendVertexPos(Vector4 currentVertex, Vector4 lightPos, float Extend)
 {
	 Vector4 lightDir;  /**< 光源位置到顶点的向量 */
	 Vector4 newPos;    /**< 延伸后的新顶点 */


	 lightDir = currentVertex - lightPos;

	 /** 计算延伸后的顶点 */
	 newPos = lightPos + lightDir * Extend;

	 /** 保存该顶点 */
	 x = newPos.x;
	 y = newPos.y;
	 z = newPos.z;
	 w = newPos.w;
 }











