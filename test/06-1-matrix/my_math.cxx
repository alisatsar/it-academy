#include "my_math.hxx"

#include <cmath>

using namespace math;

vec_2::vec_2() : x(0.f), y(0.f) {}

vec_2::vec_2(float _x, float _y) : x(_x), y(_y) {}

vec_2::vec_2(vec_2 const& v)
{
	x = v.x;
	y = v.y;
}

vec_2& vec_2::operator=(vec_2 const& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
	}
	return *this;
}

vec_2::vec_2(vec_2&& rvalue)
{
	x = rvalue.x;
	y = rvalue.y;
}

bool vec_2::operator==(vec_2 const& right_vector) const
{
	return (this->x == right_vector.x) && (this->y == right_vector.y);
}

bool vec_2::operator!=(vec_2 const& right_vector) const
{
	return !(*this == right_vector);
}

vertex::vertex() : vect(0.f, 0.f), tex_vect(0.f, 0.f) {}

bool vertex::operator==(vertex const& right_vertex) const
{
	return (this->tex_vect == right_vertex.tex_vect) && (this->vect == right_vertex.vect);
}

bool vertex::operator!=(vertex const& right_vertex) const
{
	return !(*this == right_vertex);
}

void vertex::print()
{
	std::cout << "vect.x	" << vect.x << std::endl;
	std::cout << "vect.y	" << vect.y << std::endl;
	std::cout << "tex_vect.x	" << tex_vect.x << std::endl;
	std::cout << "tex_vect.y	" << tex_vect.y << std::endl;
}

triangle::triangle()
{
	vert[0] = vertex();
	vert[1] = vertex();
	vert[2] = vertex();
}

triangle& triangle::operator*(float n)
{
	matrix scale(n);
	vec_2 result;
	for (int i = 0; i < 3; i++)
	{
		result = scale * this->vert[i].vect;

		this->vert[i].vect.x = result.x;
		this->vert[i].vect.y = result.y;
	}

	return *this;
}

triangle& triangle::operator=(triangle const& right_triangle)
{
	this->vert[0].vect = right_triangle.vert[0].vect;
	this->vert[0].tex_vect = right_triangle.vert[0].tex_vect;
	return *this;
}

bool triangle::operator==(triangle const& right_triangle) const
{
	for (int i = 0; i < 3; ++i)
	{
		if (this->vert[i] != right_triangle.vert[i])
		{
			return false;
		}
	}
	return true;
}

bool triangle::operator!=(triangle const& right_triangle) const
{
	return !(*this == right_triangle);
}

//move
triangle& triangle::operator+(vec_2 const& v)
{
	matrix m(v);
	for (int i = 0; i < 3; ++i)
	{
		this->vert[i].vect = m * this->vert[i].vect;
	}
	return *this;
}

void triangle::print()
{
	for(int i = 0; i < 3; ++i)
	{
		this->vert[i].print();
	}
}

triangle& triangle::rotate(int angle)
{
	matrix m(-angle);

	for (int i = 0; i < 3; ++i)
	{
		this->vert[i].vect = m * this->vert[i].vect;
	}
	return *this;
}

///matrix class implementation
matrix::matrix()
{
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if(i == j)
			{
				mat[i][j] = 1.f;
			}
			else
			{
				mat[i][j] = 0.f;
			}
		}
	}
}

matrix::matrix(float scale)
{
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if (j == i && j != 2)
			{
				mat[i][j] = scale;
			}
			else
			{
				mat[i][j] = 0.0;
			}
		}
	}
}

matrix::matrix(vec_2 const& v)
{
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if (j == i)
			{
				mat[i][j] = 1;
			}
			else if (j == 2 && j != i)
			{
				if (i == 0)
				{
					mat[i][j] = v.x;
				}
				if (i == 1)
				{
					mat[i][j] = v.y;
				}
			}
			else
			{
				mat[i][j] = 0.0;
			}
		}
	}
}

matrix::matrix(int angle)
{
	const float pi = 3.1416;
	float angle_rad = angle * pi / 180;
	mat[0][0] = cos(angle_rad);
	mat[0][1] = -sin(angle_rad);
	mat[0][2] = 0;

	mat[1][0] = sin(angle_rad);
	mat[1][1] = cos(angle_rad);
	mat[1][2] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
}

matrix& matrix::operator=(matrix const& right_matrix)
{
	matrix left = *this;

	if (this != &right_matrix)
	{
		for (int r = 0; r < ROW; ++r)
		{
			for (int c = 0; c < COL; ++c)
			{
				left.mat[r][c] = right_matrix.mat[r][c];
			}
		}
	}

	return left;
}

matrix& matrix::operator*(matrix const& right_matrix)
{
	return *this;
}

vec_2 matrix::operator*(vec_2 const& vec)
{
	float a = 0.f;
	float b = 0.f;

	a += this->mat[0][0] * vec.x + this->mat[0][1] * vec.y + this->mat[0][2] * 1;
	b += this->mat[1][0] * vec.x + this->mat[1][1] * vec.y + this->mat[1][2] * 1;

	//float a = this->mat[0][0] * vec.x;
	//float b = this->mat[1][1] * vec.y;
	vec_2 result(a, b);
	return result;
}

std::ostream& operator<<(std::ostream& os, matrix const& mat)
{
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 0; c < COL; c++)
		{
			os << "[" << r << "]" << "[" << c << "]" << mat.mat[r][c] << "\n";
		}
		os << std::endl;
	}

	return os;
}

void matrix::print()
{
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 0; c < COL; c++)
		{
			std::cout << this->mat[r][c];
		}
		std::cout << "\n";
	}
}

matrix matrix::rotate(int angle)
{
	const float pi = 3.1416;
	float angle_rad = angle * pi / 180;
	matrix m;
	m.mat[0][0] = cos(angle_rad);
	m.mat[0][1] = -sin(angle_rad);
	m.mat[1][0] = sin(angle_rad);
	m.mat[1][1] = cos(angle_rad);
	return m;
}





