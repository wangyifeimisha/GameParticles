//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"
#include "Matrix.h"

class Particle
{
public:
	friend class ParticleEmitter;
	
	Particle();	
	Particle(const Particle& p) = default;
	Particle& operator =(const Particle& p) = default;
	~Particle() = default;

	void Update(const float& time_elapsed);
	void CopyDataOnly(const Particle * const p);

	// For align to 16 on heap
	//void* operator new(size_t i);
	//void operator delete(void* p);

private:
	Particle *next;
	Particle *prev;

	float	life;
	float	rotation;
	//float	rotation_velocity;

	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;
	
	Vect4D	prev_Row0;
	Vect4D	prev_Row1;
	Vect4D  prev_Row2;
	Vect4D  prev_Row3;
	
	Vect4D	curr_Row0;
	Vect4D	curr_Row1;
	Vect4D  curr_Row2;
	Vect4D  curr_Row3;
	
	Vect4D  diff_Row0;
	Vect4D  diff_Row1;
	Vect4D  diff_Row2;
	Vect4D  diff_Row3;

	//Matrix prevMatrix;
	//Matrix currMatrix;
	//Matrix diffMatrix;
	
	
};

#endif 

// --- End of File ---
