
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

    //std::cout << problem.getA();

	while (threads != 0)
	{
		std::cout << "Input thread number >>>";


		std::cin >> threads;



		omp_set_num_threads(threads);

        

		double timeStart = omp_get_wtime();
		//clock_t start = clock();
		problem.solve(0.00001);
		//clock_t end = clock();
		double timeEnd = omp_get_wtime();

		auto res = problem.getSolution();

		for (int i = 0; i < res.size(); i++)
		{
			std::cout << res.at(i) << std::endl;
		}

        

		//std::cout << std::endl << "/-/-/-/-/-/-/-/" << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
		

        std:: cout << std::cout << "Precision : " << problem.evaluateSolution();

        std::cout << std::endl << "				*****             " << timeEnd - timeStart << std::endl;

        std::cout << std::endl << problem.getIterCount() << std::endl;
	}
	return 0;

}
/*
#include <iostream>
#include <vector>
#include <ctime>
#include "BiCGSTAB.h"
#include <omp.h>

static bool test;


int main()
{
    omp_set_dynamic(0);
    srand(time(NULL));

    //std::cout << "Testing mode? >>>";

    //std::cin >> test;

    //BiCGSTAB problem("C:\Users\Admin\source\repos\oper_sis\oper_sis\A.txt", "C:\Users\Admin\source\repos\oper_sis\oper_sis\b.txt", test);

    while (threads != 0)
    {
        std::cout << "Input thread number >>>";


        std::cin >> threads;



        //omp_set_num_threads(threads);

        double timeStart = omp_get_wtime();
        //clock_t start = clock();
        //clock_t end = clock();

#pragma omp parallel for num_threads(threads)

        for (int i = 0; i < 99999999; i++)
            double a = 1. / (rand()%1 + 0.1);

        double timeEnd = omp_get_wtime();
        std::cout << std::endl << "                             " << timeEnd - timeStart << std::endl;

        //    double timeStart = omp_get_wtime();
        //    //clock_t start = clock();
        //    problem.solve(0.001);
        //    //clock_t end = clock();
        //    double timeEnd = omp_get_wtime();

        //    auto res = problem.getSolution();

        //    for (int i = 0; i < res.size(); i++)
        //    {
        //        std::cout << res.at(i) << std::endl;
        //    }

        //    std::cout << std::endl << problem.getIterCount();

        //    //std::cout << std::endl << "/-/-/-/-/-/-/-/" << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
        //    std::cout << std::endl << "                             " << timeEnd - timeStart << std::endl;

    }
    return 0;
}*/