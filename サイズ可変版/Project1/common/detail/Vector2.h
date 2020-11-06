//#include "stdafx.h"
#include "../common/Vector2.h"
#include <cmath>
#include "..\Vector2.h"

template<class T>
Vector2Template<T>::Vector2Template()
{
	x = 0;
	y = 0;
}

template<class T>
Vector2Template<T>::Vector2Template(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<class T>
Vector2Template<T>::~Vector2Template()
{
}


// 代入演算子
template<class T>
Vector2Template<T> & Vector2Template<T>::operator =(const Vector2Template<T> & vec)
{
	x = vec.x;
	y = vec.y;

	return *this;

}

// 添え字演算子
template<class T>
T & Vector2Template<T>::operator[](int i)
{
	if (i == 0)
	{
		return x;
	}
	else if(i==1)
	{
		return y;
	}
	else
	{
		return x;
	}
}

// 比較演算子
template<class T>
bool Vector2Template<T>::operator==(const Vector2Template<T> & vec) const
{
	return ((this->x == vec.x) && (this->y == vec.y));
}
template<class T>
bool Vector2Template<T>::operator!=(const Vector2Template<T> & vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
}
template<class T>
bool Vector2Template<T>::operator>(const Vector2Template<T> & vec) const
{
	return ((this->x > vec.x) && (this->y > vec.y));
}
template<class T>
bool Vector2Template<T>::operator>=(const Vector2Template<T> & vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));
}
template<class T>
bool Vector2Template<T>::operator<=(const Vector2Template<T> & vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}



//単項演算子
template<class T>
Vector2Template<T> & Vector2Template<T>::operator+=(const Vector2Template<T> & vec)
{
	x += vec.x;
	y += vec.y;

	return *this;

//	return Vector2Template<T>(x+=vec.x, y += vec.y);

}
template<class T>
Vector2Template<T> & Vector2Template<T>::operator-=(const Vector2Template<T> & vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

template<class T>
Vector2Template<T> & Vector2Template<T>::operator*=(T k)
{
	//x *= k;
	//y *= k;

	//return *this;
	return Vector2Template<T>(x *= k, y *= k);
}

template<class T>
Vector2Template<T> & Vector2Template<T>::operator/=(T k)
{
	x /= k;
	y /= k;

	return *this;
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator+() const		//()const   この関数の中で全ての数字const
{

	return *this;						// *this この自身のままで（数字変わらない）
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-() const
{
	return Vector2Template<T>(-this->x,-this->y);
}

// ベクトルの演算
// Vector2<T>　+　T 
template<class T>
Vector2Template<T> operator+(const Vector2Template<T> & u, T v)
{

	return Vector2Template<T>(u.x + v, u.y + v);
}

// Vector2<T>　-　T 
template<class T>
Vector2Template<T> operator-(const Vector2Template<T> & u, T v)
{

	return Vector2Template<T>(u.x - v, u.y - v);

}
// Vector2<T>　*　T
template<class T>
Vector2Template<T> operator*(const Vector2Template<T> & u, T v)
{

	return Vector2Template<T>(u.x * v, u.y * v);

}


// Vector2<T>　/　T
template<class T>
Vector2Template<T> operator/(const Vector2Template<T> & u, T v)
{

	return Vector2Template<T>(u.x / v, u.y / v);

}
// Vector2<T>　%　T
template<class T>
Vector2Template<T> operator%(const Vector2Template<T> & u, T v)
{

	return Vector2Template<T>(u.x % v, u.y % v);

}

// T * Vector2<T>　
template<class T>
Vector2Template<T> operator*(T v, const Vector2Template<T> & u)
{

	return Vector2Template<T>(u.x * v, u.y * v);

}

// Vector2<T>　+　Vector2<T>
template<class T>
Vector2Template<T> operator+(const Vector2Template<T> & u, const Vector2Template<T> v)
{
	Vector2Template<T> vec;
	vec.x = u.x + v.x;
	vec.y = u.y + v.y;

	return vec;
}
// Vector2<T>　-　Vector2<T>
template<class T>
Vector2Template<T> operator-(const Vector2Template<T> & u, const Vector2Template<T> v)
{
	Vector2Template<T> vec;
	vec.x = u.x - v.x;
	vec.y = u.y - v.y;

	return vec;
}
// Vector2<T>　*　Vector2<T>
template<class T>
Vector2Template<T> operator*(const Vector2Template<T> & u, const Vector2Template<T> v)
{
	Vector2Template<T> vec;
	vec.x = u.x * v.x;
	vec.y = u.y * v.y;

	return vec;
}
// Vector2<T>　/　Vector2<T>
template<class T>
Vector2Template<T> operator/(const Vector2Template<T> & u, const Vector2Template<T> v)
{
	Vector2Template<T> vec;
	vec.x = u.x / v.x;
	vec.y = u.y / v.y;

	return vec;
}
// Vector2<T>　%　Vector2<T>
template<class T>
Vector2Template<T> operator%(const Vector2Template<T> & u, const Vector2Template<T> v)
{
	Vector2Template<T> vec;
	vec.x = u.x % v.x;
	vec.y = u.y % v.y;

	return vec;
}

//内積を返す
template<class T>
Vector2Template<T> Dot(const Vector2Template<T>& va, const Vector2Template<T>& vb)
{
	return va.x * vb.x + va.y * vb.y;
}

//外積を返す
template<class T>
Vector2Template<T> Cross(const Vector2Template<T>& va, const Vector2Template<T>& vb)
{
	return va.x * vb.y - vb.x * va.y;
}


//1点のベクトルの大きさを返します
template<class T>
T Magnitude(Vector2Template<T>& vec)
{

	// |vec| => hypot(vec.x,vec.y)
	return hypot(vec.x,vec.y);
}

//2点のベクトルの大きさを返します
template<class T>
Vector2Template<T> Magnitude(Vector2Template<T>& vec1, Vector2Template<T>& vec2)
{
	return Vector2Template<T>(vec1-vec2);
}


//1点の正規化(大きさを１に)ベクトルを返します
template<class T>
Vector2Template<T> Normalize(Vector2Template<T>&vec)
{
	T mag = Magnitude(vec);
	vec.x /= mag;
	vec.y /= mag;
	return vec;
}

