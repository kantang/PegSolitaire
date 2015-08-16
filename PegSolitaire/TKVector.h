//TODO: TASK 1.1 a vector class which can store any type and have a customizable dimensions


#pragma once
#include <cassert>
#include "Matrix.h"
#include "TKSparseMatrix.h"
#include "stdafx.h"
#include <cstring>

template <typename T>
class TKVector
{
public:
	TKVector(int length = default_length);
	TKVector(const TKVector<T>& src);
	TKVector(TKVector<T>&& src);
	~TKVector();

	inline int size()const{ return vector_length; };

	//overloaded operators on class
	TKVector<T>& operator=(TKVector<T>rhs);
	inline T operator[](unsigned int index) const{ return cells[index]; };
	inline T& operator[](unsigned int index){ return cells[index]; };
	
	//plus every element of the shorter vector to the longer vector.
	template <typename E>
	friend TKVector<E> operator+(const TKVector<E>& a, const TKVector<E>& b);

	//calculate the 'dot' multiply value.
	template <typename E>
	friend E operator*(const TKVector<E>& a, const TKVector<E>& b);

	//TODO: a matrix * vector overload.
	template <typename E>
	friend TKVector<E> operator*(const Matrix<E>& a, const TKVector<E>& b);


	template <typename E>
	friend ostream& operator<<(ostream& ostr, const TKVector<E>& vector);

	//compare each element in the vector if the size is the same.
	template <typename E>
	friend bool operator==(const TKVector<E>& a, const TKVector<E>& b);

	//constant 
	static const int default_length = 0;

protected:
	int vector_length;
	T* cells;
};
#include "TKVector.inl"

