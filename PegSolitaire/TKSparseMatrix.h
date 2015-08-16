//TODO: TASK 2.1 create a Sparse matrix type;
#pragma once
#include "Matrix.h"

template<typename T>
class TKVector;

struct cell_range
{
	int start_index;
	int end_index;
};


class TKSparseMatrix :
	public Matrix<int>
{
public:
	//initiallizer
	TKSparseMatrix(int nb_rows = default_nb_rows, int nb_columns = default_nb_columns);
	//copy constructor
	TKSparseMatrix(const TKSparseMatrix& src);
	~TKSparseMatrix();
	//operator overloading
	TKSparseMatrix& operator=(const TKSparseMatrix& rhs);
	inline int get_cell_size()const{ return cell_size; };
	//transfer a sparse matrix into a normal matrix
	static Matrix<int> transfer_from(const TKSparseMatrix src);
	//transfer a normal matrix into a yeal sparse matrix
	static TKSparseMatrix transfer_from(const Matrix<int> src);
	//retrieve element
	int operator()(unsigned int x, unsigned int y)const override;
	//out put the 3 vectors
	string output();
	//using get the product of a matrix and a vector
	friend TKVector<int> operator*(const TKSparseMatrix& a, const TKVector<int>& b);

protected:
	cell_range a_cells;
	cell_range ia_cells;
	cell_range ja_cells;
	int cell_size;

};

