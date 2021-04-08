#pragma once
#include "CSRMatrix.h"
#include <regex>
#include <fstream>
#include <tuple>


class BiCGSTAB
{
private:

	CSRMatrix f_A;
	std::vector<double> f_b;
	std::vector<double> f_x;
	int f_size;
	unsigned long f_counter;

	bool test;

public:

	BiCGSTAB();

	BiCGSTAB(const CSRMatrix& p_A, const std::vector<double>& p_b, bool test);

	BiCGSTAB(const std::string& p_pathA, const std::string& p_pathb);

	BiCGSTAB(std::string&& p_pathA, std::string&& p_pathb, bool test);

	void setA(std::string&& p_path);

	void setA(const std::string& p_path);

	CSRMatrix getA() const;

	void setb(std::string&& p_path);

	void setb(const std::string& p_path);

	std::vector<double> getb() const;

	void solve(double p_precision);
	
	std::vector<double> getSolution();

	unsigned long getIterCount();

	double vectorScalarMultpl(const std::vector<double>&, const std::vector<double>&);

	std::vector<double> vectorSubstract(const std::vector<double>&, const std::vector<double>&);

	std::vector<double> vectorAdd(const std::vector<double>&, const std::vector<double>&);

	std::vector<double> vectorMultiply(const std::vector<double>&, double p_omega);

	std::vector<double> find_r_(const std::vector<double>&);
};