#include "stdafx.h"
#include "PegBoard.h"
#include <sstream>

PegBoard::PegBoard(unsigned int width, unsigned int height) :
board_width(width),
board_height(height),
peg_hole_counter(0),
board_shape_matrix(width, height),
board_status_matrix(width, height),
board_index_matrix(width, height),
board_index_vector(0)
{
	for (unsigned int j = 0; j < width; j++)
	{
		for (unsigned int i = 0; i < height; i++)
		{
			board_shape_matrix(i, j) = false;
			board_status_matrix(i, j) = false;
			board_index_matrix(i, j) = -1;
		}
	}
}

PegBoard::PegBoard(const PegBoard& src):
board_width(src.get_board_width()),
board_height(src.get_board_height())
{
	board_shape_matrix = src.get_board_shape_matrix();
	board_status_matrix = src.get_board_status_matrix();
	board_index_matrix = src.get_board_index_matrix();
	board_index_vector = src.get_board_index_vector();
	peg_hole_counter = src.get_peg_hole_counter();
}

PegBoard::~PegBoard()
{
}

void PegBoard::set_peg_holes(const unsigned int x, const unsigned int y, const bool is_board)
{
	if (board_shape_matrix(x, y) != is_board)
	{
		board_shape_matrix(x, y) = is_board;
		if (is_board == true)
		{
			peg_hole_counter++;
		}
		else
		{
			peg_hole_counter--;
		}
	}
}

bool PegBoard::is_coordinate_in_bound(unsigned int x, unsigned int y)
{
	bool is_in_bound = false;
	if (x < board_width && y < board_height)
	{
		is_in_bound = board_shape_matrix(x, y);
	}
	return is_in_bound;
}

int PegBoard::move_peg(const unsigned int x, const unsigned int y, const char direction)
{
	int error_code = PEG_MOVE_SUCCESSFUL;
	switch (direction)
	{
	case PEG_MOVE_DIRECTION_UP:
		error_code = move_peg(x, y, x - 1, y, x - 2, y);
		break;
	case PEG_MOVE_DIRECTION_DOWN:
		error_code = move_peg(x, y, x + 1, y, x + 2, y);
		break;
	case PEG_MOVE_DIRECTION_LEFT:
		error_code = move_peg(x, y, x, y - 1, x, y - 2);
		break;
	case PEG_MOVE_DIRECTION_RIGHT:
		error_code = move_peg(x, y, x, y + 1, x, y + 2);
		break;
	default:
		error_code = PEG_MOVE_ERROR_UNDEFINED_BEHAVIOUR;
		break;
	}
	return error_code;
}

int PegBoard::undo_move_peg(const unsigned int x, const unsigned int y, const char direction)
{
	int error_code = PEG_MOVE_SUCCESSFUL;
	switch (direction)
	{
	case PEG_MOVE_DIRECTION_UP:
		error_code = undo_move_peg(x, y, x - 1, y, x - 2, y);
		break;
	case PEG_MOVE_DIRECTION_DOWN:
		error_code = undo_move_peg(x, y, x + 1, y, x + 2, y);
		break;
	case PEG_MOVE_DIRECTION_LEFT:
		error_code = undo_move_peg(x, y, x, y - 1, x, y - 2);
		break;
	case PEG_MOVE_DIRECTION_RIGHT:
		error_code = undo_move_peg(x, y, x, y + 1, x, y + 2);
		break;
	default:
		error_code = PEG_MOVE_ERROR_UNDEFINED_BEHAVIOUR;
		break;
	}	
	return error_code;
}

//when using the << operator to out-put, out-put the status on board.
ostream& operator<<(ostream& ostr, const PegBoard& board)
{
	ostr << "    ";

	for (unsigned int i = 0; i < board.get_board_width(); i++)
		ostr << i << "  ";
	
	ostr << endl << endl;

	for (unsigned int i = 0; i < board.get_board_height(); i++)
	{
		ostr << i << "   ";
		for (unsigned int j = 0; j < board.get_board_width(); j++)
		{
			if (!board.get_peg_holes(i, j))
				ostr << " , ";
			else if (board(i,j))
				ostr << "1, ";
			else
				ostr << "0, ";
		}
		ostr << endl << endl;
	}
	ostr << endl;
	return ostr;
}

PegBoard& PegBoard::operator=(const PegBoard& rhs)
{
	if (this == &rhs) 
		return (*this);

	board_width = rhs.get_board_width();
	board_height = rhs.get_board_height();
	board_shape_matrix = rhs.get_board_shape_matrix();
	board_status_matrix = rhs.get_board_status_matrix();
	board_index_matrix = rhs.get_board_index_matrix();
	board_index_vector = rhs.get_board_index_vector();
	peg_hole_counter = rhs.get_peg_hole_counter();

	return *this;
}

