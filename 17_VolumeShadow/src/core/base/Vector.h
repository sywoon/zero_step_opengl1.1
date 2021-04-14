#ifndef __VECTOR_H__
#define __VECTOR_H__


class Vector4
{
public:
	Vector4();
	Vector4(float X, float Y, float Z);
	Vector4(float X, float Y, float Z, float W);
	Vector4(const Vector4 &v);

	void operator=(Vector4 v);
	Vector4 operator-(Vector4 v);
	Vector4 operator+(Vector4 v);
	Vector4 operator*(Vector4 v);
	Vector4 operator/(Vector4 v);
	Vector4 operator+(float f);
	Vector4 operator-(float f);
	Vector4 operator*(float f);
	Vector4 operator/(float f);

	/** 将当前点沿某个方向延伸 */
	void extendVertexPos(Vector4 currentVertex,
		Vector4 lightPos,
		float Extend);

public:
	float x, y, z, w;
};


class Vector3
{
public:
	Vector3();
	Vector3(float xx, float yy, float zz);
	Vector3(const Vector3& vec);

    
	inline float 	length				();                    // 向量长度
	Vector3& 		normalize			();                    // 单位化向量
	float 			dotProduct			(const Vector3& v);    // 计算点积
	Vector3 		crossProduct		(const Vector3& v);    // 计算叉积

	// 重载操作符
	Vector3 		operator + 			(const Vector3& v);
	Vector3 		operator - 			(const Vector3& v);
	Vector3 		operator * 			(float scale);
	Vector3 		operator / 			(float scale);
	Vector3 		operator - 			();
	
public:
	float x, y, z;
};


class Vector2
{
public:
	Vector2(float xx = 0.0,float yy = 0.0) { x = xx; y = yy;  }
	Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

public:
	float x,y;
};


#endif //__VECTOR_H__