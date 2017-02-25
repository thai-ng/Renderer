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
	auto getElement()
	{
		return _data[x*width + y];
	}

private:
	
	std::array<T, width*height> _data;
	
	template <typename Iterator>
	using ValueType = typename std::iterator_traits<Iterator>::value_type;

	// Overload for index sequence unpacking
	template <std::size_t... I, typename RandomAccessIterator, typename Array = std::array<ValueType<RandomAccessIterator>, sizeof...(I)>>
	Array make_array(RandomAccessIterator first, std::index_sequence<I...>)
	{
		return Array{ { first[I]... } };
	}

	// Overload taking just an iterator
	template <std::size_t N, typename RandomAccessIterator>
	std::array<ValueType<RandomAccessIterator>, N> make_array(RandomAccessIterator first)
	{
		return make_array(first, std::make_index_sequence<N>());
	}

	template <int Interval, std::size_t... Indices, typename RandomAccessIterator, typename Array = std::array<ValueType<RandomAccessIterator>, sizeof...(Indices)>>
	Array make_array_with_interval(RandomAccessIterator first, std::index_sequence<Indices...>)
	{
		return Array{ {first[Indices * Interval]...} };
	}

	template <int Interval, std::size_t N, typename RandomAccessIterator>
	std::array<ValueType<RandomAccessIterator>, N> make_array_with_interval(RandomAccessIterator first)
	{
		return make_array_with_interval<Interval>(first, std::make_index_sequence<N>());
	}
};