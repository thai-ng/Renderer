#pragma once
#include <array>

template <int width, int height, typename T>
class Matrix
{
using row_t = std::array<T, width>;
using col_t = std::array<T, height>;

public:
	row_t getRow(int row)
	{
		
	}
private:
	
	std::array<T, width*height> _data;
};