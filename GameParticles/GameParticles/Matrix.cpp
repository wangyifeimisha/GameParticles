//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include <Math.h>
#include "Vect4d.h"
#include "Matrix.h"
#include <xmmintrin.h>
#include <smmintrin.h>  

Matrix::Matrix()
	:m0(0.0f),
	m1(0.0f),
	m2(0.0f),
	m3(0.0f),
	m4(0.0f),
	m5(0.0f),
	m6(0.0f),
	m7(0.0f),
	m8(0.0f),
	m9(0.0f),
	m10(0.0f),
	m11(0.0f),
	m12(0.0f),
	m13(0.0f),
	m14(0.0f),
	m15(0.0f)
{	
}

Matrix::Matrix(Vect4D v1, Vect4D v2, Vect4D v3, Vect4D v4)
	:m0(v1.x),
	m1(v1.y),
	m2(v1.z),
	m3(v1.w),
	m4(v2.x),
	m5(v2.y),
	m6(v2.z),
	m7(v2.w),
	m8(v3.x),
	m9(v3.y),
	m10(v3.z),
	m11(v3.w),
	m12(v4.x),
	m13(v4.y),
	m14(v4.z),
	m15(v4.w)
{}

void Matrix::setIdentMatrix()
{ 
	this->m0 = 1.0f;

	this->m5 = 1.0f;

	this->m10 = 1.0f;

	this->m15 = 1.0f;
}

void Matrix::setTransMatrix(const Vect4D * const t)
{ 
	this->m0 = 1.0f;

	this->m5 = 1.0f;

	this->m10 = 1.0f;
	
	this->m12 = t->x;
	this->m13 = t->y;
	this->m14 = t->z;
	this->m15 = 1.0f;
}

// maybe modify eg. tmp.set(Matrix::MatrixRow::MATRIX_ROW_0, &this->diff_Row0);
void Matrix::set(const MatrixRow row, const Vect4D * const t )
{
	switch( row )
	{
	case MatrixRow::MATRIX_ROW_0:
		this->m0 = t->x;
		this->m1 = t->y;
		this->m2 = t->z;
		this->m3 = t->w;
		break;

	case MatrixRow::MATRIX_ROW_1:
		this->m4 = t->x;
		this->m5 = t->y;
		this->m6 = t->z;
		this->m7 = t->w;
		break;

	case MatrixRow::MATRIX_ROW_2:
		this->m8 = t->x;
		this->m9 = t->y;
		this->m10 = t->z;
		this->m11 = t->w;
		break;

	case MatrixRow::MATRIX_ROW_3:
		this->m12 = t->x;
		this->m13 = t->y;
		this->m14 = t->z;
		this->m15 = t->w;
		break;

	default:
		// should never get here, if we are here something bad has happened
		assert(0);
	}
}

float &Matrix::operator[](const Index e)
{
	// get the individual elements
	switch(e)
	{
	case Index::m0:
		return m0;
		break;
	case Index::m1:
		return m1;
		break;
	case Index::m2:
		return m2;
		break;
	case Index::m3:
		return m3;
		break;
	case Index::m4:
		return m4;
		break;
	case Index::m5:
		return m5;
		break;
	case Index::m6:
		return m6;
		break;
	case Index::m7:
		return m7;
		break;
	case Index::m8:
		return m8;
		break;
	case Index::m9:
		return m9;
		break;	
	case Index::m10:
		return m10;
		break;
	case Index::m11:
		return m11;
		break;	
	case Index::m12:
		return m12;
		break;	
	case Index::m13:
		return m13;
		break;	
	case Index::m14:
		return m14;
		break;
	case Index::m15:
		return m15;
		break;
	default:
		assert(0);
		return m0;
		break;
	}
}

