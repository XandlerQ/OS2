#include "BiCGSTAB.h"
#include <math.h>


BiCGSTAB::BiCGSTAB()
{}

BiCGSTAB::BiCGSTAB(const CSRMatrix& p_A, const std::vector<double>& p_b):
	f_A(p_A),
	f_b(p_b),
	f_size(p_b.size()),
	f_counter(0)
{}

void BiCGSTAB::solve(double p_precision)
{
	
	f_counter = 0;

	f_x = f_A.trace();

	std::vector<double> r = vectorSubstract(f_b, f_A * f_x);

	std::vector<double> r_0 = r;

	std::vector<double> h;

	std::vector<double> s;

	std::vector<double> t;

	double alpha = 1;

	double omega = 1;

	double rho = 1;

	double rho_prev = 1;

	double beta;

	std::vector<double> v(f_size, 0);

	std::vector<double> p(f_size, 0);

	for (int i = 0; i < 99999; i++)
	{
		rho_prev = rho;

		rho = vectorScalarMultpl(r_0, r);

		beta = (rho / rho_prev) * (alpha / omega);

		p = vectorAdd(r,
			vectorMultiply(
				vectorSubstract(p,
					vectorMultiply(v,
						omega)),
				beta));

		v = f_A * p;

		alpha = rho / vectorScalarMultpl(r_0, v);

		h = vectorAdd(f_x, vectorMultiply(p, alpha));

		if (sqrt(
			vectorScalarMultpl(
				vectorSubstract(f_b, f_A * h),
				vectorSubstract(f_b, f_A * h))) < p_precision)
		{
			f_x = h;
			break;
		}

		s = vectorSubstract(r, vectorMultiply(v, alpha));

		t = f_A * s;

		omega = vectorScalarMultpl(t, s) / vectorScalarMultpl(t, t);

		f_x = vectorAdd(h, vectorMultiply(s, omega));

		if (sqrt(
			vectorScalarMultpl(
				vectorSubstract(f_b, f_A * f_x),
				vectorSubstract(f_b, f_A * f_x))) < p_precision)
			break;

		r = vectorSubstract(s, vectorMultiply(t, omega));

		f_counter++;
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

double BiCGSTAB::vectorScalarMultpl(const std::vector<double>& p_a, const std::vector<double>& p_b)
{
	double res = 0;

	#pragma omp parallel for
	for (int i = 0; i < p_a.size(); i++)
	{
		#pragma omp atomic
		res += p_a.at(i) * p_b.at(i);
	}

	return res;
}

std::vector<double> BiCGSTAB::vectorSubstract(const std::vector<double>& p_a, const std::vector<double>& p_b)
{
	std::vector<double> res(p_a.size()); 

#pragma omp parallel for
	for (int w = 0; w < p_a.size(); w++)
		res.at(w) = p_a.at(w) - p_b.at(w);

	return res;
}

std::vector<double> BiCGSTAB::vectorAdd(const std::vector<double>& p_a, const std::vector<double>& p_b)
{
	std::vector<double> res(p_a.size());

#pragma omp parallel for
	for (int w = 0; w < p_a.size(); w++)
		res.at(w) = p_a.at(w) + p_b.at(w);

	return res;
}

std::vector<double> BiCGSTAB::vectorMultiply(const std::vector<double>& p_a, double p_omega)
{
	std::vector<double> res(p_a.size());

#pragma omp parallel for
	for (int w = 0; w < p_a.size(); w++)
		res.at(w) = p_a.at(w) * p_omega;

	return res;

}

std::vector<double> BiCGSTAB::find_r_(const std::vector<double>& p_v)
{
	std::vector<double> res(p_v.size(), 1);
	bool isSuccessful = false;

	for(int at = 0; at < p_v.size(); at++)
	{
		if (p_v.at(at) != 0)
		{
			res.at(at) = p_v.at(at);
			isSuccessful = true;
			break;
		}
	}

	if (!isSuccessful)
	{
		throw(std::exception("Yikies!"));
	}

	return res;
}