string PegBoard::board_shape_matrix_output(const Matrix<bool>& shape_matrix)
{
	string str;
	for (int i = 0; i < shape_matrix.get_nb_rows(); i++)
	{
		for (int j = 0; j < shape_matrix.get_nb_cols(); j++)
		{
			if (!shape_matrix(i, j))
				str.append("F ");
			else
				str.append("T ");
		}
		str.append("\n");
	}
	return str;
}

string PegBoard::board_status_matrix_output(const Matrix<bool>& status_matrix, const Matrix<bool>& shape_matrix)
{
	string str;
	for (int i = 0; i < status_matrix.get_nb_rows(); i++)
	{
		for (int j = 0; j < status_matrix.get_nb_cols(); j++)
		{
			if (!shape_matrix(i, j))
				str.append(" ,");
			else if (status_matrix(i, j))
				str.append("1,");
			else
				str.append("0,");
		}
		str.append("\n");
	}
	return str;
}

string PegBoard::board_index_matrix_output(const Matrix<int>& index_matrix)
{
	string str;
	for (int i = 0; i < index_matrix.get_nb_rows(); i++)
	{
		for (int j = 0; j < index_matrix.get_nb_cols(); j++)
		{
			if (index_matrix(i, j) == -1)
				str.append("    ");
			else if (index_matrix(i, j) < 10)
				str.append(std::to_string(index_matrix(i, j)) + "   ");
			else if (index_matrix(i, j) < 100)
				str.append(std::to_string(index_matrix(i, j)) + "  ");
			else
				str.append(std::to_string(index_matrix(i, j)) + " ");
		}
		str.append("\n");
	}
	str.append("\n");
	return str;
}

Matrix<int> PegBoard::create_jump_matrix_for_each_peg()
{
	vector<TKVector<int>> jump_vector(0);
	for (unsigned int i = 0; i < board_height; i++)
	{
		for (unsigned int j = 0; j < board_width; j++)
		{
			//verify each hole with a peg on board, assign the corresponding value, and store that into a temporary vector.
			if (can_move_peg(i, j, PEG_MOVE_DIRECTION_UP) == 0)
			{
				TKVector<int> current_move(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i - 1, j)] = 1;
				current_move[board_index_matrix(i - 2, j)] = -1;
				jump_vector.push_back(current_move);
			}
			if (can_move_peg(i, j, PEG_MOVE_DIRECTION_DOWN) == 0)
			{
				TKVector<int> current_move(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i + 1, j)] = 1;
				current_move[board_index_matrix(i + 2, j)] = -1;
				jump_vector.push_back(current_move);
			}
			if (can_move_peg(i, j, PEG_MOVE_DIRECTION_LEFT) == 0)
			{
				TKVector<int> current_move(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i, j - 1)] = 1;
				current_move[board_index_matrix(i, j - 2)] = -1;
				jump_vector.push_back(current_move);
			}
			if (can_move_peg(i, j, PEG_MOVE_DIRECTION_RIGHT) == 0)
			{
				TKVector<int> current_move(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i, j + 1)] = 1;
				current_move[board_index_matrix(i, j + 2)] = -1;
				jump_vector.push_back(current_move);
			}
		}
	}
	Matrix<int> jump_matrix(jump_vector.size(), peg_hole_counter);
	//put that vector into the matrix declared above
	for (unsigned int j = 0; j < peg_hole_counter; j++)
	{
		for (unsigned int i = 0; i < jump_vector.size(); i++)
			jump_matrix(i, j) = (jump_vector[i])[j];
	}
	return jump_matrix;
}

Matrix<int> PegBoard::create_jump_matrix()
{
	vector<TKVector<int>> jump_vector(0);
	TKVector<int> current_move;
	for (unsigned int j = 0; j < board_height; j++)
	{
		for (unsigned int i = 0; i < board_width; i++)
		{
			//verify each hole on board, assign the corresponding value, and store that into a temporary vector.
			if (can_move(i, j, PEG_MOVE_DIRECTION_UP))
			{
				current_move = TKVector<int>(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i - 1, j)] = 1;
				current_move[board_index_matrix(i - 2, j)] = -1;
				jump_vector.push_back(current_move);
			}
			if (can_move(i, j, PEG_MOVE_DIRECTION_DOWN))
			{
				current_move = TKVector<int>(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i + 1, j)] = 1;
				current_move[board_index_matrix(i + 2, j)] = -1;
				jump_vector.push_back(current_move);
			}
			if (can_move(i, j, PEG_MOVE_DIRECTION_LEFT))
			{
				current_move = TKVector<int>(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i, j - 1)] = 1;
				current_move[board_index_matrix(i, j - 2)] = -1;
				jump_vector.push_back(current_move);
			}
			if (can_move(i, j, PEG_MOVE_DIRECTION_RIGHT))
			{
				current_move = TKVector<int>(peg_hole_counter);
				current_move[board_index_matrix(i, j)] = 1;
				current_move[board_index_matrix(i, j + 1)] = 1;
				current_move[board_index_matrix(i, j + 2)] = -1;
				jump_vector.push_back(current_move);
			}
		}
	}
	Matrix<int> jump_matrix(jump_vector.size(), peg_hole_counter);
	//put that vector into the matrix declared above
	for (unsigned int j = 0; j < peg_hole_counter; j++)
	{
		for (unsigned int i = 0; i < jump_vector.size(); i++)
			jump_matrix(i, j) = (jump_vector[i])[j];
	}
	return jump_matrix;
}

