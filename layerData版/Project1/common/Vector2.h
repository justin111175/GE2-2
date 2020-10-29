#pragma once
#include <math.h>

// テンプレート
template<class T>class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;

	// 代入演算子　＝
	Vector2Template<T>& operator = (const Vector2Template&vec);

	// 添え字演算子
	T& operator [] (int i);

	// 比較演算子
	bool operator == (const Vector2Template&vec)const;
	bool operator != (const Vector2Template&vec)const;
	bool operator > (const Vector2Template&vec)const;
	bool operator >= (const Vector2Template&vec)const;
	bool operator <= (const Vector2Template&vec)const;

	// 単項演算子
	Vector2Template& operator +=(const Vector2Template<T>&vec);
	Vector2Template& operator -=(const Vector2Template<T>&vec);
	Vector2Template& operator *=(T k);
	Vector2Template& operator /=(T k);

	Vector2Template operator+()const;
	Vector2Template operator-()const;
};

// ベクトルの演算
// Vector2<T>　+　T 
template<class T>
Vector2Template<T> operator+(const Vector2Template<T>&u, T v);
// Vector2<T>　-　T 
template<class T>
Vector2Template<T> operator-(const Vector2Template<T>&u, T v);
// Vector2<T>　*　T 
template<class T>
Vector2Template<T> operator*(const Vector2Template<T>&u, T v);
// Vector2<T>　/　T 
template<class T>
Vector2Template<T> operator/(const Vector2Template<T>&u, T v);
// Vector2<T>　%　T 
template<class T>
Vector2Template<T> operator%(const Vector2Template<T>&u, T v);

// T  * Vector2<T>　
template<class T>
Vector2Template<T> operator*(T v, const Vector2Template<T>&u);

// Vector2<T>　+　Vector2<T>
template<class T>
Vector2Template<T> operator+(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>　-　Vector2<T>
template<class T>
Vector2Template<T> operator-(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>　*　Vector2<T>
template<class T>
Vector2Template<T> operator*(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>　/　Vector2<T>
template<class T>
Vector2Template<T> operator/(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>　%　Vector2<T>
template<class T>
Vector2Template<T> operator%(const Vector2Template<T>&u, const Vector2Template<T> v);


//内積を返す
template<class T>
Vector2Template<T> Dot(const Vector2Template<T>& va, const Vector2Template<T>& vb);

//外積を返す
template<class T>
Vector2Template<T> Cross(const Vector2Template<T>& va, const Vector2Template<T>& vb);



//1点のベクトルの大きさを返します
template<class T>
T Magnitude(Vector2Template<T>& vec);

//2点のベクトルの大きさを返します
template<class T>
Vector2Template<T> Magnitude(Vector2Template<T>& vec1, Vector2Template<T>& vec2);

//1点の正規化(大きさを１に)ベクトルを返します
template<class T>
Vector2Template<T> Normalize(Vector2Template<T>& vec);


using Vector2 = Vector2Template<int>;
using Vector2Dbl = Vector2Template<double>;
using Vector2Flt = Vector2Template<float>;

#include "detail/Vector2.h"