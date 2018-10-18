#include "ParticleEffect.h"
#include "Random.h"

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::~ParticleEffect()
{
	if (_Particles.Positions != nullptr)
	{
		delete[] _Particles.Positions;
		delete[] _Particles.Velocities;
		delete[] _Particles.Alpha;
		delete[] _Particles.Ages;
		delete[] _Particles.Lifetimes;
		delete[] _Particles.Size;
	}
}

bool ParticleEffect::Init(const std::string &textureFile, unsigned int maxParticles, unsigned int rate)
{
	_MaxParticles = maxParticles;
	_Rate = rate;

	_Particles.Positions = new vec3[_MaxParticles];
	_Particles.Velocities = new vec3[_MaxParticles];
	_Particles.Alpha = new float[_MaxParticles];
	_Particles.Ages = new float[_MaxParticles];
	_Particles.Lifetimes = new float[_MaxParticles];
	_Particles.Size = new float[_MaxParticles];

	return true;
}

void ParticleEffect::Update(float elapsed)
{
	int NumToSpawn = _Rate;

	/// Create new particles ///
	while (
		//We have not reached the particle cap and...
		_NumCurrentParticles < _MaxParticles &&
		//We have more particles to generate this frame...
		NumToSpawn > 0)
	{
		_Particles.Alpha[_NumCurrentParticles]		= LerpAlpha.x;
		_Particles.Ages[_NumCurrentParticles]		= 0.0f;
		_Particles.Lifetimes[_NumCurrentParticles]	= RandomRangef(RangeLifetime.x, RangeLifetime.y);
		_Particles.Size[_NumCurrentParticles]		= LerpSize.x;
		_Particles.Positions[_NumCurrentParticles].Set(RandomRangef(RangeX.x, RangeX.y), RandomRangef(RangeY.x, RangeY.y), RandomRangef(RangeZ.x, RangeZ.y));

		//send the particle in a random direction, with a velocity between our range
		_Particles.Velocities[_NumCurrentParticles].Set(RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f));
		_Particles.Velocities[_NumCurrentParticles].Normalize();
		_Particles.Velocities[_NumCurrentParticles] *= RandomRangef(RangeVelocity.x, RangeVelocity.y);

		//counters...
		_NumCurrentParticles++;
		NumToSpawn--;
	}

	/// Update existing particles ///
	for (unsigned i = 0; i < _NumCurrentParticles; i++)
	{
		_Particles.Ages[i] += elapsed;

		if (_Particles.Ages[i] > _Particles.Lifetimes[i])
		{
			//remove the particle by replacing it with the one at the top of the stack
			_Particles.Alpha[i]		= _Particles.Alpha[_NumCurrentParticles - 1];
			_Particles.Ages[i]		= _Particles.Ages[_NumCurrentParticles - 1];
			_Particles.Lifetimes[i] = _Particles.Lifetimes[_NumCurrentParticles - 1];
			_Particles.Positions[i] = _Particles.Positions[_NumCurrentParticles - 1];
			_Particles.Size[i]		= _Particles.Size[_NumCurrentParticles - 1];
			_Particles.Velocities[i] = _Particles.Velocities[_NumCurrentParticles - 1];

			_NumCurrentParticles--;
			continue;
		}

		_Particles.Positions[i] += _Particles.Velocities[i] * elapsed;

		float interp = _Particles.Ages[i] / _Particles.Lifetimes[i];
		
		_Particles.Alpha[i] = LERP(LerpAlpha.x, LerpAlpha.y, interp);
		_Particles.Size[i]	= LERP(LerpSize.x, LerpSize.y, interp);
	}
}

void ParticleEffect::Render()
{
	if (_NumCurrentParticles == 0)
	{
		return;
	}
}
