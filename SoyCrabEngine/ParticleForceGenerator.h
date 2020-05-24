#pragma once
#include "SoyCrabEngine.h"

class ParticleForceGenerator
{
public:
	//주어진 입자에 적용하는 힘을 계산하고 업데이트 하기 위해 이 메서드를 재정의
	virtual void UpdateForce(Particle* Particle, real Duration) = 0;
};

#pragma region GravityGenerator
//중력을 적용하는 힘 발생기
//인스턴스 하나로 여러 개의 입자에 사용 가능.
class ParticleGravityGenerator : public ParticleForceGenerator
{
private:
	//중력 가속도
	Vector3 Gravity;
public:
	//주어진 가속도로 힘 발생기 인스턴스 생성
	ParticleGravityGenerator(const Vector3& Gravity) : Gravity(Gravity) {}
	//주어진 입자에 중력 적용
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region DragGenerator
//드레그 힘음 적용하는 힘 발생기
//인스턴스 하나로 여러 개의 입자에 사용 가능.
class ParticleDragGenerator : public ParticleForceGenerator
{
	//Drag Force 식
	//드레그 힘 = -속도(드레그 상수1(k1)*(정규 속도의 크기)+드레그 상수2(k2)*(정규 속도의 크기)^2)
private:
	//Drag Coefficient (드레그 계수)
	//속도에 곱해지는 드래그 비례상수 저장
	real k1;
	//속도의 제곱에 곱해지는 드레그 비례상수 저장
	//k2값이 클수록 속도가 빨라짐에 따라 급격히 드레그가 강해짐.
	//속도가 두배 빨라지면 공기저항은 4배가 되는 것과 같은 맥락.
	real k2;
public:
	//주어진 비례상수로 힘 발생기 개체 생성
	ParticleDragGenerator(real k1, real k2) : k1(k1), k2(k2) {}
	//주어진 입자에 드레그 힘 적용
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion