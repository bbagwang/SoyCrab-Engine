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

#pragma region ParticleFakeSpringGenerator
void ParticleFakeSpringGenerator::UpdateForce(Particle* Particle, real Duration)
{
	//질량이 무한대인지 검사
	if (!Particle->HasFiniteMass())
		return;

	//고정점을 기준으로 물체의 상대좌표 계산
	Vector3 Position;
	Particle->GetPosition(&Position);
	Position -= *Anchor;

	//상수 값을 계산하고 범위 내에 있는지를 검사
	real Gamma = 0.5f * real_sqrt(4 * SpringConstant - real_pow(Damping, 2));
	if (Gamma == 0.0f)
		return;
	Vector3 c = Position * (Damping / (2.0f * Gamma)) + Particle->GetVelocity() * (1.0f / Gamma);
	
	//옮겨가기 원하는 지점의 좌표 계산
	Vector3 Target = Position * real_cos(Gamma * Duration) + c * real_sin(Gamma * Duration);
	Target *= real_exp(-0.5f * Duration * Damping);

	//필요한 가속도와 힘 계산
	Vector3 Accel = (Target - Position) * (1.0f / real_pow(Duration, 2)) - Particle->GetVelocity() * Duration;
	Particle->AddForce(Accel * Particle->GetMass());
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
	//부력은 위로만 작용한다고 가정, 후크의 법칙(Hook's Law)에서 길이에 해당하는 값 계산.
	Force.Y = LiquidDensity * Volume * (Depth - MaxDepth - WaterHeight) / 2 * MaxDepth;
	Particle->AddForce(Force);
}
#pragma endregion