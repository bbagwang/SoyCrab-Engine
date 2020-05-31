#include "SoyCrabEngine.h"

#pragma region ParticleForceRegistry
void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* fg)
{
	//입자와 힘 발생기를 묶기 위한 구조체 선언
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ParticleForceRegistry::Remove(Particle* particle, ParticleForceGenerator* fg)
{
	Registry::iterator Iter = registrations.begin();
	for (;Iter!=registrations.end();Iter++)
	{
		if (Iter->particle == particle && Iter->fg == fg)
			return;
	}
	registrations.erase(Iter);
}

void ParticleForceRegistry::Clear()
{
	registrations.clear();
}

void ParticleForceRegistry::UpdateForces(real duration)
{
	for (auto& Iter : registrations)
	{
		Iter.fg->UpdateForce(Iter.particle, duration);
	}
}
#pragma endregion
