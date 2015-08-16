#include "stdafx.h"
#include "TKFileHandler.h"


TKFileHandler::TKFileHandler(const string& file_name)
{
	read_from_file(file_name);
}

TKFileHandler::TKFileHandler(const file_content& content):
board_width(content.board_width),
board_height(content.board_height),
shape_matrix(content.shape_matrix)
{
	for (unsigned int i = 0; i < content.start_status_set.size(); i++)
	{
		start_status_set.push_back(content.start_status_set.at(i));
		end_status_set.push_back(content.end_status_set.at(i));
		pagoda_set.push_back(content.pagoda_set.at(i));
	}
}

TKFileHandler::~TKFileHandler()
{
}

void TKFileHandler::write_to_file(const string& file_name)
{
	string* output = new string();
	Matrix<int> index_matrix = PegBoard::create_board_index_matrix(shape_matrix);
	output->append("WIDTH ");
	output->append(std::to_string(board_width) + "\n");
	output->append("HEIGHT ");
	output->append(std::to_string(board_height) + "\n");
	output->append("BOARD:\n");
	output->append(PegBoard::board_shape_matrix_output(shape_matrix));
	for (unsigned int i = 0; i < start_status_set.size(); i++)
	{
		output->append("START:\n");
		output->append(TKFileHandler::transfer_to_output(index_matrix, start_status_set[i]));
		output->append("END:\n");
		output->append(TKFileHandler::transfer_to_output(index_matrix, end_status_set[i]));
		output->append("PAGODA:\n");
		output->append(TKFileHandler::transfer_to_output(index_matrix, pagoda_set[i]));
	}
	ofstream output_stream;
	output_stream.open(file_name, ios_base::trunc);
	if (!output_stream.is_open())
	{
		delete output;
		return;
	}
	output_stream << *output;
	output_stream.close();
	delete output;
	return;
}

void TKFileHandler::read_from_file(const string& file_name) throw (std::exception)
{

	//read line per line, use regular expression to deal with the format.
	ifstream input_stream;
	try
	{
		input_stream.open(file_name);
		if (!input_stream)
			throw std::exception("Could not open file \n...\n");
	}
	catch (std::exception &ex) {
		std::cerr << ex.what();
	}
	string *input;
	string line = "";
	int counter = 0;
	while (getline(input_stream, line))
	{
		if (counter == 0)
		{
			std::regex e("(WIDTH )(.*)");
			if (!(std::regex_match(line, e)))
				throw std::exception("File format unexpected");
			else
			{
				line.erase(0, 6);
				std::stringstream(line) >> board_width;
			}
		}
		else if (counter == 1)
		{
			std::regex e("(HEIGHT )(.*)");
			if (!(std::regex_match(line, e)))
				throw std::exception("File format unexpected");
			else
			{
				line.erase(0, 7);
				std::stringstream(line) >> board_height;
			}
		}
		else if (counter == 2)
		{
			std::regex e("(BOARD)(.*)");
			if (!(std::regex_match(line, e)))
				throw std::exception("File format unexpected");
			input = new string();
			for (int i = 0; i < board_height; i++)
			{
				getline(input_stream, line);
				input->append(line);
			}
			shape_matrix = TKFileHandler::transfer_to_shape_matrix(input, board_width, board_height);
			delete input;
		}
		else if (((counter - 3) % 3) == 0)
		{
			std::regex e("(START)(.*)");
			if (!(std::regex_match(line, e)))
				throw std::exception("File format unexpected");
			input = new string();
			for (int i = 0; i < board_height; i++)
			{
				getline(input_stream, line);
				input->append(line);
			}
			start_status_set.push_back(TKFileHandler::transfer_to_vector(input, shape_matrix));
			delete input;
		}
		else if (((counter - 3) % 3) == 1)
		{
			std::regex e("(END)(.*)");
			if (!(std::regex_match(line, e)))
				throw std::exception("File format unexpected");
			input = new string();
			for (int i = 0; i < board_height; i++)
			{
				getline(input_stream, line);
				input->append(line);
			}
			end_status_set.push_back(TKFileHandler::transfer_to_vector(input, shape_matrix));
			delete input;
		}
		else if (((counter - 3) % 3) == 2)
		{
			std::regex e("(PAGODA)(.*)");
			if (!(std::regex_match(line, e)))
				throw std::exception("File format unexpected");
			input = new string();
			for (int i = 0; i < board_height; i++)
			{
				getline(input_stream, line);
				input->append(line);
			}
			pagoda_set.push_back(TKFileHandler::transfer_to_vector(input, shape_matrix));
			delete input;
		}
		counter++;
	}
}

void TKFileHandler::add_set(const TKVector<int>& start_status, const TKVector<int>& end_status, const TKVector<int>& pagoda)
{
	start_status_set.push_back(start_status);
	end_status_set. push_back(end_status);
	pagoda_set.push_back(pagoda);
}

string TKFileHandler::transfer_to_output(const Matrix<int>& index_matrix, TKVector<int> vec)
{
	string str;
	for (int i = 0; i <index_matrix.get_nb_rows(); i++)
	{
		for (int j = 0; j < index_matrix.get_nb_cols(); j++)
		{
			if (index_matrix(i, j) != -1)
				str.append(std::to_string(vec[index_matrix(i, j)]) + ",");
			else
				str.append(" ,");
		}
		str.append("\n");
	}
	return str;
}

Matrix<bool> TKFileHandler::transfer_to_shape_matrix(const string* input, int width, int height)
{
	string* a = new string(*(input));
	Matrix<bool> shape_mtx(height,width);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			shape_mtx(i, j) = ((*a)[0] == 'F' ? false : true);
			a->erase(0, 2);
		}
	}
	return shape_mtx;
}

TKVector<int> TKFileHandler::transfer_to_vector(const string* input, const Matrix<bool>& shape_mtx)
{
	string* a = new string(*(input));
	int counter = 0;
	for (int i = 0; i < shape_mtx.get_nb_rows(); i++)
	{
		for (int j = 0; j < shape_mtx.get_nb_cols(); j++)
		{
			if (shape_mtx(i, j))
				counter++;
		}
	}

	TKVector<int> vec(counter);
	Matrix<int> index_mtx = PegBoard::create_board_index_matrix(shape_mtx);
	for (int i = 0; i < index_mtx.get_nb_rows(); i++)
	{
		for (int j = 0; j < index_mtx.get_nb_cols(); j++)
		{
			//TODO: when vec is an array that stores double, a double number will be read here.
			//Since the whole program is using integer for pagoda function, integer will be default to use here.
			std::stringstream(*a) >> vec[index_mtx(i, j)];
			a->erase(0, 2);
		}
	}

	return vec;
}

void TKFileHandler::verify_correctness()
{
	int right_counter = 0;
	for (unsigned int i = 0; i < start_status_set.size(); i++)
	{
		if (TKPagoda::pagoda_proves_insolvable(start_status_set[i], end_status_set[i], pagoda_set[i]))
		{
			right_counter++;
		}
		else
		{
			//TODO: printing every pagoda that does not prove in-feasibility of the start and end configuration.
			cout << "invalid pagoda found, printing the set out\nstart:\n" << start_status_set[i] << "end:\n" << end_status_set[i] << "pagoda:\n" << pagoda_set[i];
		}
	}
		
	std::cout << "verified " << start_status_set.size() << " sets of pagoda sets, " << right_counter << " of them are(is) right." << endl;
}