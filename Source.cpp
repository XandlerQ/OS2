
#include <iostream>
#include <vector>
#include "BiCGSTAB.h"


int main()
{
	std::vector<double> test_vals({ 1,-3,5,4,1 });
	std::vector<int> test_idx({ 0,2,1,2,3 });
	std::vector<int> test_amounts({ 0,1,2,3,5 });

	std::vector<double> b({ 1,8,-1,4 });

	CSRMatrix A(4, test_vals, test_idx, test_amounts);

	

	std::cout << A << std::endl;

	std::cout << A.transpose() << std::endl;

	std::vector<double> trace = A.trace();

	for (int i = 0; i < trace.size(); i++)
	{
		std::cout << trace.at(i) << "  ";
	}

	std::cout << std::endl;

	BiCGSTAB solver(A, b);

	solver.solve(0.1);

	auto res = solver.getSolution();

	for (int i = 0; i < res.size(); i++)
	{
		std::cout << res.at(i) << "  ";
	}

	std::cout << std::endl << solver.getIterCount();

	return 0;

}