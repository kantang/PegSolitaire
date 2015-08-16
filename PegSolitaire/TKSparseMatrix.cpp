#include "stdafx.h"
#include "TKSparseMatrix.h"
#include "TKVector.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

TKSparseMatrix::TKSparseMatrix(int row, int col)
{
	nb_rows = row;
	nb_columns = col;
	cells = new int[nb_rows + 1];
	cell_size = nb_rows + 1;
	//when initialized, every element in the matrix is default 0, so a and ja cells are empty, only ia cells have columns+1 number of zeros inside
	a_cells.start_index = 0;
	a_cells.end_index = 0;
	ia_cells.start_index = 0;
	ia_cells.end_index = nb_rows;
	ja_cells.start_index = 0;
	ja_cells.end_index = 0;
}

TKSparseMatrix::TKSparseMatrix(const TKSparseMatrix& src)
{
	nb_rows = src.get_nb_cols();
	nb_columns = src.get_nb_rows();
	cell_size = src.get_cell_size();
	cells = new int[cell_size];
	memcpy_s(cells, (cell_size * sizeof(int)), src.cells, (cell_size * sizeof(int)));

	a_cells = src.a_cells;
	ia_cells = src.ia_cells;
	ja_cells = src.ja_cells;
}

TKSparseMatrix::~TKSparseMatrix()
{
}

TKSparseMatrix& TKSparseMatrix::operator=(const TKSparseMatrix& rhs)
{
	if (this == &rhs) return (*this);

	// release old memory
	delete[] cells;

	nb_rows = rhs.get_nb_cols();
	nb_columns = rhs.get_nb_rows();
	cell_size = rhs.get_cell_size();
	cells = new int[cell_size];
	memcpy_s(cells, (cell_size * sizeof(int)), rhs.cells, (cell_size * sizeof(int)));

	a_cells = rhs.a_cells;
	ia_cells = rhs.ia_cells;
	ja_cells = rhs.ja_cells;

	return *this;
}

ostream& operator<<(ostream& ostr, const TKSparseMatrix mtx)
{

	ostr << TKSparseMatrix::transfer_from(mtx);
	return ostr;
}

TKSparseMatrix operator*(const TKSparseMatrix a, const TKSparseMatrix b)
{
	return TKSparseMatrix::transfer_from(TKSparseMatrix::transfer_from(a) * TKSparseMatrix::transfer_from(b));
}

Matrix<int> TKSparseMatrix::transfer_from(const TKSparseMatrix src)
{
	//create the pointers of the 3 arrays of yale sparse matrix
	int *a = src.cells;
	int a_size = src.a_cells.end_index - src.a_cells.start_index + 1;
	int *ia = src.cells + a_size;
	int ia_size = src.ia_cells.end_index - src.ia_cells.start_index + 1;
	int *ja = src.cells + a_size + ia_size;
	int ja_size = src.ja_cells.end_index - src.ja_cells.start_index + 1;

	Matrix<int> mtx(src.get_nb_rows(), src.get_nb_cols());

	int a_index = 0;
	for (int current_row = 0; current_row < ia_size - 1; current_row++)
	{
		for (int elements_in_row = ia[current_row + 1] - ia[current_row]; elements_in_row > 0; elements_in_row--)
		{
			mtx(ja[a_index], current_row) = a[a_index];
			a_index++;
		}
	}
	return mtx;
}

TKSparseMatrix TKSparseMatrix::transfer_from(const Matrix<int> src)
{
	//create 3 vectors according to the rule of yale sparse matrix
	vector<int> a(0);
	vector<int> ia(0);
	vector<int> ja(0);
	int current_j = -1;
	for (int j = 0; j < src.get_nb_rows(); j++)
	{
		for (int i = 0; i < src.get_nb_cols(); i++)
		{
			if (src(i, j) != 0)
			{
				if (j == (current_j + 1))
				{
					current_j = j;
					ia.push_back(a.size());
				}
				a.push_back(src(i, j));
				ja.push_back(i);
			}

		}
		if (j == (current_j + 1))
		{
			current_j = j;
			ia.push_back(a.size());
		}
	}
	ia.push_back(a.size());

	//use those 3 vectors to initialise the sparse matrix we need
	TKSparseMatrix sp_mtx(src.get_nb_rows(), src.get_nb_cols());
	delete[] sp_mtx.cells;

	sp_mtx.cell_size = a.size() + ia.size() + ja.size();
	sp_mtx.cells = new int[sp_mtx.cell_size];
	sp_mtx.a_cells.start_index = 0;
	sp_mtx.a_cells.end_index = 0;
	sp_mtx.ia_cells.start_index = 0;
	sp_mtx.ia_cells.end_index = 0;
	sp_mtx.ja_cells.end_index = sp_mtx.cell_size - 1;
	for (int i = 0; i < sp_mtx.cell_size; i++)
	{
		if (i < (int)a.size())
		{
			sp_mtx.cells[i] = a[i];
			sp_mtx.a_cells.end_index = i;
			sp_mtx.ia_cells.start_index = i + 1;
		}
		else if (i < (int)(a.size() + ia.size()))
		{
			sp_mtx.ia_cells.end_index = i;
			sp_mtx.cells[i] = ia[i - a.size()];
			sp_mtx.ja_cells.start_index = i + 1;
		}
		else
			sp_mtx.cells[i] = ja[i - a.size()-ia.size()];
	}
	return sp_mtx;
}

int TKSparseMatrix::operator()(unsigned int x, unsigned int y)const
{
	int *a = cells;
	int a_size = a_cells.end_index - a_cells.start_index + 1;
	int *ia = cells + a_size;
	int ia_size = ia_cells.end_index - ia_cells.start_index + 1;
	int *ja = cells + a_size + ia_size;
	int ja_size = ja_cells.end_index - ja_cells.start_index + 1;

	int result = 0;

	for (int i = ia[y]; i < ia[y + 1]; i++)
	{
		result += ja[i] == x ? a[i] : 0 ;
	}

	return result;
}

string TKSparseMatrix::output()
{
	string str;
	str.append("array a  : ");
	for (int i = a_cells.start_index; i < a_cells.end_index; i++)
		str.append(to_string(cells[i]) + ", ");
	str.append("\narray ia  : ");
	for (int i = ia_cells.start_index; i < ia_cells.end_index; i++)
		str.append(to_string(cells[i]) + ", ");
	str.append("\narray ja  : ");
	for (int i = ja_cells.start_index; i < ja_cells.end_index; i++)
		str.append(to_string(cells[i]) + ", ");
	return str;
}


TKVector<int> operator*(const TKSparseMatrix& a, const TKVector<int>& b)
{
	int *a0 = a.cells;
	int a_size = a.a_cells.end_index - a.a_cells.start_index + 1;
	int *ia = a.cells + a_size;
	int ia_size = a.ia_cells.end_index - a.ia_cells.start_index + 1;
	int *ja = a.cells + a_size + ia_size;
	int ja_size = a.ja_cells.end_index - a.ja_cells.start_index + 1;

	TKVector<int> result_vec(ia_size - 1);
	//retrieve each non zero element in the sparse matrix, multiply with the corresponding number in the vector, and put that into sum of the row.
	for (int i = 0; i < ia_size - 1; i++)
	{
		for (int j = 0; j < ia[i + 1] - ia[i]; j++)
		{
			//ia[i] + j is the current index of the element in both a array and ja array. ja array have the y coordinate which is the corresponding index of the vector.
			result_vec[i] += (a0[ia[i] + j] * b[ja[ia[i] + j]]);
		}
	}

	return result_vec;
}
