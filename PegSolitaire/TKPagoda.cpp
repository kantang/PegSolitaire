#include "stdafx.h"
#include "TKPagoda.h"


TKPagoda::TKPagoda()
{
	pagoda_function_set = vector<TKVector<int>>(0);
	start_status_set = vector<TKVector<int>>(0);
	end_status_set = vector<TKVector<int>>(0);

}


TKPagoda::~TKPagoda()
{
}
TKPagoda::TKPagoda(const TKPagoda& src)
{
	pagoda_function_set = src.pagoda_function_set;
	start_status_set = src.start_status_set;
	end_status_set = src.end_status_set;
}

bool TKPagoda::prove_insolvable(const PegBoard& board, const TKVector<int>& start_status, const TKVector<int>& end_status)
{
	int counter = 0;
	bool insolvable = false;
	//search for the exsisting pagoda function set for valid pagoda to prove insolvable.
	for (unsigned int i = 0; i < pagoda_function_set.size(); i++)
	{
		const TKVector<int>& pagoda = pagoda_function_set[i];
		insolvable = TKPagoda::pagoda_proves_insolvable(start_status, end_status, pagoda);
	}
	//if there isn't, try generate valid pagoda to solve this problem.
	while ((!insolvable) && counter < 50)
	{
		TKVector<int> pagoda = TKPagoda::generate_valid_pagoda(board);
		insolvable = pagoda_proves_insolvable(start_status, end_status, pagoda);
		counter++;
	}
	return insolvable;
}

TKVector<int> TKPagoda::generate_valid_pagoda(const PegBoard& board)
{
	TKVector<int> pagoda; 
	int set_size = 0;
	while (true && set_size < 50)
	{
		//create start and end status randomly.
		int rand_peg_num = rand() % (board.get_peg_hole_counter() / 2);
		TKVector<int> start_status = TKPagoda::generate_random_status(board.get_peg_hole_counter() - rand_peg_num, board.get_peg_hole_counter());
		TKVector<int> end_status = TKPagoda::generate_random_status(rand_peg_num, board.get_peg_hole_counter());
		//generate pagoda function using the end status.
		pagoda = TKPagoda::generate_pagoda(board, end_status);
		//applying the pagoda using the start status.
		bool is_insolvable = TKPagoda::pagoda_proves_insolvable(start_status, end_status, pagoda);
		//if proves, put pagoda in set if it's not already in.
		if (is_insolvable)
		{
			if (!TKPagoda::pagoda_in_set(pagoda, pagoda_function_set))
			{
				pagoda_function_set.push_back(pagoda);
				set_size = pagoda_function_set.size();
				cout << "new pagoda found\n" << pagoda << endl;
				cout << "corresponding start status is\n" << start_status << "\nend status is\n" << end_status << endl;
			}
			continue;
		}
	}
	return pagoda;
}

void TKPagoda::generate_valid_pagoda_threaded_safe(const PegBoard& board, std::mutex &function_set_mutex, int thread_id, int number_of_pagoda, int batch_size)
{
	vector<TKVector<int>> start_set(0);
	vector<TKVector<int>> end_set(0);
	vector<TKVector<int>> pagoda_set(0);
	while (true)
	{
		//create start and end status randomly.
		int rand_peg_num = rand() % (board.get_peg_hole_counter() / 2);
		TKVector<int> start_status = TKPagoda::generate_random_status(board.get_peg_hole_counter() - rand_peg_num, board.get_peg_hole_counter());
		TKVector<int> end_status = TKPagoda::generate_random_status(rand_peg_num, board.get_peg_hole_counter());
		//generate pagoda function using the end status.
		TKVector<int> pagoda = TKPagoda::generate_pagoda(board, end_status);
		//applying the pagoda using the start status.
		bool is_insolvable = TKPagoda::pagoda_proves_insolvable(start_status, end_status, pagoda);
		//if proves, put pagoda in set if it's not already in.
		if (is_insolvable)
		{
			start_set.push_back(start_status);
			end_set.push_back(end_status);
			pagoda_set.push_back(pagoda);
			if (pagoda_set.size() == (unsigned int)batch_size)
			{
				function_set_mutex.lock();
				for (int i = 0; i < batch_size; i++)
				{
					if (!TKPagoda::pagoda_in_set(pagoda_set[i], pagoda_function_set))
					{
						pagoda_function_set.push_back(pagoda_set[i]);
						start_status_set.push_back(start_set[i]);
						end_status_set.push_back(end_set[i]);
					}
				}
				if ((int)pagoda_function_set.size() >= number_of_pagoda)
				{
					function_set_mutex.unlock();
					break;
				}
				start_set = vector<TKVector<int>>(0);
				end_set = vector<TKVector<int>>(0);
				pagoda_set = vector<TKVector<int>>(0);
				cout << "thread " << thread_id << " has just finnished a batch\n";
				function_set_mutex.unlock();
			}
		}
	}
}

