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

#pragma region ParticleAnchoredBungeeGenerator
void ParticleAnchoredBungeeGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= *Anchor;
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

#pragma region ParticleBuoyancyGenerator
void ParticleBuoyancyGenerator::UpdateForce(Particle* Particle, real Duration)
{
	//물속에 잠긴 깊이를 계산한다.
	real Depth = Particle->GetPosition().Y;
	//물속인지 밖인지 검사한다.
	if (Depth >= WaterHeight + MaxDepth)
		return;

	Vector3 Force;
	
	//최대 깊이인지 확인 (완전 잠긴 상태)
	if (Depth <= WaterHeight - MaxDepth)
	{
		Force.Y = LiquidDensity * Volume;
		Particle->AddForce(Force);
		return;
	}

	//아니라면, 부분적으로 잠긴 상태
	Force.Y = LiquidDensity * Volume * (Depth - MaxDepth - WaterHeight) / 2 * MaxDepth;
	Particle->AddForce(Force);
}
#pragma endregion