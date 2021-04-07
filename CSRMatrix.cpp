#include "CSRMatrix.h"

#include <iostream>
#include <iomanip>
#include <algorithm>


CSRMatrix::CSRMatrix():
	f_size(0)
{}

CSRMatrix::CSRMatrix(unsigned int p_size, const std::vector<double>& p_Mat, const std::vector<int>& p_idx, const std::vector<int>& p_amts)
{
	if (p_amts.size() != p_size + 1 || p_Mat.size() != p_idx.size() || p_Mat.size() > p_size * p_size)
		throw(std::exception("yikies!"));
	
	f_size = p_size;
	f_Mat = p_Mat;
	f_indexes = p_idx;
	f_amounts = p_amts;
}

CSRMatrix::CSRMatrix(unsigned int p_size, int matsize, int idxsize, int amountssize)
{
	f_size = p_size;
	f_Mat.resize(matsize);
	f_indexes.resize(idxsize);
	f_amounts.resize(amountssize);
}

CSRMatrix::CSRMatrix(const CSRMatrix& p_M):
	f_size(p_M.f_size),
	f_Mat(p_M.f_Mat),
	f_indexes(p_M.f_indexes),
	f_amounts(p_M.f_amounts)
{}

CSRMatrix CSRMatrix::operator* (double p_val) const
{
	CSRMatrix res(*this);
	
	std::for_each(res.f_Mat.begin(), res.f_Mat.end(), [p_val](double& element)
		{
			element *= p_val;
		});

	return res;
}

CSRMatrix& CSRMatrix::operator= (const CSRMatrix& p_M)
{
	f_size = p_M.f_size;

	f_Mat.clear();
	f_Mat.shrink_to_fit();

	f_indexes.clear();
	f_indexes.shrink_to_fit();

	f_amounts.clear();
	f_amounts.shrink_to_fit();

	f_Mat.assign(p_M.f_Mat.begin(), p_M.f_Mat.end());
	f_indexes.assign(p_M.f_indexes.begin(), p_M.f_indexes.end());
	f_amounts.assign(p_M.f_amounts.begin(), p_M.f_amounts.end());

	return *this;
}

CSRMatrix operator* (const double p_val, CSRMatrix& p_M)
{
	CSRMatrix res(p_M);

	std::for_each(res.f_Mat.begin(), res.f_Mat.end(), [p_val](double& element)
		{
			element *= p_val;
		});

	return res;
}



int CSRMatrix::Get_size() const
{
	return f_size;
}

int CSRMatrix::Get_amount() const
{
	return f_amounts.back();
}

std::ostream& operator<<(std::ostream& p_out, const CSRMatrix& p_M)
{
	std::vector<int>::const_iterator itr_amounts(p_M.f_amounts.begin());
	std::vector<int>::const_iterator itr_indexes(p_M.f_indexes.begin());
	std::vector<double>::const_iterator itr_values(p_M.f_Mat.begin());
	unsigned prev_idx;

	for (unsigned row = 0; row < p_M.f_size; row++)
	{
		prev_idx = -1;
		unsigned valCurRow = *(itr_amounts + 1) - *itr_amounts;

		for (unsigned col = 0; col < valCurRow; col++)
		{
			for (unsigned zeros = prev_idx + 1; zeros < *itr_indexes; zeros++)
			{
				p_out << std::setw(6) << std::right << "0";
			}
			p_out << std::setw(6) << std::right << *itr_values;
			prev_idx = *itr_indexes;
			itr_values++;
			itr_indexes++;
		}

		for (unsigned zeros = prev_idx + 1; zeros < p_M.f_size; zeros++)
		{
			p_out << std::setw(6) << std::right << "0";
		}

		p_out << std::endl << std::endl;

		itr_amounts++;
	}

	return p_out;
}


std::vector<double> CSRMatrix::operator* (const std::vector<double>& p_vec)
{
	std::vector<double> res;
	res.reserve(f_size);
	res.resize(f_size);

	if (f_size != p_vec.size())
		throw(std::exception("yikies!"));

	std::vector<int>::const_iterator itr_amounts(f_amounts.begin());
	std::vector<int>::const_iterator itr_indexes(f_indexes.begin());
	std::vector<double>::const_iterator itr_values(f_Mat.begin());

//#pragma omp parallel for
	for (int row = 0; row < f_size; row++)
	{
		unsigned valCurRow = *(itr_amounts + 1) - *itr_amounts;

		double elem = 0;

		for (unsigned col = 0; col < valCurRow; col++)
		{
			
			elem += p_vec.at(*itr_indexes) * *itr_values;

			itr_values++;
			itr_indexes++;
		}

		res.at(row) = elem;

		itr_amounts++;
	}



	return res;
}

CSRMatrix CSRMatrix::transpose() const
{
	CSRMatrix res(f_size, f_Mat.size(), f_indexes.size(), f_amounts.size());

	for (int i = 0; i < f_indexes.size(); ++i) {
		++res.f_amounts.at(f_indexes.at(i) + 1);
	}
	
	for (int i = 2; i < f_amounts.size(); i++)
	{
		res.f_amounts.at(i) += res.f_amounts.at(i - 1);
	}

	std::vector<double>::iterator matIter = res.f_Mat.begin();
	std::vector<int>::iterator idxIter = res.f_indexes.begin();

	for (int i = 0; i < f_size; i++)
	{
		for (int atIdx = 0; atIdx < f_indexes.size(); atIdx++)
		{
			if (f_indexes.at(atIdx) == i)
			{

				*matIter = f_Mat.at(atIdx);
				matIter++;

				int atRow = 0;
				while (atIdx + 1 > f_amounts.at(atRow + 1))
				{
					atRow++;
				}

				*idxIter = atRow;
				idxIter++;

			}
		}
	}

	return res;
}

std::vector<double> CSRMatrix::trace()
{
	std::vector<double> trace(f_size, 0);

	for (int atIdx = 0; atIdx < f_indexes.size(); atIdx++)
	{
		int atRow = 0;
		
		while (atIdx + 1 > f_amounts.at(atRow + 1))
		{
			atRow++;
		}

		if (f_indexes.at(atIdx) == atRow)
		{
			trace[atRow] = f_Mat.at(atIdx);
		}
	}

	return trace;
}
