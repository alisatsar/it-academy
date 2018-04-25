#include "my_math.hxx"

using namespace math;

///vector class implementation
vector::vector() : x(0.f), y(0.f){}

vector::vector(float _x, float _y) : x(_x), y(_y){}

vector& vector::operator=(math::vector const& right_vector)
{
	this->x = right_vector.x;
	this->y = right_vector.y;
	return *this;
}

vector::vector(const math::vector& right_vector)
{
	this->x = right_vector.x;
	this->y = right_vector.y;
}

bool vector::operator==(vector const& right_vector) const
{
	return (this->x == right_vector.x) && (this->y == right_vector.y);
}

bool vector::operator!=(vector const& right_vector) const
{
	return !(*this == right_vector);
}


vertex::vertex()
{
	vec.x = 0.f;
	vec.y = 0.f;
	text_vec.x = 0.f;
	text_vec.y = 0.f;
}

vertex::vertex(vertex const& right_vertex)
{
	vec.x = right_vertex.vec.x;
	vec.y = right_vertex.vec.y;
	text_vec.x = right_vertex.text_vec.x;
	text_vec.y = right_vertex.text_vec.y;
}

vertex& vertex::operator=(vertex const& right_vertex)
{
	this->text_vec = right_vertex.text_vec;
	this->vec = right_vertex.vec;

	return *this;
}

bool vertex::operator==(vertex const& right_vertex) const
{
	return (this->text_vec == right_vertex.text_vec) && (this->vec == right_vertex.vec);
}

bool vertex::operator!=(vertex const& right_vertex) const
{
	return !(*this == right_vertex);
}

///triangle class implementation
triangle::triangle()
{
	vert[0] = vertex();
	vert[1] = vertex();
	vert[2] = vertex();
}

triangle& triangle::operator*(float n)
{
	matrix scale(n);
	vector result;
	result = scale * this->vert[0].vec;

	return *this;
}

triangle& triangle::operator=(triangle const& right_triangle)
{
	this->vert[0].vec = right_triangle.vert[0].vec;
	this->vert[0].text_vec = right_triangle.vert[0].text_vec;
	return *this;
}

bool triangle::operator==(triangle const& right_triangle) const
{
	for(int i = 0; i < 3; ++i)
	{
		if(this->vert[i] != right_triangle.vert[i])
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

///matrix class implementation
matrix::matrix()
{
	for(int i = 0; i < ROW; ++i)
	{
		for(int j = 0; j < COL; ++j)
		{
			mat[i][j] = 0.f;
		}
	}
}

matrix::matrix(float scale)
{
	for(int i = 0; i < ROW; ++i)
	{
		for(int j = 0; j < COL; ++j)
		{
			if(j == i && j != 2)
			{
				mat[i][j] = scale;
			}
			else
			{
				mat[i][j] = 0.f;
			}
		}
	}
}

matrix& matrix::operator=(matrix const& right_matrix)
{
	matrix left = *this;

	if(this != &right_matrix)
	{
		for(int r = 0; r < ROW; ++r)
		{
			for(int c = 0; c < COL; ++c)
			{
				//left[r][c] = right_matrix[r][c];
			}
		}
	}

	return left;
}

matrix& matrix::operator*(matrix const& right_matrix)
{
	return *this;
}

vector& matrix::operator*(vector const& vec)
{
	float a = this->mat[0][0] * vec.x;
	float b = this->mat[1][1] * vec.y;
	vector result(a, b);
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




