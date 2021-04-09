
#include <iostream>
#include <vector>
#include <ctime>
#include "BiCGSTAB.h"

static bool test;

void printvector(std::vector<double> p_vec)
{
    for (int i = 0; i < p_vec.size(); i++)
    {
        std::cout << p_vec.at(i) << std::endl;
    }
}

int main()
{
	srand(time(NULL));

	std::cout << "Testing mode? >>>";

	std::cin >> test;

    omp_set_num_threads(4);

	BiCGSTAB problem("D:\\screens\\Study\\ня\\work2\\opersis2\\1138_bus.txt", "D:\\screens\\Study\\ня\\work2\\opersis2\\b1138.txt", test);

	while (threads != 0)
	{
		std::cout << "Input thread number >>>";


		std::cin >> threads;



		omp_set_num_threads(threads);

        

		double timeStart = omp_get_wtime();
		problem.solve(0.00001);
		double timeEnd = omp_get_wtime();

		auto res = problem.getSolution();

        printvector(res);

        std:: cout << std::endl << "Precision : " << problem.evaluateSolution();

        std::cout << std::endl << "				*****             " << timeEnd - timeStart << std::endl;

        std::cout << std::endl << problem.getIterCount() << std::endl;
	}
	return 0;

}