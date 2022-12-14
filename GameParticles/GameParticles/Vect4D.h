//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"

// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;
	friend class ParticleEmitter;

	Vect4D();
	Vect4D(const Vect4D& v) = default;
	Vect4D& operator =(const Vect4D& v) = default;
	~Vect4D() = default;

	Vect4D(float tx, float ty, float tz, float tw = 1.0f);

	void Cross(const Vect4D &vin, Vect4D &vout) const;
	float &operator[](const Vect e);
	void norm(Vect4D &v) const;
	void set(const float tx, const float ty, const float tz, const float tw = 1.0f);

	Vect4D operator * (const float scale) const;
	Vect4D operator - (const Vect4D t) const;
	Vect4D operator + (const Vect4D t) const;

private:
	
	union
	{
		__m128	_m;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

#endif

// --- End of File ---
