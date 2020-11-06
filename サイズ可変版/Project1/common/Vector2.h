#pragma once
#include <math.h>

// �e���v���[�g
template<class T>class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;

	// ������Z�q�@��
	Vector2Template<T>& operator = (const Vector2Template&vec);

	// �Y�������Z�q
	T& operator [] (int i);

	// ��r���Z�q
	bool operator == (const Vector2Template&vec)const;
	bool operator != (const Vector2Template&vec)const;
	bool operator > (const Vector2Template&vec)const;
	bool operator >= (const Vector2Template&vec)const;
	bool operator <= (const Vector2Template&vec)const;

	// �P�����Z�q
	Vector2Template& operator +=(const Vector2Template<T>&vec);
	Vector2Template& operator -=(const Vector2Template<T>&vec);
	Vector2Template& operator *=(T k);
	Vector2Template& operator /=(T k);

	Vector2Template operator+()const;
	Vector2Template operator-()const;
};

// �x�N�g���̉��Z
// Vector2<T>�@+�@T 
template<class T>
Vector2Template<T> operator+(const Vector2Template<T>&u, T v);
// Vector2<T>�@-�@T 
template<class T>
Vector2Template<T> operator-(const Vector2Template<T>&u, T v);
// Vector2<T>�@*�@T 
template<class T>
Vector2Template<T> operator*(const Vector2Template<T>&u, T v);
// Vector2<T>�@/�@T 
template<class T>
Vector2Template<T> operator/(const Vector2Template<T>&u, T v);
// Vector2<T>�@%�@T 
template<class T>
Vector2Template<T> operator%(const Vector2Template<T>&u, T v);

// T  * Vector2<T>�@
template<class T>
Vector2Template<T> operator*(T v, const Vector2Template<T>&u);

// Vector2<T>�@+�@Vector2<T>
template<class T>
Vector2Template<T> operator+(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>�@-�@Vector2<T>
template<class T>
Vector2Template<T> operator-(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>�@*�@Vector2<T>
template<class T>
Vector2Template<T> operator*(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>�@/�@Vector2<T>
template<class T>
Vector2Template<T> operator/(const Vector2Template<T>&u, const Vector2Template<T> v);
// Vector2<T>�@%�@Vector2<T>
template<class T>
Vector2Template<T> operator%(const Vector2Template<T>&u, const Vector2Template<T> v);


//���ς�Ԃ�
template<class T>
Vector2Template<T> Dot(const Vector2Template<T>& va, const Vector2Template<T>& vb);

//�O�ς�Ԃ�
template<class T>
Vector2Template<T> Cross(const Vector2Template<T>& va, const Vector2Template<T>& vb);



//1�_�̃x�N�g���̑傫����Ԃ��܂�
template<class T>
T Magnitude(Vector2Template<T>& vec);

//2�_�̃x�N�g���̑傫����Ԃ��܂�
template<class T>
Vector2Template<T> Magnitude(Vector2Template<T>& vec1, Vector2Template<T>& vec2);

//1�_�̐��K��(�傫�����P��)�x�N�g����Ԃ��܂�
template<class T>
Vector2Template<T> Normalize(Vector2Template<T>& vec);


using Vector2 = Vector2Template<int>;
using Vector2Dbl = Vector2Template<double>;
using Vector2Flt = Vector2Template<float>;

#include "detail/Vector2.h"