string PegBoard::jump_matrix_output(Matrix<int> jump_matrix)
{
	string str;
	str.append("      ");
	for (int i = 0; i < jump_matrix.get_nb_rows(); i++)
	{
		if (i < 10)
			str.append(std::to_string(i) + "   ");
		else if (i < 100)
			str.append(std::to_string(i) + "  ");
		else
			str.append(std::to_string(i) + " ");
	}

	str.append("\n");

	for (int j = 0; j < jump_matrix.get_nb_cols(); j++)
	{
		if (j < 10)
			str.append(std::to_string(j) + "  : ");
		else if (j < 100)
			str.append(std::to_string(j) + " : ");
		else
			str.append(std::to_string(j) + ": ");

		for (int i = 0; i < jump_matrix.get_nb_rows(); i++)
		{
			switch (jump_matrix(i, j))
			{
			case 0:
				str.append(" 0  ");
				break;
			case 1:
				str.append(" 1  ");
				break;
			case -1:
				str.append("-1  ");
				break;
			}
		}
		str.append("\n");
	}
	str.append("\n");
	return str;
}

int PegBoard::create_board_index_matrix()
{
	board_index_vector.clear();
	int peg_counter = 0;
	//start from the bottom to the top, as the handout shows.
	for (int i = board_height - 1; i > -1; i--)
	{
		for (unsigned int j = 0; j < board_width; j++)
		{
			if (board_shape_matrix(i,j))
			{
				//store the structure into a vector, to ease iterate step when finding a index's coordinate
				index_coordinate coo;
				coo.x = i;
				coo.y = j;
				board_index_vector.push_back(coo);
				board_index_matrix(i,j) = peg_counter;
				peg_counter++;
			}
			else
			{
				board_index_matrix(i, j) = -1;
			}
		}
	}
	return peg_counter;
}

Matrix<int> PegBoard::create_board_index_matrix(const Matrix<bool>& board_shape_matrix)
{
	Matrix<int> board_index(board_shape_matrix.get_nb_rows(), board_shape_matrix.get_nb_cols());
	int peg_counter = 0;
	for (int i = board_shape_matrix.get_nb_rows() - 1; i > -1; i--)
	{
		for (int j = 0; j < board_shape_matrix.get_nb_cols(); j++)
		{
			if (board_shape_matrix(i, j))
			{
				board_index(i, j) = peg_counter;
				peg_counter++;
			}
			else
			{
				board_index(i, j) = -1;
			}
		}
	}
	return board_index;
}

int PegBoard::move_peg(unsigned int beginning_x, unsigned int beginning_y, unsigned int jumped_x, unsigned int jumped_y, unsigned int destination_x, unsigned int destination_y)
{
	int error_code = can_move_peg(beginning_x, beginning_y, jumped_x, jumped_y, destination_x, destination_y);
	if ( error_code == 0)
	{
		board_status_matrix(beginning_x, beginning_y) = false;
		board_status_matrix(jumped_x, jumped_y) = false;
		board_status_matrix(destination_x, destination_y) = true;
	}
	return error_code;
}

bool PegBoard::can_move(const unsigned int x, const unsigned int y, const char direction)
{
	//if all 3 peg holes invovles is in bound, then can move.
	bool can_move = false;
	switch (direction)
	{
	case PEG_MOVE_DIRECTION_UP:
		can_move = is_coordinate_in_bound(x, y) && is_coordinate_in_bound(x - 1, y) && is_coordinate_in_bound(x - 2, y);
		break;
	case PEG_MOVE_DIRECTION_DOWN:
		can_move = is_coordinate_in_bound(x, y) && is_coordinate_in_bound(x + 1, y) && is_coordinate_in_bound(x + 2, y);
		break;
	case PEG_MOVE_DIRECTION_LEFT:
		can_move = is_coordinate_in_bound(x, y) && is_coordinate_in_bound(x, y - 1) && is_coordinate_in_bound(x, y - 2);
		break;
	case PEG_MOVE_DIRECTION_RIGHT:
		can_move = is_coordinate_in_bound(x, y) && is_coordinate_in_bound(x, y + 1) && is_coordinate_in_bound(x, y + 2);
		break;
	}

	return can_move;
}

