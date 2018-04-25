#pragma once
#include <iostream>

namespace math
{
///define for matrix dimensions
const int ROW = 3;
const int COL = 3;

///struct for any vector
struct vector
{
	float x;
	float y;
	vector();
	vector(float _x, float _y);
	vector(const vector &right_vector);
	vector& operator=(vector const& right_vector);
	bool operator==(vector const& right_vector) const;
	bool operator!=(vector const& right_vector) const;

};

struct vertex
{
	vector vec;
	vector text_vec;
	vertex();
	vertex(vertex const& right_vertex);
	vertex& operator=(vertex const& right_vertex);
	bool operator==(vertex const& right_vertex) const;
	bool operator!=(vertex const& right_vertex) const;
};

struct triangle
{
	vertex vert[3];
	triangle();
	triangle& operator*(float n);
	triangle& operator=(triangle const& right_triangle);
	bool operator==(triangle const& right_triangle) const;
	bool operator!=(triangle const& right_triangle) const;
};

class matrix
{
public:
	float mat[ROW][COL];
	matrix();
	matrix(float scale);
	matrix& operator=(matrix const& right_matrix);
	matrix& operator*(matrix const& right_matrix);
	vector& operator*(vector const& vec);
	friend std::ostream& operator<<(std::ostream& os, matrix const& mat);
	void print();
};
std::ostream& operator<<(std::ostream& os, matrix const& mat);

}



//struct matrixn
//{
//
//
//	matrix(vector const& v)
//		{
//			m00 = v.x;
//			m10 = v.y;
//		}
//
//
//
//	matrix operator*(float n)
//	{
//		//m00 = m00 * n;
//		//m10 = m10 * n;
//
//		return *this;
//	}
//
//	matrix operator+(vector const& v)
//	{
//		//m00 = 1;
//		//m11 = 1;
//		//m02 = v.x;
//		//m22 = 1;
//		//m12 = v.y;
//
//		return *this;
//	}
//
//	matrix operator*(matrix const& r)
//	{
//		matrix l = *this;
//		l.m00 = m00 * r.m00 + m01 * r.m10 + m02 * r.m20;
//		l.m01 = m00 * r.m01 + m01 * r.m11 + m02 * r.m21;
//		l.m02 = m00 * r.m02 + m01 * r.m12 + m02 * r.m22;
//
//		l.m10 = m10 * r.m00 + m11 * r.m10 + m12 * r.m20;
//		l.m11 = m10 * r.m01 + m11 * r.m11 + m12 * r.m21;
//		l.m12 = m10 * r.m02 + m11 * r.m12 + m12 * r.m22;
//
//		l.m20 = m20 * r.m00 + m21 * r.m10 + m22 * r.m20;
//		l.m21 = m20 * r.m01 + m21 * r.m11 + m22 * r.m21;
//		l.m22 = m20 * r.m02 + m21 * r.m12 + m22 * r.m22;
//
//		return l;
//	}
//
//
//};






