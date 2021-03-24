#include "bytestoarray.hpp"

namespace bytes2array
{
	double** file2array(const char* filename, int &n_rows,int &n_cols)
	{
		double** arr{};

		int n{};
		
		std::ifstream file(filename, std::ios::binary|std::ios::in);
		//find the number of rows
		file.read(CAST2CHARPTR(n),sizeof(int));
		n_rows = n;
		//find the number of columns
		file.read(CAST2CHARPTR(n),sizeof(int));
		n_cols = n;

		arr = new double*[n_rows];
		for (unsigned int i = 0; i < n_rows; ++i)
		{
			arr[i] = new double[n_cols];
			for (unsigned int j = 0; j < n_cols; ++j)
				file.read(CAST2CHARPTR(arr[i][j]),sizeof(double));;
		}
		return arr;
	}

	void freearray(double** arr,int n_rows)
	{
		for (unsigned int i = 0; i < n_rows; ++i)
		{
			delete[] arr[i];
		}
		delete[] arr;
	}
}