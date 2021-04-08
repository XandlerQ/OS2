
#include <iostream>
#include <vector>
#include "BiCGSTAB.h"


int main()
{
	std::vector<double> test_vals({ 9, 3, 1, 1, 11, 2, 1, 2, 1, 10, 2, 2, 1, 2, 9, 1, 1, 1, 12, 1, 8, 2, 2, 3, 8 });
	std::vector<int> test_idx({ 0, 3, 4, 6, 1, 2, 3, 6, 1, 2, 3, 0, 1, 2, 3, 4, 0, 3, 4, 6, 5, 0, 1, 4, 6 });
	std::vector<int> test_amounts({ 0,4,8,11,16,20,21,25 });

	std::cout << test_vals.size() << "  " << test_idx.size() << std::endl;

	std::vector<double> b({ 100,100,100,100,100,100,100 });

	CSRMatrix A(7, test_vals, test_idx, test_amounts);

	omp_set_num_threads(8);

	std::cout << A << std::endl;

	std::cout << A.transpose() << std::endl;

	std::vector<double> trace = A.trace();

	for (int i = 0; i < trace.size(); i++)
	{
		std::cout << trace.at(i) << "  ";
	}

	std::cout << std::endl;

	BiCGSTAB solver(A, b);

	std::cout << std::endl << "***** " << omp_get_wtime() << std::endl;

	solver.solve(0.000000001);

	std::cout << std::endl << "***** " << omp_get_wtime() << std::endl;

	auto res = solver.getSolution();

	for (int i = 0; i < res.size(); i++)
	{
		std::cout << res.at(i) << "  ";
	}

	std::cout << std::endl << solver.getIterCount();


	return 0;

}