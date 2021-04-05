#include "BiCGSTAB.h"
#include <math.h>


BiCGSTAB::BiCGSTAB()
{}

BiCGSTAB::BiCGSTAB(const CSRMatrix& p_A, const std::vector<double>& p_b):
	f_A(p_A),
	f_b(p_b),
	f_x(p_b.size(), 0.5),
	f_counter(0),
	f_alpha(0),
	f_beta(0)
{
	f_size = p_b.size();
	f_r.resize(f_size);
	f_p.resize(f_size);
	f_z.resize(f_size);
	f_s.resize(f_size);
}

void BiCGSTAB::solve(double p_precision)
{
	init();

	std::vector<double> prevSolution(f_size, 0);
	std::vector<double> prevP;
	std::vector<double> prevR;

	//double precision = 100 * sqrt(vectorScalarMultpl(vectorSubstract(f_A * f_x, f_b), vectorSubstract(f_A * f_x, f_b))) / sqrt(vectorScalarMultpl(f_b,f_b));

	for (int i = 0; i < 10; i++)
	{
/*#pragma omp parallel sections
		{
#pragma omp section
			{*/
				f_alpha = vectorScalarMultpl(f_p, f_r) / vectorScalarMultpl(f_s, f_A * f_z);
			/*}
#pragma omp section
			{*/
				prevSolution = f_x;
				f_x = vectorAdd(prevSolution, vectorMultiply(f_z, f_alpha));
			/*}
#pragma omp section
			{*/
				prevR = f_r;
				f_r = vectorSubstract(f_r, vectorMultiply(f_A * f_z, f_alpha));
			/*}
#pragma omp section
			{*/
				prevP = f_p;
				f_p = vectorSubstract(f_p, vectorMultiply(f_A.transpose() * f_s, f_alpha));
			/*}
		}*/

		f_beta = vectorScalarMultpl(f_p, f_r) / vectorScalarMultpl(prevP, prevR);
/*#pragma omp parallel sections
		{
#pragma omp section
			{*/
				f_z = vectorAdd(f_r, vectorMultiply(f_z, f_beta));
			/*}
#pragma omp section
			{*/
				f_s = vectorAdd(f_p, vectorMultiply(f_s, f_beta));
			/*}
		}*/
		f_counter++;

		//precision = 100 * sqrt(vectorScalarMultpl(vectorSubstract(f_A * f_x, f_b), vectorSubstract(f_A * f_x, f_b))) / sqrt(vectorScalarMultpl(f_b, f_b));
	}
	
}

std::vector<double> BiCGSTAB::getSolution()
{
	return f_x;
}

unsigned long BiCGSTAB::getIterCount()
{
	return f_counter;
}

void BiCGSTAB::init()
{
	std::vector<double> tech = f_A * f_x;

	f_r = vectorSubstract(f_b, tech);

	f_p.assign(f_r.begin(), f_r.end());
	f_z.assign(f_r.begin(), f_r.end());
	f_s.assign(f_r.begin(), f_r.end());
}

double BiCGSTAB::vectorScalarMultpl(const std::vector<double>& p_a, const std::vector<double>& p_b)
{
	double res = 0;

	//#pragma omp parallel for
	for (int i = 0; i < p_a.size(); i++)
	{
		//#pragma omp atomic
		res += p_a.at(i) * p_b.at(i);
	}

	return res;
}

std::vector<double> BiCGSTAB::vectorSubstract(const std::vector<double>& p_a, const std::vector<double>& p_b)
{
	std::vector<double> res(p_a.size()); 

//#pragma omp parallel for
	for (int w = 0; w < p_a.size(); w++)
		res.at(w) = p_a.at(w) - p_b.at(w);

	return res;
}

std::vector<double> BiCGSTAB::vectorAdd(const std::vector<double>& p_a, const std::vector<double>& p_b)
{
	std::vector<double> res(p_a.size());

//#pragma omp parallel for
	for (int w = 0; w < p_a.size(); w++)
		res.at(w) = p_a.at(w) + p_b.at(w);

	return res;
}

std::vector<double> BiCGSTAB::vectorMultiply(const std::vector<double>& p_a, double p_omega)
{
	std::vector<double> res(p_a.size());

//#pragma omp parallel for
	for (int w = 0; w < p_a.size(); w++)
		res.at(w) = p_a.at(w) * p_omega;

	return res;

}


