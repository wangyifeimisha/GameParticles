//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Particle.h"
#include "Matrix.h"

//void* Particle::operator new(size_t i)
//{
//	return _mm_malloc(i, 16);
//}
//
//void Particle::operator delete(void* p)
//{
//	_mm_free(p);
//}

Particle::Particle()
	:next(nullptr), 
	prev(nullptr),
	life(0.0f),
	rotation(0.0f),
	//rotation_velocity(0.15f),
	position(Vect4D(0.0f, 0.0f, -6.0f)),
	velocity(Vect4D(-1.0f, 0.0f, 0.0f)),
	scale(Vect4D(1.0f, 1.0f, 1.0f))
{
	// construtor
}


void Particle::CopyDataOnly(const Particle * const p)
{
	// copy the data only
	// this way of copying data is more efficient that element by element
	this->life = p->life;
	this->rotation = p->rotation;
	//this->rotation_velocity = p->rotation_velocity;
	this->position = p->position;
	this->velocity = p->velocity;
	this->scale    = p->scale;
}

void Particle::Update(const float& time_elapsed)
{
	//prevMatrix = currMatrix;
	prev_Row0 = curr_Row0;
	prev_Row1 = curr_Row1;
	prev_Row2 = curr_Row2;
	prev_Row3 = curr_Row3;

	//Matrix tmp;
	Matrix tmp(diff_Row0, diff_Row1, diff_Row2, diff_Row3);

	// serious math below - magic secret sauce
	// life update, position update twice, rotation update
	// velocity, rotation_velocity, scale remain
	life += time_elapsed;
	position = position + (velocity * time_elapsed);
	Vect4D z_axis(0.0f, 0.0f, -5.0f);
	Vect4D v(-12,1,0);
	position.Cross( z_axis, v);
	v.norm(v);
	position = position + v * 0.05f * life;

	float MatrixScale = -1.3f * tmp.Determinant();

	if( MatrixScale > 1.0f )
	{
		MatrixScale = 1.0f/MatrixScale;
	};

	rotation = rotation + MatrixScale + 0.15f * time_elapsed ;
}

// --- End of File ---
