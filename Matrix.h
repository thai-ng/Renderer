#pragma once
#include <array>
#include <algorithm>

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

	// This is wrong, should be implemented with fold (or with recursion, but I don't have time for that)

	//void operator*=(Matrix<width, height, T>& other)
	//{
	//	auto row0 = getRow<0>();
	//	auto row1 = getRow<1>();
	//	auto row2 = getRow<2>();

	//	auto otherCol0 = other.getCol<0>();
	//	auto otherCol1 = other.getCol<1>();
	//	auto otherCol2 = other.getCol<2>();

	//	auto elem00 = row0[0] * otherCol0[0] + row0[1] * otherCol0[1] + row0[2] * otherCol0[2];
	//	auto elem01 = row0[0] * otherCol1[0] + row0[1] * otherCol1[1] + row0[2] * otherCol1[2];
	//	auto elem02 = row0[0] * otherCol2[0] + row0[1] * otherCol2[1] + row0[2] * otherCol2[2];
	//	
	//	auto elem10 = row1[0] * otherCol0[0] + row1[1] * otherCol0[1] + row1[2] * otherCol0[2];
	//	auto elem11 = row1[0] * otherCol1[0] + row1[1] * otherCol1[1] + row1[2] * otherCol1[2];
	//	auto elem12 = row1[0] * otherCol2[0] + row1[1] * otherCol2[1] + row1[2] * otherCol2[2];

	//	auto elem20 = row2[0] * otherCol0[0] + row2[1] * otherCol0[1] + row2[2] * otherCol0[2];
	//	auto elem21 = row2[0] * otherCol1[0] + row2[1] * otherCol1[1] + row2[2] * otherCol1[2];
	//	auto elem22 = row2[0] * otherCol2[0] + row2[1] * otherCol2[1] + row2[2] * otherCol2[2];

	//	_data = { elem00, elem01, elem02,
	//			  elem10, elem11, elem12,
	//			  elem20, elem21, elem22 };
	//}

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


// No Fold expression in MSVC yet
//template <typename T, std::size_t... Indices, typename Array = std::array<T, sizeof...(Indices)>>
//auto multSumIndice(Array& l, Array& r, std::index_sequence<Indices...>)
//{
//	return (l[Indices] * r[Indices]) + ...;
//}
//
//template <typename T, int length, typename Array = std::array<T, length>>
//T multSum(Array& l, Array& r)
//{
//	return multSumIndice<T>(l, r, std::make_index_sequence<length>());
//}

template <typename T>
auto operator*(Matrix<4, 4, T>& l, Matrix<4, 4, T>& r)
{
	auto row0 = l.getRow<0>();
	auto row1 = l.getRow<1>();
	auto row2 = l.getRow<2>();
	auto row3 = l.getRow<3>();

	auto otherCol0 = r.getCol<0>();
	auto otherCol1 = r.getCol<1>();
	auto otherCol2 = r.getCol<2>();
	auto otherCol3 = r.getCol<3>();

	auto elem00 = row0[0] * otherCol0[0] + row0[1] * otherCol0[1] + row0[2] * otherCol0[2] + row0[3] * otherCol0[3];
	auto elem01 = row0[0] * otherCol1[0] + row0[1] * otherCol1[1] + row0[2] * otherCol1[2] + row0[3] * otherCol1[3];
	auto elem02 = row0[0] * otherCol2[0] + row0[1] * otherCol2[1] + row0[2] * otherCol2[2] + row0[3] * otherCol2[3];
	auto elem03 = row0[0] * otherCol3[0] + row0[1] * otherCol3[1] + row0[2] * otherCol3[2] + row0[3] * otherCol3[3];

	auto elem10 = row1[0] * otherCol0[0] + row1[1] * otherCol0[1] + row1[2] * otherCol0[2] + row1[3] * otherCol0[3];
	auto elem11 = row1[0] * otherCol1[0] + row1[1] * otherCol1[1] + row1[2] * otherCol1[2] + row1[3] * otherCol1[3];
	auto elem12 = row1[0] * otherCol2[0] + row1[1] * otherCol2[1] + row1[2] * otherCol2[2] + row1[3] * otherCol2[3];
	auto elem13 = row1[0] * otherCol3[0] + row1[1] * otherCol3[1] + row1[2] * otherCol3[2] + row1[3] * otherCol3[3];

	auto elem20 = row2[0] * otherCol0[0] + row2[1] * otherCol0[1] + row2[2] * otherCol0[2] + row2[3] * otherCol0[3];
	auto elem21 = row2[0] * otherCol1[0] + row2[1] * otherCol1[1] + row2[2] * otherCol1[2] + row2[3] * otherCol1[3];
	auto elem22 = row2[0] * otherCol2[0] + row2[1] * otherCol2[1] + row2[2] * otherCol2[2] + row2[3] * otherCol2[3];
	auto elem23 = row2[0] * otherCol3[0] + row2[1] * otherCol3[1] + row2[2] * otherCol3[2] + row2[3] * otherCol3[3];

	auto elem30 = row3[0] * otherCol0[0] + row3[1] * otherCol0[1] + row3[2] * otherCol0[2] + row3[3] * otherCol0[3];
	auto elem31 = row3[0] * otherCol1[0] + row3[1] * otherCol1[1] + row3[2] * otherCol1[2] + row3[3] * otherCol1[3];
	auto elem32 = row3[0] * otherCol2[0] + row3[1] * otherCol2[1] + row3[2] * otherCol2[2] + row3[3] * otherCol2[3];
	auto elem33 = row3[0] * otherCol3[0] + row3[1] * otherCol3[1] + row3[2] * otherCol3[2] + row3[3] * otherCol3[3];


	return Matrix<4, 4, T> { elem00, elem01, elem02, elem03,
							 elem10, elem11, elem12, elem13,
							 elem20, elem21, elem22, elem23,
							 elem30, elem31, elem32, elem33 };
}

template <typename T>
auto operator*(Matrix<4, 4, T>& matrix, std::array<T, 4>& vector)
{
	return std::array<T, 4> { matrix.getElement<0, 0>() * vector[0] + matrix.getElement<0, 1>() * vector[1] + matrix.getElement<0, 2>() * vector[2] + matrix.getElement<0, 3>() * vector[3],
							  matrix.getElement<1, 0>() * vector[0] + matrix.getElement<1, 1>() * vector[1] + matrix.getElement<1, 2>() * vector[2] + matrix.getElement<1, 3>() * vector[3],
							  matrix.getElement<2, 0>() * vector[0] + matrix.getElement<2, 1>() * vector[1] + matrix.getElement<2, 2>() * vector[2] + matrix.getElement<2, 3>() * vector[3],
							  matrix.getElement<3, 0>() * vector[0] + matrix.getElement<3, 1>() * vector[1] + matrix.getElement<3, 2>() * vector[2] + matrix.getElement<3, 3>() * vector[3] };
}