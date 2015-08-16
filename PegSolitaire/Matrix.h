// This class manages all the matrix that the coursework uses.
// I haven't made change to this file except I overload the braces for element retrieving.

#pragma once
#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
class Matrix
{
public:
	Matrix(int nb_rows = default_nb_rows, int nb_columns = default_nb_columns);
	Matrix(const Matrix<T>& src);
	Matrix(Matrix<T>&& src);
	virtual ~Matrix();
	// overloaded operators on class
	virtual Matrix<T>& operator = (Matrix<T> rhs);
	template <typename E>
	friend ostream& operator<<(ostream& ostr, const Matrix<E>& mtx);
	template <typename E>
	friend Matrix<E> operator*(const Matrix<E>& a, const Matrix<E>& b);
	virtual int get_nb_rows() const { return nb_rows; }
	virtual int get_nb_cols() const { return nb_columns; }
	//TODO: Element retrieving using overload braces.
	virtual T operator()(unsigned int x, unsigned int y) const{ return cells[x * nb_columns + y]; };
	virtual T& operator()(unsigned int x, unsigned int y){ return cells[x * nb_columns + y]; };
	// constant elements
	static const int default_nb_rows = 3;
	static const int default_nb_columns = 3;
protected:
	T* cells;
	int nb_rows;
	int nb_columns;
};
#include "Matrix.inl"
