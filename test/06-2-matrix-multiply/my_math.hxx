#pragma once
#include <iostream>

namespace math
{
///define for matrix dimensions
	const int ROW = 3;
	const int COL = 3;

	struct vec_2
	{
		float x;
		float y;
		vec_2();
		vec_2(float _x, float _y);
		vec_2(vec_2 const& v);
		vec_2(vec_2&& rvalue);
		vec_2& operator=(vec_2 const& v);
		bool operator==(vec_2 const& right_vector) const;
		bool operator!=(vec_2 const& right_vector) const;
	};

	struct vertex
	{
		vec_2 vect;
		vec_2 tex_vect;
		vertex();
		bool operator==(vertex const& right_vertex) const;
		bool operator!=(vertex const& right_vertex) const;
		void print();

	};

	struct triangle
	{
		vertex vert[3];
		triangle();
		triangle& operator*(float n);
		triangle& operator=(triangle const& right_triangle);
		bool operator==(triangle const& right_triangle) const;
		bool operator!=(triangle const& right_triangle) const;
		triangle& operator+(vec_2 const& v);
		void print();

		triangle& rotate(int angle);
	};

	class matrix
	{
	public:
		float mat[ROW][COL];
		matrix();
		matrix(float scale);
		matrix(vec_2 const& v);
		matrix(int angle);
		matrix& operator=(matrix const& right_matrix);
		matrix& operator*(matrix const& right_matrix);
		vec_2 operator*(vec_2 const& vec);
		void print();

		matrix rotate(int angle);
	};
}
