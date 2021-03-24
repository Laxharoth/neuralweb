#include <fstream>
#define CAST2CHARPTR(x) reinterpret_cast<char*>(&x)

namespace bytes2array
{
	double** file2array(const char* filename, int &outer,int &inner);
	void freearray(double** arr,int n_rows);
}