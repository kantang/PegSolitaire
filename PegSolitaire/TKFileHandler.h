#pragma once
#pragma warning( disable : 4290 )
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Matrix.h"
#include "PegBoard.h"
#include "TKVector.h"
#include <regex>
#include "TKPagoda.h"
using namespace std;

struct file_content
{
	int board_width;
	int board_height;
	Matrix<bool> shape_matrix;
	vector<TKVector<int>> start_status_set;
	vector<TKVector<int>> end_status_set;
	vector<TKVector<int>> pagoda_set;
};

class TKFileHandler
{
public:
	//initialize the file handler with file name. normally used for file reading
	TKFileHandler(const string& file_name);
	//initialize the file handler with structure content, normally used for file writing.
	TKFileHandler(const file_content& content);
	~TKFileHandler();

	//write the data stored in this handler to file
	//TODO: TASK 2.4
	void write_to_file(const string& file_name);
	//read the data stored in file, and transfer it into the valid data, stores in the object
	//TODO: TASK 2.5
	void read_from_file(const string& file_name);

	//add a set of start/end status and a corresponding pagoda function
	void add_set(const TKVector<int>& start_status, const TKVector<int>& end_status, const TKVector<int>& pagoda);
	// transfer the status to output, in order to write to file
	static string transfer_to_output(const Matrix<int>& index_matrix, TKVector<int> status);
	//transfer the raw string into valid data
	static TKVector<int> transfer_from_input(const string input);
	//verify the correctness of read file.
	void verify_correctness();

protected:
	int board_width;
	int board_height;
	Matrix<bool> shape_matrix;
	vector<TKVector<int>> start_status_set;
	vector<TKVector<int>> end_status_set;
	vector<TKVector<int>> pagoda_set;
	static Matrix<bool> transfer_to_shape_matrix(const string* input, int width, int height);
	static TKVector<int> transfer_to_vector(const string* input, const Matrix<bool>& shape_mtx);
	
};

