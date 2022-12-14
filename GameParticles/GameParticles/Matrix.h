//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"
#include "Vect4D.h"

// forward declare
class Vect4D;

// class
class Matrix
{
public:
	friend class ParticleEmitter;

	// local enumerations
	enum class MatrixRow
	{
		MATRIX_ROW_0,
		MATRIX_ROW_1,
		MATRIX_ROW_2,
		MATRIX_ROW_3
	};

	Matrix();	
	Matrix(const Matrix& t) = default;
	Matrix& operator =(const Matrix& t) = default;
	~Matrix() = default;
	
	Matrix(Vect4D v1, Vect4D v2, Vect4D v3, Vect4D v4);

	void set(const MatrixRow row, const Vect4D * const t );
	void get(const MatrixRow row, Vect4D *vOut ) const;

	void setIdentMatrix();
	void setTransMatrix(const Vect4D * const t );
	void setScaleMatrix(const Vect4D * const s );
	void setRotZMatrix( const float az );

	float &operator[](const Index e);
	
	Matrix operator*(const Matrix &t ) const;
	//Matrix operator*( float s );

	float Determinant() const;
	
	Matrix GetAdjugate() const;
	Matrix& operator/=(const float t);
	
	void Inverse( Matrix &out ) const;
	
private:
	union
	{
		struct
		{
			Vect4D v0;
			Vect4D v1;
			Vect4D v2;
			Vect4D v3;
		};

		struct
		{
			// ROW 0
			float m0;
			float m1;
			float m2;
			float m3;

			// ROW 1
			float m4;
			float m5;
			float m6;
			float m7;

			// ROW 2
			float m8;
			float m9;
			float m10;
			float m11;

			// ROW 3
			float m12;
			float m13;
			float m14;
			float m15;
		};
	};
};

#endif  

// --- End of File ---
