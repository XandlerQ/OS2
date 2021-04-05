#pragma once
#include "CSRMatrix.h"


class BiCGSTAB
{
private:

	CSRMatrix f_A;
	std::vector<double> f_b;
	std::vector<double> f_x;
	std::vector<double> f_r;
	std::vector<double> f_p;
	std::vector<double> f_z;
	std::vector<double> f_s;
	double f_alpha;
	double f_beta;
	int f_size;
	unsigned long f_counter;

public:

	BiCGSTAB();

	BiCGSTAB(const CSRMatrix& p_A, const std::vector<double>& p_b);

	void solve(double p_precision);
	
	std::vector<double> getSolution();

	unsigned long getIterCount();

	void init();

	double vectorScalarMultpl(const std::vector<double>&, const std::vector<double>&);

	std::vector<double> vectorSubstract(const std::vector<double>&, const std::vector<double>&);

	std::vector<double> vectorAdd(const std::vector<double>&, const std::vector<double>&);

	std::vector<double> vectorMultiply(const std::vector<double>&, double p_omega);
};