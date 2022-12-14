//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.scale_variance
//---------------------------------------------------------------

#include "OpenGLDevice.h"
#include "ParticleEmitter.h"
#include "Settings.h"

PerformanceTimer globalTimer;

ParticleEmitter::ParticleEmitter()
:	 		
	last_spawn(globalTimer.GetGlobalTime()),
	max_particles(NUM_PARTICLES),
	last_active_particle(-1),
	last_loop(globalTimer.GetGlobalTime()),
	bufferCount(0),
	headParticle(nullptr),
	vel_variance(-29.0f, 0.70f, -1.0f),
	pos_variance(-3.50f, 3.50f, 5.0f),
	start_position(-1.0f, -4.0f, 1.0f),
	start_velocity(-2.0f, 3.0f, -0.10f),
	particle_list(NUM_PARTICLES)
{
	// nothing to do
	
}

ParticleEmitter::~ParticleEmitter()
{
	Particle *pTmp = this->headParticle;
	while (pTmp != nullptr)
	{
		Particle *pDeleteMe = pTmp;
		pTmp = pTmp->next;
		delete pDeleteMe;
	}
}

void ParticleEmitter::SpawnParticle()
{
	// create another particle if there are ones free
	if( last_active_particle < max_particles-1 )
	{
	
		// create new particle
		Particle *newParticle = new Particle();

		// initialize the particle
		newParticle->life     = 0.0f;
		newParticle->position = start_position;
		newParticle->velocity = start_velocity;
		newParticle->scale    = Vect4D(-1.0, -1.0, -1.0, 1.0);

		// apply the variance
		this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);

		// increment count
		last_active_particle++;

		// add to list
		this->addParticleToList( newParticle );

	}
}

void ParticleEmitter::update()
{
	// get current time
	float current_time = globalTimer.GetGlobalTime();

	// spawn particles
	float time_elapsed = current_time - last_spawn;
	
	// last time (invariants)
	last_spawn = current_time;

	// update
	while(0.00001f < time_elapsed )
	{
		// spawn a particle
		this->SpawnParticle();
		// adjust time
		time_elapsed -= 0.00001f;
	}
	
	// total elapsed
	time_elapsed = current_time - last_loop;

	Particle *p = this->headParticle;
	// walk the particles

	while( p != nullptr )
	{
		// call every particle and update its position 
		p->Update(time_elapsed);

		// if life is greater that the max_life 
		// and there is some left on the list
		// remove node
		if((last_active_particle > 0) && (p->life > MAX_LIFE))
		{
			// particle to remove
			Particle *s = p;

			// need to squirrel it away.
			p=p->next;

			// remove last node
			this->removeParticleFromList( s );

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
		}
	}

	//move a copy to vector for faster iterations in draw
	p = this->headParticle;
	bufferCount = 0;

	// clear the buffer
	drawBuffer.clear();

	// walk the pointers, add to list
	while(p != 0)
	{
		// add to buffer
		drawBuffer.push_back(*p);

		// advance ptr
		p = p->next;

		// track the current count
		bufferCount++;
	}

	// make sure the counts track (asserts go away in release - relax Christos)
	assert(bufferCount == (last_active_particle+1));
	last_loop = current_time;
}
	   
void ParticleEmitter::addParticleToList(Particle *p )
{
	assert(p);
	if( this->headParticle == nullptr )
	{ // first on list
		this->headParticle = p;
		p->next = nullptr;
		p->prev= nullptr;
	}
	else 
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = nullptr;
		headParticle = p;
	}

}

void ParticleEmitter::removeParticleFromList( Particle *p )
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if( p->prev == nullptr && p->next == nullptr  )
	{ // only one on the list
		this->headParticle = nullptr;
	}
	else if( p->prev == nullptr && p->next != nullptr  )
	{ // first on the list
		p->next->prev = nullptr;
		this->headParticle = p->next;
	}
	else if( p->prev!= nullptr && p->next == nullptr )
	{ // last on the list 
		p->prev->next = nullptr;
	}
	else//( p->next != nullptr  && p->prev !=nullptr )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	
	// bye bye
	delete p;
}

