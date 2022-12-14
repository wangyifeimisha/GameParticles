//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Matrix.h"
#include "Vect4D.h"
#include "Particle.h"

#include <list>
#include <vector>

class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter& p) = delete;
	ParticleEmitter& operator =(const ParticleEmitter& p) = delete;
	~ParticleEmitter();
	
	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(Particle *p );
	void removeParticleFromList( Particle *p );

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:
	float	last_spawn;
	int		max_particles;
	int		last_active_particle;
	float	last_loop;
	int		bufferCount;		// speed efficiency
	Particle* headParticle;
	
	// added for speed efficiency
	Vect4D	vel_variance;
	Vect4D	pos_variance;

	Vect4D	start_position;		// remain the same
	Vect4D	start_velocity;		// remain the same
		
	std::vector<Particle> drawBuffer;
	std::vector<Particle> particle_list;

};

#endif 

// --- End of File ---
