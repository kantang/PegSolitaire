#pragma once
#include "Matrix.h"
#include "TKVector.h"
#include "PegBoard.h"
#include <vector>
#include <mutex>
#include "TKFileHandler.h"
#include <thread>

#define THREAD_NUMBER 8

class TKPagoda
{
public:
	TKPagoda();
	TKPagoda(const TKPagoda& src);
	~TKPagoda();
	
	//to prove the given start/end state insolvable.
	bool prove_insolvable(const PegBoard& board, const TKVector<int>& start_status, const TKVector<int>& end_status);
	//to find out if the pagoda function is already in set
	static bool pagoda_in_set(const TKVector<int>& pagoda, const vector<TKVector<int>>& pagoda_function_set);
	//getter
	vector<TKVector<int>> get_p_function_set()const{ return pagoda_function_set; };
	//generate valid pagoda function using the board.
	TKVector<int> generate_valid_pagoda(const PegBoard& board);
	//create number of pagoda function and write it into the file with name :file_name
	static void create_pagoda_to_file(const PegBoard& board, int number,const string& file_name);
	//TODO:TASK 2.2 generate pagoda using boad and end state.
	static TKVector<int> generate_pagoda(PegBoard board, const TKVector<int>& end_status);
	//try to apply pagoda and see if proves insolvable
	static bool pagoda_proves_insolvable(const TKVector<int>& start_status, const TKVector<int>& end_status, const TKVector<int>& pagoda);
	//try to see if a pagoda is valid according to the jump matrix
	static int valid_pagoda(const Matrix<int>& jump_matrix, const TKVector<int>& pagoda);
	//TODO: TASK 2.3 test how good the algorithm is at finding pagoda.
	static void test_finding_pagoda(PegBoard board, int test_number);
	//generate a random status of peg number = peg_counter; and number of holes = hole_counter;
	static TKVector<int> generate_random_status(const int& peg_counter, const int& hole_counter);
	//TODO: TASK 2.6 try to generate pagoda function with multi-threading
	void generate_valid_pagoda_to_file_threaded(const PegBoard& board, int number, const string& file_name, int batch_size);
	//create batch of jobs for a single thread to increase efficiency
	void generate_valid_pagoda_threaded_safe(const PegBoard& board, std::mutex &function_set_mutex, int thread_id, int number_of_pagoda, int batch_size);

protected:
	vector<TKVector<int>> pagoda_function_set;
	vector<TKVector<int>> start_status_set;
	vector<TKVector<int>> end_status_set;
	
	
};