void ParticleEmitter::draw()
{
	// initialize the camera matrix
	//Matrix cameraMatrix;
	//cameraMatrix.setIdentMatrix();

	// setup the translation matrix
	//Matrix transMatrix;
	Vect4D trans(0.0f, 5.0f, 40.0f);
	//transMatrix.setTransMatrix(&trans);

	Matrix tmp;
	// multiply them together
	//tmp = cameraMatrix * transMatrix;
	tmp.setTransMatrix(&trans);

	// get the inverse matrix
	Matrix inverseCameraMatrix;
	tmp.Inverse(inverseCameraMatrix);

	// iterate throught the list of particles
	std::vector<Particle>::iterator it;
	for( it = drawBuffer.begin(); it != drawBuffer.end(); ++it)
	{
		// get the position from this matrix
		Vect4D camPosVect;
		inverseCameraMatrix.get( Matrix::MatrixRow::MATRIX_ROW_3, &camPosVect );

		// camera position
		Matrix transCamera;
		transCamera.setTransMatrix( &camPosVect );

		// particle position
		Matrix transParticle;
		transParticle.setTransMatrix( &it->position );

		// rotation matrix
		Matrix rotParticle;
		rotParticle.setRotZMatrix( it->rotation );

		// pivot Point
		//Matrix pivotParticle;
		//Vect4D pivotVect(-20.0f, 0.0f, 200.0f);
		//
		////pivotVect = pivotVect * -20.0 * it->life;
		//pivotVect = pivotVect * it->life;
		//pivotParticle.setTransMatrix( &pivotVect );

		// scale Matrix
		Matrix scaleMatrix;
		scaleMatrix.setScaleMatrix( &it->scale );

		// total transformation of particle
		tmp = scaleMatrix * transCamera * transParticle * rotParticle * scaleMatrix;

		// ------------------------------------------------
		//  Set the Transform Matrix and Draws Triangle
		//  Note: 
		//       this method is using doubles... 
		//       there is a float version (hint)
		// ------------------------------------------------
		OpenGLDevice::SetTransformMatrixFloat((const float *)&tmp);

		// squirrel away matrix for next update
		tmp.get(Matrix::MatrixRow::MATRIX_ROW_0, &it->curr_Row0 );
		tmp.get(Matrix::MatrixRow::MATRIX_ROW_1, &it->curr_Row1 );
		tmp.get(Matrix::MatrixRow::MATRIX_ROW_2, &it->curr_Row2 );
		tmp.get(Matrix::MatrixRow::MATRIX_ROW_3, &it->curr_Row3 );
		//it->currMatrix = tmp;

		// difference vector
		it->diff_Row0 = it->curr_Row0 - it->prev_Row0;
		it->diff_Row1 = it->curr_Row1 - it->prev_Row1;
		it->diff_Row2 = it->curr_Row2 - it->prev_Row2;
		it->diff_Row3 = it->curr_Row3 - it->prev_Row3;
		//it->diffMatrix.v0._m = _mm_sub_ps(it->currMatrix.v0._m, it->prevMatrix.v0._m);
		//it->diffMatrix.v1._m = _mm_sub_ps(it->currMatrix.v1._m, it->prevMatrix.v1._m);
		//it->diffMatrix.v2._m = _mm_sub_ps(it->currMatrix.v2._m, it->prevMatrix.v2._m);
		//it->diffMatrix.v3._m = _mm_sub_ps(it->currMatrix.v3._m, it->prevMatrix.v3._m);

	}

	// delete the buffer
	for( size_t i = 0; i < drawBuffer.size(); i++ )
	{
		drawBuffer.pop_back();
	}

	// done with buffer, clear it.
	drawBuffer.clear();
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Ses it's ugly - I didn't write this so don't bitch at me
	// Sometimes code like this is inside real commerical code ( so know you now how it feels )
	
	// x - variance
	float var = static_cast<float>(rand() % 1000) * 0.0001f;
	float sign = static_cast<float>(rand() % 2);
	float *t_pos = reinterpret_cast<float*>(&pos);
	float *t_var = &pos_variance[Vect::X];
	if(sign == 0.0f)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.0001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if(sign == 0.0f)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if(sign == 0.0f)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	var = static_cast<float>(rand() % 1000) * 0.0001f;
	sign = static_cast<float>(rand() % 2);
	
	// x  - add velocity
	t_pos = &vel[Vect::X];
	t_var = &vel_variance[Vect::X];
	if(sign == 0.0f)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.0001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if(sign == 0.0f)
	{
		var *= -3.0f;
	}
	*t_pos += *t_var * var;
	
	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if(sign == 0.0f)
	{
		var *= -3.0f;
	}
	*t_pos += *t_var * var;
	
	// correct the sign
	var = 1.20f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	
	if(sign == 0.0f)
	{
		var *= -3.0f;
	}
	sc = sc * var;
}

// --- End of File ---