int PegBoard::can_move_peg(const unsigned int x, const unsigned int y, const char direction)
{
	int error_code = PEG_MOVE_SUCCESSFUL;
	switch (direction)
	{
	case PEG_MOVE_DIRECTION_UP:
		error_code = can_move_peg(x, y, x - 1, y, x - 2, y);
		break;
	case PEG_MOVE_DIRECTION_DOWN:
		error_code = can_move_peg(x, y, x + 1, y, x + 2, y);
		break;
	case PEG_MOVE_DIRECTION_LEFT:
		error_code = can_move_peg(x, y, x, y - 1, x, y - 2);
		break;
	case PEG_MOVE_DIRECTION_RIGHT:
		error_code = can_move_peg(x, y, x, y + 1, x, y + 2);
		break;
	default:
		error_code = PEG_MOVE_ERROR_UNDEFINED_BEHAVIOUR;
		break;
	}
	return error_code;
}

int PegBoard::can_move_peg(unsigned int beginning_x, unsigned int beginning_y, unsigned int jumped_x, unsigned int jumped_y, unsigned int destination_x, unsigned int destination_y)
{
	int error_code = PEG_MOVE_SUCCESSFUL;
	if (!(is_coordinate_in_bound(beginning_x, beginning_y) && is_coordinate_in_bound(jumped_x, jumped_y) && is_coordinate_in_bound(destination_x, destination_y)))
	{
		error_code = PEG_MOVE_ERROR_OUT_OF_BOUNDS;
	}
	else if (!(board_shape_matrix(beginning_x, beginning_y) && board_shape_matrix(jumped_x, jumped_y) && board_shape_matrix(destination_x, destination_y)))
	{
		error_code = PEG_MOVE_ERROR_OUT_OF_BOUNDS;
	}
	else if (!board_status_matrix(beginning_x, beginning_y))
	{
		error_code = PEG_MOVE_ERROR_NO_PEG_CHOOSEN;
	}
	else if (!board_status_matrix(jumped_x, jumped_y))
	{
		error_code = PEG_MOVE_ERROR_NO_PEG_IN_DIRECTION;
	}
	else if (board_status_matrix(destination_x, destination_y))
	{
		error_code = PEG_MOVE_ERROR_PEG_IN_DESTINATION;
	}
	return error_code;
}

int PegBoard::undo_move_peg(unsigned int beginning_x, unsigned int beginning_y, unsigned int jumped_x, unsigned int jumped_y, unsigned int destination_x, unsigned int destination_y)
{
	int error_code = PEG_MOVE_SUCCESSFUL;

	if (!(is_coordinate_in_bound(beginning_x, beginning_y) && is_coordinate_in_bound(jumped_x, jumped_y) && is_coordinate_in_bound(destination_x, destination_y)))
	{
		error_code = PEG_MOVE_ERROR_OUT_OF_BOUNDS;
	}
	else if (!(board_shape_matrix(beginning_x, beginning_y) && board_shape_matrix(jumped_x, jumped_y) && board_shape_matrix(destination_x, destination_y)))
	{
		error_code = PEG_MOVE_ERROR_OUT_OF_BOUNDS;
	}
	else if (board_status_matrix(beginning_x, beginning_y))
	{
		error_code = PEG_MOVE_UNDO_ERROR_PEG_AT_BEGIN;
	}
	else if (board_status_matrix(jumped_x, jumped_y))
	{
		error_code = PEG_MOVE_UNDO_ERROR_PEG_AT_JUMPED;
	}
	else if (!board_status_matrix(destination_x, destination_y))
	{
		error_code = PEG_MOVE_UNDO_ERROR_NO_PEG_DESTINATION;
	}
	else
	{
		board_status_matrix(beginning_x, beginning_y) = true;
		board_status_matrix(jumped_x, jumped_y) = true;
		board_status_matrix(destination_x, destination_y) = false;
	}
	return error_code;
}

TKVector<int> PegBoard::status_vector(const Matrix<bool>& status_mtx, const vector<index_coordinate>& index_vector)
{
	TKVector<int> status_vector(index_vector.size());

	for (int i = 0; i < status_vector.size(); i++)
		status_vector[i] = status_mtx((index_vector[i]).x, (index_vector[i]).y);

	return status_vector;
}