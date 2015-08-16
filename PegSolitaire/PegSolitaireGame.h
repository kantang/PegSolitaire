//this class contains everything related to the game task of 1.6.
//TODO: TASK 1.6

#pragma once
#pragma warning( disable : 4290 )
#include "PegBoard.h"
#include <vector>
#include <regex>

//game end type.
#define GAME_OVER_WIN 0
#define GAME_OVER_LOST -1
#define GAME_OVER_RESTART -2
#define GAME_OVER_QUIT -3
#define GAME_NOT_OVER 1

//game valid input type
#define INPUT_END_GAME 0;
#define INPUT_UNDO 1;
#define INPUT_JUMP 2;

struct jump_move
{
	int x;
	int y;
	char direction;
};

class PegSolitaireGame
{
public:
	PegSolitaireGame();
	~PegSolitaireGame();
	//to start the game, initialise it with english board by default, however, the start status/end status/board shape can be customized
	void start_game();
	void set_initial_board(PegBoard new_board){ initial_board = new_board; };
	static PegBoard create_eng_board();
	static PegBoard create_eu_board();
protected:
	PegBoard board;
	PegBoard initial_board;
	//try to find out if it's eng game state.
	int is_end_game();
	//begin a new game
	void begin_new_game();
	vector<int> read_user_input(string str) throw (invalid_argument);
	bool try_move_peg(const vector<int>& input);
};

