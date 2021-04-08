
#include <iostream>
#include <vector>
#include <ctime>
#include "BiCGSTAB.h"

static bool test;


int main()
{
	srand(time(NULL));

	std::cout << "Testing mode? >>>";

	std::cin >> test;

	BiCGSTAB problem("D:\\screens\\Study\\ня\\work2\\opersis2\\A.txt", "D:\\screens\\Study\\ня\\work2\\opersis2\\b.txt", test);

	unsigned threads = 1;

	while (threads != 0)
	{
		std::cout << "Input thread number >>>";


		std::cin >> threads;



		omp_set_num_threads(threads);



		double timeStart = omp_get_wtime();
		//clock_t start = clock();
		problem.solve(0.001);
		//clock_t end = clock();
		double timeEnd = omp_get_wtime();

		auto res = problem.getSolution();

		for (int i = 0; i < res.size(); i++)
		{
			std::cout << res.at(i) << std::endl;
		}

		std::cout << std::endl << problem.getIterCount();

		//std::cout << std::endl << "/-/-/-/-/-/-/-/" << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
		std::cout << std::endl << "				*****             " << timeEnd - timeStart << std::endl;

	}
	return 0;

}