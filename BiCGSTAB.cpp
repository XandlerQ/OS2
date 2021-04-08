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

BiCGSTAB::BiCGSTAB(const std::string & p_pathA, const std::string & p_pathb):
	f_size(0)
{
	setA(p_pathA);
	setb(p_pathb);
}

BiCGSTAB::BiCGSTAB(std::string&& p_pathA, std::string&& p_pathb):
	f_size(0)
{
	setA(p_pathA);
	setb(p_pathb);
}


void BiCGSTAB::setA(std::string&& p_path)
{
	std::vector<double> mat;

	std::vector<int> indexes;
	std::vector<int> amounts;
	std::vector<std::tuple<int, int, double>> tupleVector;



	std::string tmp;

	std::ifstream file_stream(p_path);

	if (!file_stream.is_open())
	{
		std::cout << "ERROR" << std::endl;
		throw(std::exception());
	}

	while (std::getline(file_stream, tmp))
	{
		std::regex numRegEx("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");

		std::sregex_token_iterator cutIter(
			tmp.cbegin(),
			tmp.cend(),
			numRegEx
		);

		int row = std::stoi(*cutIter);
		cutIter++;

		int col = std::stoi(*cutIter);
		cutIter++;

		double num = std::stod(*cutIter);

		tupleVector.push_back(
			std::make_tuple(row, col, num)
		);
	}

	std::sort(
		tupleVector.begin(),
		tupleVector.end(),
		[](const auto& a, const auto& b)
		{
			if (std::get<0>(a) == std::get<0>(b))
				return std::get<1>(a) < std::get<1>(b);
			return std::get<0>(a) < std::get<0>(b);
		}

	);

	int prev_row = -1;



	for (int i = 0; i < tupleVector.size(); i++)
	{
		mat.push_back(std::get<2>(tupleVector[i]));
		indexes.push_back(std::get<1>(tupleVector[i]) - 1);

		if (std::get<0>(tupleVector[i]) != prev_row)
		{
			amounts.push_back(i);
			prev_row = std::get<0>(tupleVector[i]);
		}
	}

	amounts.push_back(mat.size());

	f_A = CSRMatrix(amounts.size() - 1, mat, indexes, amounts);
	f_size = amounts.size() - 1;

}

void BiCGSTAB::setA(const std::string& p_path)
{
	std::string path = p_path;

	setA(std::move(path));
}

CSRMatrix BiCGSTAB::getA() const
{
	return f_A;
}

void BiCGSTAB::setb(std::string&& p_path)
{
	if (f_size == 0)
		throw(std::exception());

	std::vector<double> numbers;

	std::ifstream file_stream(p_path);

	if (!file_stream.is_open())
	{
		throw(std::exception());
	}

	std::string tmp;

	std::getline(file_stream, tmp);

	std::regex numRegEx("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");

	std::transform(std::sregex_token_iterator{ tmp.cbegin(), tmp.cend(), numRegEx },
				{},
				std::back_inserter(numbers),
				[](const auto& val)
				{
					return std::stod(val.str());
				}
	); 

	f_b = numbers;
}

void BiCGSTAB::setb(const std::string& p_path)
{
	std::string path = p_path;

	setb(std::move(path));
}

std::vector<double> BiCGSTAB::getb() const
{
	return f_b;
}

void BiCGSTAB::solve(double p_precision)
{
	
	f_counter = 0;

	f_x = f_A.trace();

	std::vector<double> r = vectorSubstract(f_b, f_A * f_x);

	std::vector<double> r_0 = r;

	std::vector<double> h;

	std::vector<double> s;

	std::vector<double> t;

	long double alpha = 1;

	long double omega = 1;

	long double rho = 1;

	long double rho_prev = 1;

	long double beta;

	std::vector<double> v(f_size, 0);

	std::vector<double> p(f_size, 0);

	for (int i = 0; i < 9999; i++)
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


