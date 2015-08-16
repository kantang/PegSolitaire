#pragma once
#include "TKVector.h"

template <typename T>
TKVector<T>::TKVector(int length) :
vector_length(length)
{
	cells = new T[vector_length];
	memset(cells, 0, (vector_length * sizeof(T)));
}

template <typename T>
TKVector<T>::TKVector(const TKVector<T>& src) :
vector_length(src.size())
{
	cells = new T[vector_length];
	memcpy_s(cells, (vector_length * sizeof(T)), src.cells, (vector_length * sizeof(T)));
}

template <typename T>
TKVector<T>::TKVector(TKVector<T>&& src)
{
	vector_length = src.vector_length;
	cells = src.cells;
	src.cells = nullptr;
}

template <typename T>
TKVector<T>& TKVector<T>::operator=(TKVector<T> rhs)
{
	std::swap(cells, rhs.cells);
	std::swap(vector_length, rhs.vector_length);

	return *this;
}

template <typename E>
TKVector<E> operator+(const TKVector<E>& a, const TKVector<E>& b)
{
	TKVector<E> result = (a.size() > b.size() ? a : b);
	int smaller_size = a.size() < b.size() ? a.size() : b.size();
	const TKVector<E> &smaller_vector = (a.size() > b.size() ? a : b);
	for (int i = 0; i < smaller_size; i++)
		result.cells[i] += smaller_vector.cells[i];
	return result;
}

template <typename T>
TKVector<T>::~TKVector()
{
	delete[] cells;
}

template <typename E>
E operator*(const TKVector<E>& a, const TKVector<E>& b)
{
	E result = NULL;
	for (int i = a.size()  > b.size() ? (b.size() - 1) : (a.size() - 1); i >= 0; i--)
		result += (a[i] * b[i]);

	return result;
}

template <typename E>
TKVector<E> operator*(const Matrix<E>& a, const TKVector<E>& b)
{
	assert(a.get_nb_cols() == b.size());

	TKVector<E> result(a.get_nb_rows());

	for (int j = 0; j < a.get_nb_rows(); j++)
	{
		for (int i = 0; i < a.get_nb_cols(); i++)
		{
			result[j] += a(j, i) * b[i];
		}
	}

	return result;
}


template <typename E>
ostream& operator<<(ostream& ostr, const TKVector<E>& vector)
{
	for (int i = 0; i < vector.vector_length; ++i) {
		ostr << vector.cells[i] << ", ";
	}
	ostr << std::endl;
	return ostr;
}

template <typename E>
bool operator==(const TKVector<E>& a, const TKVector<E>& b)
{
	if (a.size() != b.size()) return false;
	return memcmp(a.cells, b.cells, a.size()) == 0 ? true : false;
}

