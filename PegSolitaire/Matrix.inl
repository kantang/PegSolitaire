#pragma once
#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix(int row, int col) :
nb_rows(row), nb_columns(col) {
	cells = new T[nb_rows * nb_columns];
	memset(cells, 0, (nb_rows * nb_columns * sizeof(T)));
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& src) :
nb_rows(src.nb_rows), nb_columns(src.nb_columns)
{
	const int TL = nb_rows * nb_columns;
	cells = new T[TL];
	memcpy_s(cells, (TL * sizeof(T)), src.cells, (TL * sizeof(T)));
	//memcpy(cells, src.cells, TL * sizeof(T)); // same but less safe than memcpy_s
}
template <typename T>
Matrix<T>::Matrix(Matrix<T>&& src)
{
	nb_rows = src.nb_rows;
	nb_columns = src.nb_columns;

	cells = src.cells;

	src.cells = nullptr;
}

template <typename T>
Matrix<T>::~Matrix() {
	delete[] cells;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> rhs)
{
	std::swap(nb_rows, rhs.nb_rows);
	std::swap(nb_columns, rhs.nb_columns);
	std::swap(cells, rhs.cells);

	return *this;
}

// friend functions

template <typename E>
ostream& operator<<(ostream& ostr, const Matrix<E>& mtx) {
	for (int i = 0; i < mtx.nb_rows * mtx.nb_columns; ++i) {
		if (i % mtx.nb_columns == 0 && i != 0) {
			ostr << std::endl;
		}
		ostr << mtx.cells[i] << ",";
	}
	ostr << std::endl;
	return ostr;
}

template <typename E>
Matrix<E> operator*(const Matrix<E>& a, const Matrix<E>& b) {

	assert(a.nb_columns == b.nb_rows);

	Matrix<E> result(a.nb_rows, b.nb_columns);

	for (int i = 0; i < a.nb_rows; ++i) {
		for (int j = 0; j < b.nb_columns; ++j) {
			for (int k = 0; k < a.nb_columns; ++k) {
				result.cells[i * result.nb_columns + j] += a.cells[i * a.nb_columns + k] * b.cells[k * b.nb_columns + j];
			}
		}
	}
	return result;
}