void TKPagoda::create_pagoda_to_file(const PegBoard& board, int number, const string& file_name)
{
	TKVector<int> start_status;
	TKVector<int> end_status;
	TKVector<int> pagoda;
	file_content content;
	vector<TKVector<int>> start_status_set;
	vector<TKVector<int>> end_status_set;
	vector<TKVector<int>> pagoda_set;
	content.board_height = board.get_board_height();
	content.board_width = board.get_board_width();
	content.shape_matrix = board.get_board_shape_matrix();
	

	while (true && (start_status_set.size() < (unsigned)number))
	{
		TKVector<int> pagoda;
		//create start and end status randomly.
		int rand_peg_num = rand() % (board.get_peg_hole_counter() / 2);
		start_status = TKPagoda::generate_random_status(board.get_peg_hole_counter() - rand_peg_num, board.get_peg_hole_counter());
		end_status = TKPagoda::generate_random_status(rand_peg_num, board.get_peg_hole_counter());
		//generate pagoda function using the end status.
		pagoda = TKPagoda::generate_pagoda(board, end_status);
		//applying the pagoda using the start status.
		bool is_insolvable = TKPagoda::pagoda_proves_insolvable(start_status, end_status, pagoda);
		//if proves, put pagoda in set if it's not already in.
		if (is_insolvable)
		{
			if (!pagoda_in_set(pagoda, pagoda_set))
			{
				start_status_set.push_back(start_status);
				end_status_set.push_back(end_status);
				pagoda_set.push_back(pagoda);
			}
			continue;
		}
	}
	content.start_status_set = start_status_set;
	content.end_status_set = end_status_set;
	content.pagoda_set = pagoda_set;
	TKFileHandler file_handler(content);
	file_handler.write_to_file(file_name);
}

void TKPagoda::generate_valid_pagoda_to_file_threaded(const PegBoard& board, int number, const string& file_name, int batch_size)
{
	std::mutex function_mtx;

	//TODO: TASK 2.6 Multithread using std::thread class, lambda function for thread generation
	auto thread_generation = [&](int thread_id)
	{
		TKPagoda::generate_valid_pagoda_threaded_safe(board, function_mtx, thread_id, number,batch_size);
		cout << "thread " << thread_id << " has just finnished a batch\n";
	};

	vector<std::thread> thread_vec(THREAD_NUMBER);
	for (int i = 0; i < THREAD_NUMBER; i++)
		thread_vec[i] = std::thread(thread_generation, i);

	for (int i = 0; i < THREAD_NUMBER; i++)
		thread_vec[i].join();

	std::cout << start_status_set.size() << " pagoda actually created in total" << endl;
	//write the result to file.
	file_content content;

	content.board_height = board.get_board_height();
	content.board_width = board.get_board_width();
	content.shape_matrix = board.get_board_shape_matrix();
	content.start_status_set = start_status_set;
	content.end_status_set = end_status_set;
	content.pagoda_set = pagoda_function_set;
	TKFileHandler file_handler(content);
	file_handler.write_to_file(file_name);

}

