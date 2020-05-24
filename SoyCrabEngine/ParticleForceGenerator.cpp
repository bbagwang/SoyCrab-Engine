#include "ParticleForceGenerator.h"

#pragma region GravityGenerator
void ParticleGravityGenerator::UpdateForce(Particle* Particle, real Duration)
{
	if (!Particle->HasFiniteMass())
		return;
	//������ ���� ũ�⿡ ���� ���� �����Ŵ
	Particle->AddForce(Gravity * Particle->GetMass());
}
#pragma endregion

#pragma region DragGenerator
void ParticleDragGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetVelocity(&Force);
	//��Ż �巹�� ����� ����Ѵ�.
	real DragCoefficient = Force.Magnitude();
	DragCoefficient = k1 * DragCoefficient + k2 * real_pow(DragCoefficient, 2);

	//���� ���� ����Ͽ� �����Ѵ�.
	Force.Normalize();
	Force *= -DragCoefficient;
	Particle->AddForce(Force);
}
#pragma endregion