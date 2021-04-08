
#include <iostream>
#include <vector>
#include <ctime>
#include "BiCGSTAB.h"


int main()
{
	srand(time(NULL));

	BiCGSTAB problem("D:\\screens\\Study\\ня\\work2\\opersis2\\A.txt", "D:\\screens\\Study\\ня\\work2\\opersis2\\b.txt");

	omp_set_num_threads(8);

	//std::cout << problem.getA() << std::endl;

	//std::cout << problem.getA().transpose() << std::endl;

	std::vector<double> trace = problem.getA().trace();

	for (int i = 0; i < trace.size(); i++)
	{
		std::cout << trace.at(i) << "  ";
	}

	std::cout << std::endl;

	
	
	

	//double timeStart = omp_get_wtime();
	clock_t start = clock();
	problem.solve(0.001);
	clock_t end = clock();

	std::cout << "/-/-/-/-/-/-/-/" << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
	//double timeEnd = omp_get_wtime();

	//std::cout << std::endl << "				*****             " << timeEnd - timeStart << std::endl;

	auto res = problem.getSolution();

	for (int i = 0; i < res.size(); i++)
	{
		std::cout << res.at(i) << "  ";
	}

	std::cout << std::endl << problem.getIterCount();


	return 0;

}