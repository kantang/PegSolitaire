//This is the class that I spend most of the times on. I create a lot of methods in this class to make things clear and to do each task precisely, although not concisely.

#pragma once
#include "Matrix.h"
#include "TKVector.h"
#include "TKSparseMatrix.h"
#include <vector>
#include <string>

//TODO: define macros for the output to increase readability of the code
#define PEG_MOVE_ERROR_UNDEFINED_BEHAVIOUR -1
#define PEG_MOVE_ERROR_OUT_OF_BOUNDS -2
#define PEG_MOVE_ERROR_NO_PEG_IN_DIRECTION -3
#define PEG_MOVE_ERROR_PEG_IN_DESTINATION -4
#define PEG_MOVE_ERROR_NO_PEG_CHOOSEN -5
#define PEG_MOVE_UNDO_ERROR_PEG_AT_BEGIN -6
#define PEG_MOVE_UNDO_ERROR_PEG_AT_JUMPED -7
#define PEG_MOVE_UNDO_ERROR_NO_PEG_DESTINATION -8
#define PEG_MOVE_SUCCESSFUL 0

#define PEG_MOVE_DIRECTION_UP 'u'
#define PEG_MOVE_DIRECTION_DOWN 'd'
#define PEG_MOVE_DIRECTION_LEFT 'l'
#define PEG_MOVE_DIRECTION_RIGHT 'r'

struct index_coordinate
{
	int x;
	int y;
};

class PegBoard
{
public:
	PegBoard(unsigned int width = default_width, unsigned int height = default_height);
	//copy constructor
	PegBoard(const PegBoard& srs);
	~PegBoard();

	//all the getter and setter method, most of them inline.
	void set_peg_holes(const unsigned int x, const unsigned int y, const bool is_board);
	inline bool get_peg_holes(const unsigned int x, const unsigned int y) const{ return board_shape_matrix(x, y); };
	inline unsigned int get_peg_hole_counter() const { return peg_hole_counter; };
	inline unsigned int get_board_width()const{ return board_width; };
	inline unsigned int get_board_height()const{ return board_height; };
	inline Matrix<bool> get_board_shape_matrix()const{ return board_shape_matrix; };
	inline Matrix<bool> get_board_status_matrix()const{ return board_status_matrix; };
	inline Matrix<int> get_board_index_matrix()const{ return board_index_matrix; };
	inline vector<index_coordinate> get_board_index_vector()const { return board_index_vector; };

	//this method returns true if peg hole(x, y) can move towards given direction if. Only relevant to shape of the board.
	bool can_move(const unsigned int x, const unsigned int y, const char direction);
	//this method returns the value defined in the macro, indicating the possiblity of moving for the given status on the board.
	int can_move_peg(const unsigned int x, const unsigned int y, const char direction);
	//try to move peg in the given coordinate towards the given direction.
	int move_peg(const unsigned int x, const unsigned int y, const char direction);
	//undo the move of the peg.
	int undo_move_peg(const unsigned int x, const unsigned int y, const char direction);

	//TODO: TASK 1.3 be able to use the shape and status matrix to create a status vector.
	static TKVector<int> status_vector(const Matrix<bool>& status_mtx, const vector<index_coordinate>& board_index_vector);
	//return the string for out put of the given matrix of different types.
	static string board_shape_matrix_output(const Matrix<bool>& shape_matrix);
	//TODO: TASK 1.4 print the board state in the shape of given matrix.
	static string board_status_matrix_output(const Matrix<bool>& status_matrix, const Matrix<bool>& shape_matrix);
	static string board_index_matrix_output(const Matrix<int>& index_matrix);
	static string jump_matrix_output(Matrix<int> jump_matrix);

	//create index matrix. not neccessary for each initialising, so made it member method, and also static for use outside the class.
	int create_board_index_matrix();
	static Matrix<int> create_board_index_matrix(const Matrix<bool>& board_shape_matrix);

	//TODO: TASK 1.5 create the jump matrix. the first one can be used to determin end game situation.
	Matrix<int> create_jump_matrix_for_each_peg();
	Matrix<int> create_jump_matrix();

	//overloaded operators
	friend ostream& operator<<(ostream& ostr, const PegBoard& board);
	inline bool operator()(unsigned int x, unsigned int y) const{ return board_status_matrix(x, y); };
	inline bool& operator()(unsigned int x, unsigned int y){ return board_status_matrix(x, y); };
	PegBoard& operator=(const PegBoard& rhs);

	//constants 
	static const int default_width = 7;
	static const int default_height = 7;

protected:
	unsigned int board_width;
	unsigned int board_height;
	unsigned int peg_hole_counter;
	//TODO: TASK 1.2 stored board shape as a matrix of booleans.
	Matrix<bool> board_shape_matrix;
	Matrix<bool> board_status_matrix;
	//store the index of each peg hole in the matrix in 2 different ways. to make things easier.
	Matrix<int> board_index_matrix;
	vector<index_coordinate> board_index_vector;
	//try to move peg from the begnning through jumped to destination
	int move_peg(unsigned int beginning_x, unsigned int beginning_y, unsigned int jumped_x, unsigned int jumped_y, unsigned int destination_x, unsigned int destination_y);
	int undo_move_peg(unsigned int beginning_x, unsigned int beginning_y, unsigned int jumped_x, unsigned int jumped_y, unsigned int destination_x, unsigned int destination_y);
	//whether can be done like that.
	int can_move_peg(unsigned int beginning_x, unsigned int beginning_y, unsigned int jumped_x, unsigned int jumped_y, unsigned int destination_x, unsigned int destination_y);

	//check if the coordinate is in bound whenever want to make a jump at the direction.
	inline bool is_coordinate_in_bound(unsigned int x, unsigned int y);
	
};

