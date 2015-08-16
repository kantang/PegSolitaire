#include "stdafx.h"
#include "PegSolitaireGame.h"
#include <iostream>

using namespace std;

PegSolitaireGame::PegSolitaireGame():
initial_board(create_eng_board()),
board(initial_board)
{
	
}


PegSolitaireGame::~PegSolitaireGame()
{

}

void PegSolitaireGame::start_game()
{
	cout << "Welcome to Peg Solitaire Game" << endl;
	bool end_playing = false;
	while (!end_playing)
	{
		board = initial_board;
		cout << board;
		int end_game = GAME_NOT_OVER;
		vector<jump_move>* all_jumps = new vector<jump_move>;
		while (end_game == GAME_NOT_OVER)
		{
			cout << "Please input the jump you want to make in the format of number_number_direction." << endl;
			cout << "the first number indicates the collum and the second the row, use 'u' 'd' 'l' 'r' for up,down,left and right." << endl;
			cout << "input 'end' to end the game." << endl;
			cout << "input 'restart' to restart the game." << endl;
			cout << "input 'undo' to undo your last move : " << endl;
			string str;
			getline(cin,str);
			vector<int> input;
			//TODO: exception handling!
			try{
				input = read_user_input(str);
			}
			catch (const invalid_argument& iae)
			{
				cout << iae.what() << "\n";
				cout << board;
				continue;
			}
			switch (input[0])
			{
			case 0:
				end_playing = true;
				end_game = GAME_OVER_QUIT;
				break;
			case 1:
				if (all_jumps->size() > 0)
				{
					board.undo_move_peg(all_jumps->back().x, all_jumps->back().y, all_jumps->back().direction);
					all_jumps->pop_back();
					cout << "undo last move successful" << endl;
				}
				else
				{
					cout << "No last move exist" << endl;
				}
				cout << board;
				break;
			case 2:
				if (try_move_peg(input))
				{
					jump_move last_move;
					last_move.x = input[1];
					last_move.y = input[2];
					last_move.direction = (char)input[3];
					all_jumps->push_back(last_move);
					end_game = is_end_game();
				}
				cout << board;
				break;
			case 3:
				end_game = GAME_OVER_RESTART;
				break;
			default:
				break;
			}
		}
		delete all_jumps;
		if (end_game == GAME_OVER_WIN)
			cout << "Congratulations! You win the game!" << endl;
		else if (end_game == GAME_OVER_LOST)
			cout << "No move available, you lost the game!" << endl;
		else if (end_game == GAME_OVER_RESTART)
			cout << "Restarting game..." << endl;
		else if (end_game == GAME_OVER_QUIT)
			cout << "Ending game..." << endl;
		else
			cout << "Ending game." << endl;

	}
}

vector<int> PegSolitaireGame::read_user_input(string str) throw (invalid_argument)
{
	vector<int> input(0);
	if (str == "end")
		input.push_back(0);
	else if (str == "undo")
		input.push_back(1);
	else if (str == "restart")
		input.push_back(3);
	else
	{
		//TODO: Use the regular expression to see if the input is in the format of number-number-(u or d or l or r).
		//also, exception handling.
		std::regex e("(\\d\\W\\d\\W(u|d|r|l))((.|\\n)*)");
		if (std::regex_match(str,e))
		{
			input.push_back(2);
			input.push_back((int)(str[0] - '0'));
			input.push_back((int)(str[2] - '0'));
			input.push_back((int)str[4]);
		}
		else
			throw invalid_argument("undefined input of :\n" + str + "\nplease check your input again and re-input the correct format");
	}
	return input;
}

bool PegSolitaireGame::try_move_peg(const vector<int>& input)
{
	bool moved = false;
	switch (board.move_peg(input[1], input[2], (char)input[3]))
	{
	case PEG_MOVE_ERROR_OUT_OF_BOUNDS:
		cout << "move invalid because out of bounds" << endl;
		break;
	case PEG_MOVE_ERROR_NO_PEG_IN_DIRECTION:
		cout << "move invalid because no peg in direction" << endl;
		break;
	case PEG_MOVE_ERROR_PEG_IN_DESTINATION:
		cout << "move invalid because there is a peg in destination" << endl;
		break;
	case PEG_MOVE_ERROR_NO_PEG_CHOOSEN:
		cout << "move invalid because no peg in choosen coordinate" << endl;
		break;
	case PEG_MOVE_SUCCESSFUL:
		moved = true;
		break;
	default:
		cout << "undefined behaviour" << endl;
	}
	return moved;
}
//TODO: a lamda function!
auto set_peg = [&](unsigned int x, unsigned int y, PegBoard& board, bool is_peg)
{
	board.set_peg_holes(x, y, is_peg);
	board(x, y) = is_peg;
};

PegBoard PegSolitaireGame::create_eu_board()
{
	PegBoard eu_board(7,7);
	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < 7; i++)
		{
			set_peg(i, j, eu_board, true);
		}
	}
	set_peg(0, 0, eu_board, false);
	set_peg(0, 1, eu_board, false);
	set_peg(1, 0, eu_board, false);
	set_peg(6, 0, eu_board, false);
	set_peg(0, 6, eu_board, false);
	set_peg(6, 1, eu_board, false);
	set_peg(1, 6, eu_board, false);
	set_peg(5, 0, eu_board, false);
	set_peg(0, 5, eu_board, false);
	set_peg(6, 6, eu_board, false);
	set_peg(6, 5, eu_board, false);
	set_peg(5, 6, eu_board, false);

	eu_board(3, 3) = false;

	return eu_board;
}

PegBoard PegSolitaireGame::create_eng_board()
{
	PegBoard eng_board(7,7);

	eng_board = create_eu_board();
	
	set_peg(1, 5, eng_board, false);
	set_peg(5, 1, eng_board, false);
	set_peg(1, 1, eng_board, false);
	set_peg(5, 5, eng_board, false);

	return eng_board;
}

int PegSolitaireGame::is_end_game()
{
	int is_end_game = GAME_NOT_OVER;
	board.create_board_index_matrix();
	if (board.create_jump_matrix_for_each_peg().get_nb_rows() == 0)
	{
		int peg_counter = 0;
		for (unsigned int j = 0; j < board.get_board_height(); j++)
		{
			for (unsigned int i = 0; i < board.get_board_width(); i++)
			{
				if (board(i, j))
					peg_counter++;
			}
		}

		if (peg_counter == 1)
		{
			if (board(3, 3))
				is_end_game = GAME_OVER_WIN;
			else
				is_end_game = GAME_OVER_LOST;
		}
		else
			is_end_game = GAME_OVER_LOST;
	}

	return is_end_game;
}

