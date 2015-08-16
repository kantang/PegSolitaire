// PegSolitaire.cpp : Defines the entry point for the console application.
// This is Kan Tang's coursework.
// Every thing that the course work demands will be marked in TODO list to make marking easier.
// main function is messy because it's all testing code. Again, to ease marking procedure.
// following list shows which class relate to the corresponding task
// 1.1 TKVector
// 1.2 PegBoard
// 1.3 PegBoard
// 1.4 PegBoard
// 1.5 PegBoard
// 1.6 PegSolitaireGame
// 2.1 TKSparseMatrix
// 2.2 TKPagoda
// 2.3 TKPagoda
// 2.4 TKFileHandler
// 2.5 TKFileHandler
// 2.6 TKPagoda

#include "stdafx.h"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <windows.h>

#include "TKVector.h"
#include "Matrix.h"
#include "PegBoard.h"
#include "PegSolitaireGame.h"
#include "TKSparseMatrix.h"
#include "TKPagoda.h"
using namespace std;

//code below is from online, only for performance checking.
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}
//code above is from online, only for performance checking.
// website http://stackoverflow.com/questions/1739259/how-to-use-queryperformancecounter;

int _tmain(int argc, _TCHAR* argv[])
{

	srand((unsigned int)time(NULL));
	
	cout << "Welcome to Kan Tang's course work, student number 140553695." << endl;
	cout << "input the number of the task you want to check according to the list below." << endl;
	cout << "task 1.1 is implemented but ignored here." << endl;
	while (1)
	{
		cout << "input the number of the task you want to check according to the list below." << endl;
		cout << "0 : task 1.2, output a shape for standard english board, which is stored as boolean." << endl;
		cout << "1 : task 1.3, output a state vector for standard start of the english board." << endl;
		cout << "2 : task 1.4, output a state matrix for standard start of the english board." << endl;
		cout << "3 : task 1.5, output a jump matrix for standard english board." << endl;
		cout << "4 : task 1.6, play with the english board, good luck and have fun!" << endl;
		cout << "5 : task 2.1, out put the 3 vectors for the yale sparse matrix. of the given jump matrix." << endl;
		cout << "6 : task 2.2, task 2.4, generate pagoda function NOT using multithread and write it to file pagoda.txt ." << endl;
		cout << "7 : task 2.3, show the result of testing pagoda function" << endl;
		cout << "8 : task 2.5, read the file just generated or pre-saved named pagoda.txt and varify it." << endl;
		cout << "9 : task 2.6, generate pagoda function using multithread and write it to file pagoda.txt ." << endl;
		int input = -1;
		try{
			cin >> input;
			if (input < 0 && input > 9)
				throw exception("invalid input");
		}
		catch (exception &e)
		{
			std::cerr << e.what() << "\nPlease try again\n";
		}
		PegBoard eng_board = PegSolitaireGame::create_eng_board();
		eng_board.create_board_index_matrix();
		PegSolitaireGame new_game;
		TKPagoda new_pagoda;
		int number = 0;
		int batch_size = 5;
		TKFileHandler file_handle("pagoda.txt");
		switch (input)
		{
		case 0:
			cout << PegBoard::board_shape_matrix_output(eng_board.get_board_shape_matrix()) << endl;
			break;
		case 1:
			cout << PegBoard::status_vector(eng_board.get_board_status_matrix(), eng_board.get_board_index_vector());
			break;
		case 2:
			cout << PegBoard::board_status_matrix_output(eng_board.get_board_status_matrix(), eng_board.get_board_shape_matrix()) << endl;
			break;
		case 3:
			cout << PegBoard::jump_matrix_output(eng_board.create_jump_matrix()) << endl;
			break;
		case 4:
			new_game.start_game();
			break;
		case 5:
			cout << (TKSparseMatrix::transfer_from(eng_board.create_jump_matrix())).output() << endl;
			break;
		case 6:
			cout << "input the number of pagoda you want to create, note it take approx. 15s to create 200 valid pagoda (single threaded) that can prove some insovable." << endl;
			cin >> number;
			StartCounter();
			cout << "finding pagoda..." << endl; 
			TKPagoda::create_pagoda_to_file(eng_board, number,"pagoda.txt");
			cout << "task finished, took " << GetCounter() << " ms\nto produce " << number << " unique pagoda\n";
			break;
		case 7:
			cout << "input the number of tries you want to run, note it take approx. 12s to run 50,000 test" << endl;
			cin >> number;
			StartCounter();
			TKPagoda::test_finding_pagoda(eng_board,number);
			cout << "task finished, took " << GetCounter() << " ms\n";
			break;
		case 8:
			file_handle = TKFileHandler("pagoda.txt");
			file_handle.verify_correctness();
			break;
		case 9:
			cout << "input the number of pagoda you want to create, note it take approx. 32s to create 200 valid pagoda (multi threaded) that can prove some insovable." << endl;
			cin >> number;
			cout << "input the number of batch you want to have in one thread, suggest 5" << endl;
			cin >> batch_size;
			cout << "creating pagoda functions using multithread, check your cpu usage!" << endl;
			StartCounter();
			new_pagoda = TKPagoda();
			new_pagoda.generate_valid_pagoda_to_file_threaded(eng_board, number, "pagoda.txt", batch_size);
			cout << "task finished, took " << GetCounter() << " ms\n";
			break;
		default:
			break;

		}
	}
	return 0;
}

