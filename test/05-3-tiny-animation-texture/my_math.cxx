#include "my_math.hxx"

using namespace math;

///vector class implementation
vector::vector() : x(0.f), y(0.f){}

vector::vector(float _x, float _y) : x(_x), y(_y){}

///vertex class implementation
vertex::vertex() : vec(0.f, 0.f), text_vec(0.f, 0.f){}

///triangle class implementation
triangle::triangle()
{
	vert[0] = vertex();
	vert[1] = vertex();
	vert[2] = vertex();
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






