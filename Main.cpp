#include <stdio.h>
#include "main_header.h"
#include <iostream>



int main()
{
   double* tech_arr;
   /*
    CSRMatrix B;

    std::cin >> B;

    CSRMatrix A;

    std::cin >> A;*/

    /*Vector v_1;
    printf("\nInput vector 1:\n");
    std::cin >> v_1;

    v_1[3] = 3;
    
    std::cout << v_1;*/

    Vector v_1;
    Vector v_2;
    Vector v_3;
    Vector v_res;

    printf("\nInput vector 1:\n");
    std::cin >> v_1;
    v_1 = v_1;
    printf("\nInput vector 2:\n");
    std::cin >> v_2;
    printf("\nInput vector 3:\n");
    std::cin >> v_3;

    printf("\nTrying to know digit 5 of vector 1:\n");
    try
    {
        std::cout << v_1[5];
    }
    catch (OutOfRangeException & ex)
    {
        std::cout << ex.what() << std::endl << "Trying to know digit " << ex.GetTrying() << " of vector sized by " << ex.GetSize() << "..." << std::endl;
    }


    std::cout << "Vector 1:" << v_1 << std::endl << "Vector 2:" << v_2 << std::endl << "Vector 3:" << v_3 << std::endl;

    CSRMatrix A;

    printf("\nInput matrix A:\n");

    std::cin >> A;

    printf("\nMatrix A:\n");
    std::cout << A;

    printf("\nTrying to calculate v_res = (-1.5 * A * (v_1 + 2.1 * v_2)) + v_3 * -2\n");
    try
    {
        v_res = (-1.5 * A * (v_1 + 2.1 * v_2)) + v_3 * -2;
    }
    catch (IncompatibleDimException & ex)
    {
        std::cout << ex.what() << std::endl << "Matrix : " << ex.GetSA1() << " x " << ex.GetSA2() << "; Vector : [" << ex.GetSB2() << "]..." << std::endl;
    }

    printf("\nResult\nv_res: ");
    std::cout << v_res;

    tech_arr = (double*)v_1;

    return 0;
}