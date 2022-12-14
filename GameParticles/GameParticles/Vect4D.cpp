//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Vect4D.h"

Vect4D::Vect4D()
	:x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Vect4D::Vect4D(float tx, float ty, float tz, float tw)
	: x(tx), y(ty), z(tz), w(tw)
{
}


void Vect4D::norm(Vect4D &v) const
{
	float mag = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);

	if(0.0f < mag)
	{
		float inv_mag = 1.0f / mag;
		v.x = this->x * inv_mag;
		v.y = this->y * inv_mag;
		v.z = this->z * inv_mag;
		//v.w = 1.0f;
	}
}

Vect4D Vect4D::operator + (const Vect4D t) const
{
	return Vect4D(this->x + t.x, this->y + t.y, this->z + t.z);
}

float &Vect4D::operator[](const Vect e)
{
	switch(e)
	{
	case Vect::X:
		return x;
		break;
	case Vect::Y:
		return y;
		break;
	case Vect::Z:
		return z;
		break;
	case Vect::W:
		return w;
		break;
	default:
		assert(0);
		return x;
	}
}

Vect4D Vect4D::operator *(const float scale) const
{
	return Vect4D(this->x * scale, this->y * scale, this->z * scale);
}

Vect4D Vect4D::operator - (const Vect4D t) const
{
	return Vect4D(this->x - t.x, this->y - t.y, this->z - t.z);
}

void Vect4D::Cross(const Vect4D &vin, Vect4D &vout) const
{
	vout.x = (y * vin.z - z * vin.y);
	vout.y = (z * vin.x - x * vin.z);
	vout.z = (x * vin.y - y * vin.x);
	//vout.w = 1.0f;
}

void Vect4D::set(const float tx, const float ty, const float tz, const float tw)
{
	this->x = tx;
	this->y = ty;
	this->z = tz;
	this->w = tw;
}

// --- End of File ---
