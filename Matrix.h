#pragma once
#include <array>

template <int width, int height, typename T>
class Matrix
{
	using row_t = std::array<T, width>;
	using col_t = std::array<T, height>;

public:

	template <typename... T>
	Matrix(T... values) 
	{
		_data = {values...};
	}

	template <std::size_t i>
	auto getRow()
	{
		return make_array<width>(std::next(_data.begin(), width*i));
	}

	template <std::size_t col>
	auto getCol()
	{
		return make_array_with_interval<width, height>(std::next(_data.begin(), col));
	}

	template <std::size_t x, std::size_t y>
	auto getElement() const
	{
		return _data[x*width + y];
	}

	void operator*=(Matrix<width, height, T>& other)
	{
		auto row0 = getRow<0>();
		auto row1 = getRow<1>();
		auto row2 = getRow<2>();

		auto otherCol0 = other.getCol<0>();
		auto otherCol1 = other.getCol<1>();
		auto otherCol2 = other.getCol<2>();

		auto elem00 = row0[0] * otherCol0[0] + row0[1] * otherCol0[1] + row0[2] * otherCol0[2];
		auto elem01 = row0[0] * otherCol1[0] + row0[1] * otherCol1[1] + row0[2] * otherCol1[2];
		auto elem02 = row0[0] * otherCol2[0] + row0[1] * otherCol2[1] + row0[2] * otherCol2[2];
		
		auto elem10 = row1[0] * otherCol0[0] + row1[1] * otherCol0[1] + row1[2] * otherCol0[2];
		auto elem11 = row1[0] * otherCol1[0] + row1[1] * otherCol1[1] + row1[2] * otherCol1[2];
		auto elem12 = row1[0] * otherCol2[0] + row1[1] * otherCol2[1] + row1[2] * otherCol2[2];

		auto elem20 = row2[0] * otherCol0[0] + row2[1] * otherCol0[1] + row2[2] * otherCol0[2];
		auto elem21 = row2[0] * otherCol1[0] + row2[1] * otherCol1[1] + row2[2] * otherCol1[2];
		auto elem22 = row2[0] * otherCol2[0] + row2[1] * otherCol2[1] + row2[2] * otherCol2[2];

		_data = { elem00, elem01, elem02,
				  elem10, elem11, elem12,
				  elem20, elem21, elem22 };
	}

private:
	
	std::array<T, width*height> _data;
	
	template <typename Iterator>
	using ValueType = typename std::iterator_traits<Iterator>::value_type;

	template <int Interval, std::size_t... Indices, typename RandomAccessIterator, typename Array = std::array<ValueType<RandomAccessIterator>, sizeof...(Indices)>>
	Array make_array_with_interval(RandomAccessIterator first, std::index_sequence<Indices...>)
	{
		return Array{ { first[Indices * Interval]... } };
	}

	template <std::size_t N, typename RandomAccessIterator>
	std::array<ValueType<RandomAccessIterator>, N> make_array(RandomAccessIterator first)
	{
		return make_array_with_interval<1>(first, std::make_index_sequence<N>());
	}

	template <int Interval, std::size_t N, typename RandomAccessIterator>
	std::array<ValueType<RandomAccessIterator>, N> make_array_with_interval(RandomAccessIterator first)
	{
		return make_array_with_interval<Interval>(first, std::make_index_sequence<N>());
	}
};

template <int width, int height, typename T>
Matrix<width, height, T> operator*(Matrix<width, height, T>& l, Matrix<width, height, T>& r)
{
	auto row0 = l.getRow<0>();
	auto row1 = l.getRow<1>();
	auto row2 = l.getRow<2>();

	auto otherCol0 = r.getCol<0>();
	auto otherCol1 = r.getCol<1>();
	auto otherCol2 = r.getCol<2>();

	auto elem00 = row0[0] * otherCol0[0] + row0[1] * otherCol0[1] + row0[2] * otherCol0[2];
	auto elem01 = row0[0] * otherCol1[0] + row0[1] * otherCol1[1] + row0[2] * otherCol1[2];
	auto elem02 = row0[0] * otherCol2[0] + row0[1] * otherCol2[1] + row0[2] * otherCol2[2];

	auto elem10 = row1[0] * otherCol0[0] + row1[1] * otherCol0[1] + row1[2] * otherCol0[2];
	auto elem11 = row1[0] * otherCol1[0] + row1[1] * otherCol1[1] + row1[2] * otherCol1[2];
	auto elem12 = row1[0] * otherCol2[0] + row1[1] * otherCol2[1] + row1[2] * otherCol2[2];

	auto elem20 = row2[0] * otherCol0[0] + row2[1] * otherCol0[1] + row2[2] * otherCol0[2];
	auto elem21 = row2[0] * otherCol1[0] + row2[1] * otherCol1[1] + row2[2] * otherCol1[2];
	auto elem22 = row2[0] * otherCol2[0] + row2[1] * otherCol2[1] + row2[2] * otherCol2[2];

	return Matrix<width, height, T> { elem00, elem01, elem02,
									  elem10, elem11, elem12,
									  elem20, elem21, elem22 };
}