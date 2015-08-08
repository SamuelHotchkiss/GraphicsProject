#pragma once

#include <cmath>


struct VERTEX
{
	float position[4]; // 0 = X, 1 = Y, 2 = Z, 3 = W
	float color[4];
	float normal[4];
	float U, V;

	VERTEX(float x = 0, float y = 0, float z = 0, float w = 1.0f, float r = 0, float g = 0, float b = 0, float a = 1.0f, 
		float n0 = 0, float n1 = 0, float n2 = 0, float u = 0, float v = 0)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
		position[3] = w;

		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;

		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;
		//normal[3] = 1.0f;

		U = u;
		V = v;
	}

	void CreateNormal()
	{
		float norm = sqrt( (position[0] * position[0]) + (position[1] * position[1]) + (position[2] * position[2]) );
		normal[0] = position[0] / norm;
		normal[1] = position[1] / norm;
		normal[2] = position[2] / norm;
		//normal[3] = position[3] / norm;
	}
};

struct M_3x3
{
	float m[3][3];

	M_3x3(float a = 1, float b = 0, float c = 0, float d = 0, float e = 1, float f = 0, float g = 0, float h = 0, float i = 1)
	{
		m[0][0] = a;
		m[0][1] = b;
		m[0][2] = c;

		m[1][0] = d;
		m[1][1] = e;
		m[1][2] = f;

		m[2][0] = g;
		m[2][1] = h;
		m[2][2] = i;
	}

	M_3x3(float arr[9])
	{
		for (unsigned int rows = 0; rows < 3; rows++)
		{
			for (unsigned int columns = 0; columns < 3; columns++)
			{
				m[rows][columns] = arr[(rows * 3) + columns];
			}
		}
	}
};

struct M_4x4
{
	float M[4][4];

	M_4x4(float a = 1, float b = 0, float c = 0, float d = 0,
		float e = 0, float f = 1, float g = 0, float h = 0,
		float i = 0, float j = 0, float k = 1, float l = 0,
		float m = 0, float n = 0, float o = 0, float p = 1)
	{
		M[0][0] = a;
		M[0][1] = b;
		M[0][2] = c;
		M[0][3] = d;

		M[1][0] = e;
		M[1][1] = f;
		M[1][2] = g;
		M[1][3] = h;

		M[2][0] = i;
		M[2][1] = j;
		M[2][2] = k;
		M[2][3] = l;

		M[3][0] = m;
		M[3][1] = n;
		M[3][2] = o;
		M[3][3] = p;
	}

	M_4x4(float arr[16])
	{
		for (unsigned int rows = 0; rows < 4; rows++)
		{
			for (unsigned int columns = 0; columns < 4; columns++)
			{
				M[rows][columns] = arr[(rows * 4) + columns];
			}
		}
	}
};

void MultiplyVertexByMatrix(VERTEX &vert, M_4x4 mat)
{
	VERTEX ret;
	ret = vert;
	//ret.color = vert.color;
	// Write dot product func later
	ret.position[0] = (vert.position[0] * mat.M[0][0])
		+ (vert.position[1] * mat.M[1][0])
		+ (vert.position[2] * mat.M[2][0])
		+ (vert.position[3] * mat.M[3][0]);
	ret.position[1] = (vert.position[0] * mat.M[0][1])
		+ (vert.position[1] * mat.M[1][1])
		+ (vert.position[2] * mat.M[2][1])
		+ (vert.position[3] * mat.M[3][1]);
	ret.position[2] = (vert.position[0] * mat.M[0][2])
		+ (vert.position[1] * mat.M[1][2])
		+ (vert.position[2] * mat.M[2][2])
		+ (vert.position[3] * mat.M[3][2]);
	ret.position[3] = (vert.position[0] * mat.M[0][3])
		+ (vert.position[1] * mat.M[1][3])
		+ (vert.position[2] * mat.M[2][3])
		+ (vert.position[3] * mat.M[3][3]);

	vert = ret;
}

void MultiplyVertexByMatrix(VERTEX &vert, M_3x3 mat)
{
	VERTEX ret;
	
	for (unsigned int i = 0; i < 4; i++)
		ret.color[i] = vert.color[i];
	// Write dot product func later
	ret.position[0] = (vert.position[0] * mat.m[0][0])
		+ (vert.position[1] * mat.m[1][0])
		+ (vert.position[2] * mat.m[2][0]);
	ret.position[1] = (vert.position[0] * mat.m[0][1])
		+ (vert.position[1] * mat.m[1][1])
		+ (vert.position[2] * mat.m[2][1]);
	ret.position[2] = (vert.position[0] * mat.m[0][2])
		+ (vert.position[1] * mat.m[1][2])
		+ (vert.position[2] * mat.m[2][2]);

	vert = ret;
}

// Matrix Math
M_4x4 RotateZ(float rads)
{
	rads = (rads * 3.14f) / 180.0f;

	M_4x4 ret;
	ret.M[0][0] = cosf(rads);
	ret.M[0][1] = -sinf(rads);
	ret.M[0][2] = 0;

	ret.M[1][0] = sinf(rads);
	ret.M[1][1] = cosf(rads);
	ret.M[1][2] = 0;

	ret.M[2][0] = 0;
	ret.M[2][1] = 0;
	ret.M[2][2] = 1;

	return ret;
}

