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
		auto deltaVal1 = endVal1 - beginVal1;
		auto deltaVal2 = endVal2 - beginVal2;
		auto slope = static_cast<double>(deltaVal2) / static_cast<double>(deltaVal1);

		auto currentVal1 = beginVal1;
		auto currentVal2 = static_cast<double>(beginVal2);

		std::generate_n(std::back_inserter(steps),
			deltaVal1 + 1,
			[&currentVal1, &currentVal2, slope]
		{
			auto result = std::make_pair(currentVal1, currentVal2);
			++currentVal1;
			currentVal2 += slope;
			return result;
		});
	}

	typedef typename std::pair<T, double> value_t;

	typedef typename std::vector<value_t>::iterator iterator;
	typedef typename std::vector<value_t>::const_iterator const_iterator;

	iterator begin() { return steps.begin(); }
	const_iterator begin() const { return steps.begin(); }
	const_iterator cbegin() const { return steps.cbegin(); }

	iterator end() { return steps.end(); }
	const_iterator end() const { return steps.end(); }
	const_iterator cendn() const { return steps.cend(); }

	auto operator[](int i) const
	{
		return steps[i];
	}

	auto size() const
	{
		return steps.size();
	}


private:
	std::vector<value_t> steps;
};
