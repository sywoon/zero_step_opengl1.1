#ifndef __VECTOR_H__
#define __VECTOR_H__


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

#endif //__VECTOR_H__