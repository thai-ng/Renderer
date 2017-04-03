#pragma once

#include <algorithm>
#include <vector>
#include <utility>

template <typename T>
class Lerp
{
public:
	Lerp<T>(T beginVal1, T endVal1, T beginVal2, T endVal2)
	{
		steps = static_cast<int>(endVal1 - beginVal1);
		auto deltaVal2 = endVal2 - beginVal2;
		if (steps != 0)
		{
			slope = static_cast<double>(deltaVal2) / static_cast<double>(steps);
		}
		else
		{
			slope = 0;
		}

		start1 = beginVal1;
		start2 = beginVal2;
	}
	typedef typename std::pair<T, double> value_t;
/*

	typedef typename std::vector<value_t>::iterator iterator;
	typedef typename std::vector<value_t>::const_iterator const_iterator;

	iterator begin() { return steps.begin(); }
	const_iterator begin() const { return steps.begin(); }
	const_iterator cbegin() const { return steps.cbegin(); }

	iterator end() { return steps.end(); }
	const_iterator end() const { return steps.end(); }
	const_iterator cendn() const { return steps.cend(); }
*/
	auto operator[](int i) const
	{
		if (i > steps)
		{
			throw std::invalid_argument("Accessing lerp value out of bound.");
		}
		auto current1 = start1 + i;
		auto current2 = start2 + static_cast<double>(i)*slope;
		return std::make_pair(current1, current2);
	}

	auto size() const
	{
		return steps;
	}


private:
	double slope;
	T start1;
	int steps;
	double start2;
};
