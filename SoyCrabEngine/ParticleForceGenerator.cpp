#include "ParticleForceGenerator.h"

#pragma region GravityGenerator
void ParticleGravityGenerator::UpdateForce(Particle* Particle, real Duration)
{
	if (!Particle->HasFiniteMass())
		return;
	//입자의 질량 크기에 따른 힘을 적용시킴
	Particle->AddForce(Gravity * Particle->GetMass());
}
#pragma endregion

#pragma region DragGenerator
void ParticleDragGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetVelocity(&Force);
	//토탈 드레그 계수를 계산한다.
	real DragCoefficient = Force.Magnitude();
	DragCoefficient = k1 * DragCoefficient + k2 * real_pow(DragCoefficient, 2);

	//최종 힘을 계산하여 적용한다.
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
	
	//스프링 벡터 계산
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= Other->GetPosition();
	
	//후크의 법칙(Hook's Law)에 의거한 스프링 수식 적용
	//힘의 크기 계산
	real Magnitude = Force.Magnitude();
	Magnitude = real_abs(Magnitude - RestLength);
	Magnitude *= SpringConstant;
	
	//최종 힘을 계산하여 입자에 적용한다.
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

	//후크의 법칙(Hook's Law)에 의거한 스프링 수식 적용
	//힘의 크기 계산
	real Magnitude = Force.Magnitude();
	Magnitude = (RestLength - Magnitude) * SpringConstant;

	//최종 힘을 계산하여 적용
	Force.Normalize();
	Force *= Magnitude;
	Particle->AddForce(Force);
}
#pragma endregion

#pragma region ParticleBungeeGenerator
void ParticleBungeeGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= Other->GetPosition();
	//고무줄이 압축되었는지 검사
	real Magnitude = Force.Magnitude();
	if (Magnitude <= RestLength)
		return;
	
	//힘의 크기 계산
	Magnitude = SpringConstant * (RestLength - Magnitude);

	//최종 힘을 계산하고 적용
	Force.Normalize();
	Force *= -Magnitude;
	Particle->AddForce(Force);
}
#pragma endregion