M_4x4 RotateY(float rads)
{
	rads = (rads * 3.14f) / 180.0f;

	M_4x4 ret;
	ret.M[0][0] = cosf(rads);
	ret.M[0][1] = 0.0f;
	ret.M[0][2] = sinf(rads);
	ret.M[0][3] = 0.0f;

	ret.M[1][0] = 0.0f;
	ret.M[1][1] = 1.0f;
	ret.M[1][2] = 0.0f;
	ret.M[1][3] = 0.0f;

	ret.M[2][0] = -sinf(rads);
	ret.M[2][1] = 0.0f;
	ret.M[2][2] = cosf(rads);
	ret.M[2][3] = 0.0f;

	ret.M[3][0] = 0.0f;
	ret.M[3][1] = 0.0f;
	ret.M[3][2] = 0.0f;
	ret.M[3][3] = 1.0f;

	return ret;
}

M_4x4 RotateX(float rads)
{
	rads = (rads * 3.14f) / 180.0f;

	M_4x4 ret;
	ret.M[0][0] = 1.0f;
	ret.M[0][1] = 0.0f;
	ret.M[0][2] = 0.0f;
	ret.M[0][3] = 0.0f;

	ret.M[1][0] = 0.0f;
	ret.M[1][1] = cosf(rads);
	ret.M[1][2] = -sinf(rads);
	ret.M[1][3] = 0.0f;

	ret.M[2][0] = 0.0f;
	ret.M[2][1] = sinf(rads);
	ret.M[2][2] = cosf(rads);
	ret.M[2][3] = 0.0f;

	ret.M[3][0] = 0.0f;
	ret.M[3][1] = 0.0f;
	ret.M[3][2] = 0.0f;
	ret.M[3][3] = 1.0f;

	return ret;
}


M_4x4 operator*(M_4x4 a, M_4x4 b)
{
	M_4x4 ret;

	for (unsigned int row = 0; row < 4; row++)
	{
		ret.M[row][0] = (a.M[row][0] * b.M[0][0])
			+ (a.M[row][1] * b.M[1][0])
			+ (a.M[row][2] * b.M[2][0])
			+ (a.M[row][3] * b.M[3][0]);
		ret.M[row][1] = (a.M[row][0] * b.M[0][1])
			+ (a.M[row][1] * b.M[1][1])
			+ (a.M[row][2] * b.M[2][1])
			+ (a.M[row][3] * b.M[3][1]);
		ret.M[row][2] = (a.M[row][0] * b.M[0][2])
			+ (a.M[row][1] * b.M[1][2])
			+ (a.M[row][2] * b.M[2][2])
			+ (a.M[row][3] * b.M[3][2]);
		ret.M[row][3] = (a.M[row][0] * b.M[0][3])
			+ (a.M[row][1] * b.M[1][3])
			+ (a.M[row][2] * b.M[2][3])
			+ (a.M[row][3] * b.M[3][3]);
	}

	return ret;
}

M_3x3 Find3x3(M_4x4 mat)
{
	M_3x3 ret =
	{
		mat.M[0][0], mat.M[0][1], mat.M[0][2],
		mat.M[1][0], mat.M[1][1], mat.M[1][2],
		mat.M[2][0], mat.M[2][1], mat.M[2][2]
	};

	return ret;
}

M_3x3 Transpose3x3(M_3x3 mat)
{
	M_3x3 ret = mat;

	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int column = 0; column < 3; column++)
		{
			ret.m[row][column] = mat.m[column][row];
		}
	}

	return ret;
}

M_4x4 Inverse4x4(M_4x4 mat)
{
	//M_4x4 ret;

	M_3x3 trans = Find3x3(mat);
	trans = Transpose3x3(trans);

	VERTEX vert = { mat.M[3][0], mat.M[3][1], mat.M[3][2] };
	MultiplyVertexByMatrix(vert, trans);

	// Negate position vector
	vert.position[0] = -vert.position[0];
	vert.position[1] = -vert.position[1];
	vert.position[2] = -vert.position[2];

	M_4x4 ret =
	{
		trans.m[0][0], trans.m[0][1], trans.m[0][2], mat.M[0][3],
		trans.m[1][0], trans.m[1][1], trans.m[1][2], mat.M[1][3],
		trans.m[2][0], trans.m[2][1], trans.m[2][2], mat.M[2][3],
		vert.position[0], vert.position[1], vert.position[2], mat.M[3][3]
	};

	return ret;
}

void Translate(M_4x4& mat, float x = 0, float y = 0, float z = 0)
{
	M_4x4 mult =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	};

	mat = mult * mat;
}

void ReverseTranslate(M_4x4& mat, float x = 0, float y = 0, float z = 0)
{
	M_4x4 mult =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	};

	mat = mat * mult;
}

float FindZ(VERTEX a, VERTEX b, VERTEX c,
	float alpha, float beta, float gamma)
{
	//Barry interpolate three vertices
	float ret = (a.position[2] * alpha) + (b.position[2] * beta) + (c.position[2] * gamma);
	return ret;
}

float FindW(VERTEX a, VERTEX b, VERTEX c,
	float alpha, float beta, float gamma)
{
	//Barry interpolate three vertices
	float ret = (a.position[3] * alpha) + (b.position[3] * beta) + (c.position[3] * gamma);
	return ret;
}

float FindFloat(float a, float b, float c,
	float alpha, float beta, float gamma)
{
	float ret = (a * alpha) + (b * beta) + (c * gamma);
	return ret;
}

void FindUV(VERTEX a, VERTEX b, VERTEX c,
	float alpha, float beta, float gamma, float &u, float &v)
{
	u = (a.U * alpha) + (b.U * beta) + (c.U * gamma);
	v = (a.V * alpha) + (b.V * beta) + (c.V * gamma);
}