void Matrix::get(const MatrixRow row, Vect4D *t ) const
{ // get a row of the matrix
	switch( row )
	{
	case MatrixRow::MATRIX_ROW_0:
		t->x = this->m0;
		t->y = this->m1;
		t->z = this->m2;
		t->w = this->m3;
		break;

	case MatrixRow::MATRIX_ROW_1:
		t->x = this->m4;
		t->y = this->m5;
		t->z = this->m6;
		t->w = this->m7;
		break;

	case MatrixRow::MATRIX_ROW_2:
		t->x = this->m8;
		t->y = this->m9;
		t->z = this->m10;
		t->w = this->m11;
		break;

	case MatrixRow::MATRIX_ROW_3:
		t->x = this->m12;
		t->y = this->m13;
		t->z = this->m14;
		t->w = this->m15;
		break;

	default:
		assert(0);
	}
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
	Matrix M;

	Vect4D A;
	Vect4D B;
	Vect4D C;
	Vect4D D;

	A._m = _mm_set1_ps(this->v0.x);
	M.v0._m = _mm_mul_ps(A._m, rhs.v0._m);
	B._m = _mm_set1_ps(this->v1.x);
	M.v1._m = _mm_mul_ps(B._m, rhs.v0._m);
	C._m = _mm_set1_ps(this->v2.x);
	M.v2._m = _mm_mul_ps(C._m, rhs.v0._m);
	D._m = _mm_set1_ps(this->v3.x);
	M.v3._m = _mm_mul_ps(D._m, rhs.v0._m);


	A._m = _mm_set1_ps(this->v0.y);
	M.v0._m = _mm_add_ps(M.v0._m, _mm_mul_ps(A._m, rhs.v1._m));
	B._m = _mm_set1_ps(this->v1.y);
	M.v1._m = _mm_add_ps(M.v1._m, _mm_mul_ps(B._m, rhs.v1._m));
	C._m = _mm_set1_ps(this->v2.y);
	M.v2._m = _mm_add_ps(M.v2._m, _mm_mul_ps(C._m, rhs.v1._m));
	D._m = _mm_set1_ps(this->v3.y);
	M.v3._m = _mm_add_ps(M.v3._m, _mm_mul_ps(D._m, rhs.v1._m));


	A._m = _mm_set1_ps(this->v0.z);
	M.v0._m = _mm_add_ps(M.v0._m, _mm_mul_ps(A._m, rhs.v2._m));
	B._m = _mm_set1_ps(this->v1.z);
	M.v1._m = _mm_add_ps(M.v1._m, _mm_mul_ps(B._m, rhs.v2._m));
	C._m = _mm_set1_ps(this->v2.z);
	M.v2._m = _mm_add_ps(M.v2._m, _mm_mul_ps(C._m, rhs.v2._m));
	D._m = _mm_set1_ps(this->v3.z);
	M.v3._m = _mm_add_ps(M.v3._m, _mm_mul_ps(D._m, rhs.v2._m));


	A._m = _mm_set1_ps(this->v0.w);
	M.v0._m = _mm_add_ps(M.v0._m, _mm_mul_ps(A._m, rhs.v3._m));
	B._m = _mm_set1_ps(this->v1.w);
	M.v1._m = _mm_add_ps(M.v1._m, _mm_mul_ps(B._m, rhs.v3._m));
	C._m = _mm_set1_ps(this->v2.w);
	M.v2._m = _mm_add_ps(M.v2._m, _mm_mul_ps(C._m, rhs.v3._m));
	D._m = _mm_set1_ps(this->v3.w);
	M.v3._m = _mm_add_ps(M.v3._m, _mm_mul_ps(D._m, rhs.v3._m));

	return M;
}

Matrix& Matrix::operator/=(const float rhs)
{ 
	// divide each element by a value
	// using inverse multiply trick, faster that individual divides
	float inv_rhs = 1.0f/rhs;

	Vect4D A;
	A._m = _mm_set1_ps(inv_rhs);
	this->v0._m = _mm_mul_ps(A._m, this->v0._m);
	this->v1._m = _mm_mul_ps(A._m, this->v1._m);
	this->v2._m = _mm_mul_ps(A._m, this->v2._m);
	this->v3._m = _mm_mul_ps(A._m, this->v3._m);
	return *this;
}

float Matrix::Determinant() const
{
	float ta = (m10 * m15) - (m11 * m14);
	// tb = (kq - mo)
	float tb = (m9 * m15) - (m11 * m13);
	// tc = (kp - lo)
	float tc = (m9 * m14) - (m10 * m13);
	// td = (jq - mn)
	float td = (m8 * m15) - (m11 * m12);
	// te = (jo - kn)
	float te = (m8 * m13) - (m9 *  m12);
	// tf = (jp - ln)
	float tf = (m8 * m14) - (m10 * m12);
	
	return ((m0 * ((m5 * ta) - (m6 * tb) + (m7 * tc)))
			- (m1 * ((m4 * ta) - (m6 * td) + (m7 * tf)))
			+ (m2 * ((m4 * tb) - (m5 * td) + (m7 * te)))
			- (m3 * ((m4 * tc) - (m5 * tf) + (m6 * te))));
	
}

Matrix Matrix::GetAdjugate() const
{
	Matrix tmp;
	
	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);
	
	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc))
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));
	
	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -( (m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);
	
	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);
	
	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);
	
	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));
	
	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);
	
	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));
	
	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);
	
	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -( (m0*t4) - (m1*t3) + (m3*t5));
	
	// unload	M		(4)		DL-FR
	// load		E		(5)		DLEFR
	t3 = (m5*m15) - (m7*m13);
	
	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);
	
	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);
	
	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);
	
	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -( (m1*t1) - (m2*t2) + (m3*t3));
	
	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);
	
	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);
	
	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);
	
	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));
	
	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);
	
	return tmp;
}

void Matrix::Inverse( Matrix &out ) const
{
	
	float det = Determinant();
	if(fabs(det) < 0.0001f)
	{
		// do nothing, Matrix is not invertable
	}
	else
	{
		out = GetAdjugate();
		out /= det;
	}
}

void Matrix::setScaleMatrix(const Vect4D * const scale)
{
	//	{	sx		0		0		0	}
	//	{	0		sy		0		0	}
	//	{	0		0		sz		0	}
	//	{	0		0		0		1	}
	
	this->m0 = scale->x;

	this->m5 = scale->y;

	this->m10 = scale->z;

	this->m15 = 1.0f;
}

void Matrix::setRotZMatrix(const float az)
{
	//	{	cos		-sin	0		0	}
	//	{	sin		cos		0		0	}
	//	{	0		0		1		0	}
	//	{	0		0		0		1	}
	
	this->m0 = cos(az);
	this->m1 = -sin(az);

	this->m4 = sin(az);
	this->m5 = cos(az);

	this->m10 = 1.0f;

	this->m15 = 1.0f;
	
}

// --- End of File ---
