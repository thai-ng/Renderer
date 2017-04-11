#pragma once
#include <array>
#include <algorithm>
#include "primitives.hpp"

template <int width, int height, typename T>
class Matrix
{
	using row_t = std::array<T, width>;
	using col_t = std::array<T, height>;

public:

	template <typename... T>
	Matrix(T... values)
	{
		_data = { values... };
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

	auto getData() const
	{
		return _data;
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
auto operator*(const Matrix<4, 4, T>& matrix, const std::array<T, 4>& vector)
{
	return std::array<T, 4> { matrix.getElement<0, 0>() * vector[0] + matrix.getElement<0, 1>() * vector[1] + matrix.getElement<0, 2>() * vector[2] + matrix.getElement<0, 3>() * vector[3],
							  matrix.getElement<1, 0>() * vector[0] + matrix.getElement<1, 1>() * vector[1] + matrix.getElement<1, 2>() * vector[2] + matrix.getElement<1, 3>() * vector[3],
							  matrix.getElement<2, 0>() * vector[0] + matrix.getElement<2, 1>() * vector[1] + matrix.getElement<2, 2>() * vector[2] + matrix.getElement<2, 3>() * vector[3],
							  matrix.getElement<3, 0>() * vector[0] + matrix.getElement<3, 1>() * vector[1] + matrix.getElement<3, 2>() * vector[2] + matrix.getElement<3, 3>() * vector[3] };
}

template <typename T>
auto operator*(const Matrix<4, 4, T>& matrix, const Point4D& vector)
{
	auto v = std::array<T, 4>{vector.x, vector.y, vector.z, vector.w};
	auto newV = matrix * v;
	return Point4D{ newV, vector.color };
}


template <typename T>
auto operator/(const std::array<T, 4>& vector, T divident)
{
	return std::array<T, 4> { vector[0] / divident,
							  vector[1] / divident,
							  vector[2]/* / divident*/,
							  vector[3] / divident };
}

template <typename T>
auto invert(const Matrix<4, 4, T>& matrix)
{
	auto m = matrix.getData();

	std::array<T, 16> inv;
	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	auto det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	if (det == 0)
	{
		throw std::invalid_argument("singular matrix");
	}

	det = 1.0 / det;

	std::array<T, 16> result;
	std::transform(inv.begin(), inv.end(), result.begin(), [det](auto e) { return e * det; });

	return Matrix<4, 4, T>{result};
}