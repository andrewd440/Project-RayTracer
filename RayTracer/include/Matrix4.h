#pragma once

#include <initializer_list>
#include "Vector3.h"
#include "Vector4.h"

/**
*	4x4 floating-point matrix
*	Elements are access by [row][col]
*/
struct Matrix4
{
public:
	enum class Axis
	{
		X,
		Y,
		Z
	};

public:
	/** 
	* Each row in the matrix is a vector.
	*/
	float M[4][4];

	/**
	* Constructs identity matrix.
	*/
	Matrix4();

	/**
	* Constructs member-wise initialized matrix.
	* @param Mat - Matrix element values by row
	*/
	Matrix4(const std::initializer_list<float>& Mat);

	/**
	* Constructs a matrix by row vectors.
	* @param XRow X vector
	* @param YRow Y vector
	* @param ZRow Z vector
	*/
	Matrix4(const Vector3f& XRow, const Vector3f& YRow, const Vector3f& ZRow);
	
	/**
	* Performs matrix-matrix multiplication.
	* @param Rhs Matrix to multiply by.
	* @return Reference to this matrix.
	*/
	Matrix4& operator*=(const Matrix4& Rhs);

	/**
	* Performs matrix-scalar multiplication.
	* @param scalar - Scalar to multiply by.
	* @return Reference to this matrix.
	*/
	Matrix4& operator*=(float Scalar);

	/**
	* Performs matrix component-wise addition.
	* @param Rhs Matrix to add.
	* @return Reference to this matrix.
	*/
	Matrix4& operator+=(const Matrix4& Rhs);

	/**
	* Transforms a vector without taking translation into account.
	*/
	Vector3f TransformDirection(const Vector3f& Direction) const;

	/**
	* Transforms a position, taking translation into account.
	*/
	Vector3f TransformPosition(const Vector3f& Position) const;

	/**
	* Transforms a vector with homogeneous coordinates.
	*/
	Vector4f TransformVector(const Vector4f& Vector) const;

	/**
	* Retrieve an axis vector from the matrix.
	*/
	Vector3f GetAxis(Matrix4::Axis Axis) const;

	/**
	* Sets a basis vector in the matrix.
	*/
	void SetAxis(Matrix4::Axis Axis, const Vector3f& Basis);

	/**
	* Retreive a row of the matrix. Includes translation component.
	* @param Row of the matrix, 0 = first row, 1 = second row ...
	*/
	Vector4f GetRow(int Row) const;
};

///////////////////////////////////////////////////////////////////////////
////////////////// Inlined Member Functions ///////////////////////////////
///////////////////////////////////////////////////////////////////////////

inline Matrix4::Matrix4()
	: Matrix4({ 1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 })
{}

inline Matrix4::Matrix4(const std::initializer_list<float>& Mat)
{
	assert(Mat.size() == 16);

	auto itr = Mat.begin();

	M[0][0] = *(itr++); M[0][1] = *(itr++); M[0][2] = *(itr++); M[0][3] = *(itr++);
	M[1][0] = *(itr++); M[1][1] = *(itr++); M[1][2] = *(itr++); M[1][3] = *(itr++);
	M[2][0] = *(itr++); M[2][1] = *(itr++); M[2][2] = *(itr++); M[2][3] = *(itr++);
	M[3][0] = *(itr++); M[3][1] = *(itr++); M[3][2] = *(itr++); M[3][3] = *(itr++);
}

inline Matrix4::Matrix4(const Vector3f& XRow, const Vector3f& YRow, const Vector3f& ZRow)
{
	SetAxis(Axis::X, XRow);
	SetAxis(Axis::Y, YRow);
	SetAxis(Axis::Z, ZRow);

	M[3][0] = M[3][1] = M[3][2] = M[0][3] = M[1][3] = M[2][3] = 0.0f;
	M[3][3] = 1.0f;
}

inline Matrix4& Matrix4::operator*=(const Matrix4& Rhs)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			Vector4f thisRow(M[row][0], M[row][1], M[row][2], M[row][3]);
			Vector4f rhsCol(Rhs.M[0][col], Rhs.M[1][col], Rhs.M[2][col], Rhs.M[3][col]);
			M[row][col] = Vector4f::Dot4(thisRow, rhsCol);
		}
	}

	return *this;
}