TKVector<int> TKPagoda::generate_pagoda(PegBoard board, const TKVector<int>& end_status)
{
	board.create_board_index_matrix();
	int pagoda_size = board.get_peg_hole_counter();
	Matrix<bool> fixed_mtx(board.get_board_shape_matrix());

	TKVector<int> pagoda(end_status);
	bool is_end = false;

	Matrix<int> jump_mtx;

	jump_mtx = board.create_jump_matrix();


	while (!is_end)
	{
		int error_code = TKPagoda::valid_pagoda(jump_mtx, pagoda);
		if ( error_code == -1)
		{
			for (int k = 0; k < pagoda.size(); k++)
				is_end = pagoda[k] == 0 ? false : true;
			if (!is_end)
				break;
		}
		else
		{
			TKVector<int> index(3);
			index[1] = -1;
			for (int k = 0; k < jump_mtx.get_nb_cols(); k++)
			{
				int a = jump_mtx(error_code, k);
				if (a == -1)
					index[0] = k;
				else if (a == 1)
				{
					if (index[1] != -1)
						index[2] = k;
					else
						index[1] = k;
				}
			}
			index_coordinate x = board.get_board_index_vector()[index[1]];
			index_coordinate y = board.get_board_index_vector()[index[2]];
			index_coordinate z = board.get_board_index_vector()[index[0]];
			
			if (fixed_mtx(x.x, x.y))
			{
				pagoda[index[1]] = pagoda[index[0]] - pagoda[index[2]] + (rand() % 2);
				fixed_mtx(x.x, x.y) = false;
			}
			else if (fixed_mtx(y.x, y.y))
			{
				pagoda[index[2]] = pagoda[index[0]] - pagoda[index[1]] + (rand() % 2);
				fixed_mtx(y.x, y.y) = false;
			}
			else if (fixed_mtx(z.x, z.y))
			{
				pagoda[index[0]] = pagoda[index[2]] + pagoda[index[1]] - (rand() % 2);
				fixed_mtx(z.x, z.y) = false;
			}
			else
			{
				break;
			}
		}
	}
	if (!is_end)
	{
		for (int i = 0; i < pagoda.size(); i++)
			pagoda[i] = 0;
	}
	return pagoda;
}

void TKPagoda::test_finding_pagoda(PegBoard board, int test_number)
{
	vector<TKVector<int>> start_status_set;
	vector<TKVector<int>> end_status_set;
	vector<TKVector<int>> pagoda_set;
	while (test_number > 0)
	{
		//create start and end status randomly.
		int rand_peg_num = rand() % (board.get_peg_hole_counter() / 2);
		TKVector<int> start_status = TKPagoda::generate_random_status(board.get_peg_hole_counter() - rand_peg_num, board.get_peg_hole_counter());
		TKVector<int> end_status = TKPagoda::generate_random_status(rand_peg_num, board.get_peg_hole_counter());
		start_status_set.push_back(start_status);
		end_status_set.push_back(end_status);
		//generate pagoda function using the end status.
		TKVector<int> pagoda = TKPagoda::generate_pagoda(board, end_status);
		//applying the pagoda using the start status.
		bool is_insolvable = TKPagoda::pagoda_proves_insolvable(start_status, end_status, pagoda);
		--test_number;
		//if proves, put pagoda in set if it's not already in.
		if (is_insolvable)
		{
			if (!TKPagoda::pagoda_in_set(pagoda, pagoda_set))
			{
				pagoda_set.push_back(pagoda);
			}
			
			continue;
		}
	}
	int counter = 0;
	cout << pagoda_set.size() << " number of unique pagoda is found that can prove insolvable." << endl;
	for (unsigned int i = 0; i < start_status_set.size(); i++)
	{
		for (unsigned int j = 0; j < pagoda_set.size(); j++)
		{
			if (TKPagoda::pagoda_proves_insolvable(start_status_set[i], end_status_set[i], pagoda_set[j]))
			{
				counter++;
				break;
			}
		}
	}
	double ratio = (double)counter / (double)start_status_set.size();
	cout << counter << " out of " << start_status_set.size() << " board set is proven insolvable. That's " << ratio * 100 << "% of them\n";

}

TKVector<int> TKPagoda::generate_random_status(const int& peg_counter, const int& hole_counter)
{
	TKVector<int> status_vector(hole_counter);
	for (int i = 0; i < peg_counter;)
	{
		int k = rand() % hole_counter;
		if (status_vector[k] == 0)
		{
			status_vector[k] = 1;
			i++;
		}
			
	}
	return status_vector;
}

bool TKPagoda::pagoda_proves_insolvable(const TKVector<int>& start_status, const TKVector<int>& end_status, const TKVector<int>& pagoda)
{
	return (start_status * pagoda - end_status * pagoda) >= 0 ? false : true;
}

int TKPagoda::valid_pagoda(const Matrix<int>& jump_matrix, const TKVector<int>& pagoda)
{
	TKVector<int> result(jump_matrix * pagoda);
	int error_index = -1;
	for (int i = 0; i < jump_matrix.get_nb_rows(); i++)
	{
		if (result[i] < 0)
		{
			error_index = i;
			break;
		}
	}
	return error_index;
}

bool TKPagoda::pagoda_in_set(const TKVector<int>& pagoda,const vector<TKVector<int>>& pagoda_set)
{
	bool in_set = false;
	for (unsigned int i = 0; i < pagoda_set.size(); i++)
	{
		if (pagoda == pagoda_set[i])
		{
			in_set = true;
			break;
		}
	}
	return in_set;
}

