
#include <iostream>
#include <vector>
#include "BiCGSTAB.h"


int main()
{
	std::vector<double> test_vals({ 0.1,0.31,0.251,0.8852 });
	std::vector<int> test_idx({ 0,1,0,1 });
	std::vector<int> test_amounts({ 0,2,4 });

	std::vector<double> b({ 0.05,0.01 });

	CSRMatrix A(2, test_vals, test_idx, test_amounts);

	

	std::cout << A;

	std::cout << A.transpose();

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