inline Matrix4& Matrix4::operator*=(float Scalar)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			M[row][col] *= Scalar;
		}
	}

	return *this;
}

inline Matrix4& Matrix4::operator+=(const Matrix4& Rhs)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			M[row][col] += Rhs.M[row][col];
		}
	}

	return *this;
}

inline Vector3f Matrix4::TransformDirection(const Vector3f& Direction) const
{
	Vector4f vec(Direction);
	vec.w = 0.0f;
	vec = TransformVector(vec);
	return Vector3f(vec.x, vec.y, vec.z);
}

inline Vector3f Matrix4::TransformPosition(const Vector3f& Position) const
{
	Vector4f vec(Position);
	vec.w = 1.0f;
	vec = TransformVector(vec);
	return Vector3f(vec.x, vec.y, vec.z);
}

inline Vector4f Matrix4::TransformVector(const Vector4f& Vector) const
{
	Vector4f transformed;
	transformed.x = Vector4f::Dot4(GetRow(0), Vector);
	transformed.y = Vector4f::Dot4(GetRow(1), Vector);
	transformed.z = Vector4f::Dot4(GetRow(2), Vector);
	transformed.w = Vector4f::Dot4(GetRow(3), Vector);
	return transformed;
}

inline Vector3f Matrix4::GetAxis(Matrix4::Axis Axis) const
{
	switch (Axis)
	{
	case Axis::X:
		return Vector3f(M[0][0], M[0][1], M[0][2]);
	case Axis::Y:
		return Vector3f(M[1][0], M[1][1], M[1][2]);
	case Axis::Z:
		return Vector3f(M[2][0], M[2][1], M[2][2]);
	}
}

inline void Matrix4::SetAxis(Matrix4::Axis Axis, const Vector3f& Basis)
{
	int row = -1;
	switch (Axis)
	{
		case Axis::X:
			row = 0;
			break;
		case Axis::Y:
			row = 1;
			break;
		case Axis::Z:
			row = 2;
			break;
		default:
			return;
	}

	M[row][0] = Basis.x; M[row][1] = Basis.y; M[row][2] = Basis.z;
}

inline Vector4f Matrix4::GetRow(int Row) const
{
	return Vector4f(M[Row][0], M[Row][1], M[Row][2], M[Row][3]);
}


///////////////////////////////////////////////////////////////////////////
////////////////// Non Member Functions ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////

inline Matrix4 operator*(Matrix4 Lhs, const Matrix4& Rhs)
{
	return Lhs *= Rhs;
}

inline Matrix4 operator*(Matrix4 Lhs, float Scalar)
{
	return Lhs *= Scalar;
}

inline Matrix4 operator*(float Scalar, Matrix4 Lhs)
{
	return Lhs *= Scalar;
}

inline Matrix4 operator+(Matrix4 Lhs, const Matrix4& Rhs)
{
	return Lhs += Rhs;
}



/**
* A camera look-at matrix.
*/
struct LookAtMatrix : Matrix4
{
	/**
	* Constructs a look-at matrix.
	* @param Eye Position of the eye.
	* @param LookLocation Position to look toward.
	* @param UpDirection Direction of up.
	*/
	LookAtMatrix(const Vector3f& Eye, const Vector3f& LookLocation, const Vector3f& UpDirection);
};

LookAtMatrix::LookAtMatrix(const Vector3f& Eye, const Vector3f& LookLocation, const Vector3f& UpDirection)
{
	Vector3f N = (LookLocation - Eye).Normalize();
	Vector3f U = Vector3f::Cross(N, UpDirection).Normalize();
	Vector3f V = Vector3f::Cross(U, N).Normalize();

	for (int col = 0; col < 3; col++)
	{
		M[0][col] = U[col];
		M[1][col] = V[col];
		M[2][col] = N[col];
		M[3][col] = 0.0f;
	}

	M[0][3] = -Vector3f::Dot(Eye, U);
	M[1][3] = -Vector3f::Dot(Eye, V);
	M[2][3] = -Vector3f::Dot(Eye, N);
	M[3][3] = 1.0f;
}