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

#pragma region SpringGenerator
void ParticleSpringGenerator::UpdateForce(Particle* Particle, real Duration)
{
	if (!Particle)
		return;
	
	//������ ���� ���
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= Other->GetPosition();
	
	//��ũ�� ��Ģ(Hook's Law)�� �ǰ��� ������ ���� ����
	//���� ũ�� ���
	real Magnitude = Force.Magnitude();
	Magnitude = real_abs(Magnitude - RestLength);
	Magnitude *= SpringConstant;
	
	//���� ���� ����Ͽ� ���ڿ� �����Ѵ�.
	Force.Normalize();
	Force *= -Magnitude;
	Particle->AddForce(Force);
}
#pragma endregion
#pragma region AnchoredSpringGenerator
void ParticleAnchoredSpringGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= *Anchor;

	//��ũ�� ��Ģ(Hook's Law)�� �ǰ��� ������ ���� ����
	//���� ũ�� ���
	real Magnitude = Force.Magnitude();
	Magnitude = (RestLength - Magnitude) * SpringConstant;

	//���� ���� ����Ͽ� ����
	Force.Normalize();
	Force *= Magnitude;
	Particle->AddForce(Force);
}
#pragma endregion
#pragma endregion