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

//간단하다는 장점이 있지만, 댐핑과 중력 가속도는 기존 코드 사용.
//이유는 매 프레임마다 이걸 일일히 계산해야할 필요가 없기 때문.
//이미 답을 알고있는 계산을 하는 것과 다름이 없다.
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

#pragma region SpringGenerator
//스프링 힘을 적용하는 힘 발생기
class ParticleSpringGenerator : public ParticleForceGenerator
{
private:
	//스프링의 반대쪽 끝의 입자
	Particle* Other;
	//스프링 상수
	real SpringConstant;
	//스프링 휴지 길이
	real RestLength;
public:
	//주어진 인자들을 토대로 새로운 스프링 개체를 생성하는 생성자
	ParticleSpringGenerator(Particle* Other, real SpringConstant, real RestLength)
		:Other(Other), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//주어진 입자에 스프링 힘을 적용한다.
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region ParticleAnchoredSpringGenerator
//한쪽 끝이 고정점에 연결된 스프링의 힘을 적용하는 힘 발생기
class ParticleAnchoredSpringGenerator : public ParticleForceGenerator
{
private:
	//스프링 고정단 위치
	Vector3* Anchor;
	//스프링 상수
	real SpringConstant;
	//스프링 휴지 길이
	real RestLength;
public:
	//주어진 인자를 바탕으로 새로운 스프링 인스턴스를 생성하는 생성자
	ParticleAnchoredSpringGenerator(Vector3* Anchor, real SpringConstant, real RestLength) :
		Anchor(Anchor), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//주어진 입자에 스프링 힘을 적용한다.
	virtual void UpdateForce(Particle* Particle, real Duration);
	void SetAnchor(Vector3* NewAnchorPosition) { Anchor = NewAnchorPosition; }
	Vector3 GetAnchor() const { return *Anchor; }
};
#pragma endregion

#pragma region ParticleBungeeGenerator
//잡아 늘였을 때에만 스프링 힘을 적용하는 힘 발생기
class ParticleBungeeGenerator : public ParticleForceGenerator
{
private:
	//스프링 반대편 끝에 있는 입자
	Particle* Other;
	//스프링 상수
	real SpringConstant;
	//힘을 발생시키기 시작하는 시점에서 고무줄의 길이
	real RestLength;
public:
	//주어진 인자를 바탕으로 새로운 고무줄을 생성하는 생성자
	ParticleBungeeGenerator(Particle* Other, real SpringConstant, real RestLength) :
		Other(Other), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//주어진 입자에 스프링 힘을 적용한다.
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region ParticleAnchoredBungeeGenerator
//한쪽 끝이 고정점에 연결된 잡아 늘였을 때에만 스프링의 힘을 적용하는 힘 발생기
class ParticleAnchoredBungeeGenerator : public ParticleForceGenerator
{
private:
	//스프링 고정단 위치
	Vector3* Anchor;
	//스프링 상수
	real SpringConstant;
	//힘을 발생시키기 시작하는 시점에서 고무줄의 길이
	real RestLength;
public:
	//주어진 인자를 바탕으로 새로운 고무줄을 생성하는 생성자
	ParticleAnchoredBungeeGenerator(Vector3* Anchor, real SpringConstant, real RestLength) :
		Anchor(Anchor), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//주어진 입자에 스프링 힘을 적용한다.
	virtual void UpdateForce(Particle* Particle, real Duration);
	void SetAnchor(Vector3* NewAnchorPosition) { Anchor = NewAnchorPosition; }
	Vector3 GetAnchor() const { return *Anchor; }
};
#pragma endregion

#pragma region ParticleBuoyancyGenerator
//XZ 평면에 평행한 수면에 대해 부력을 적용하는 힘 발생기
class ParticleBuoyancyGenerator : public ParticleForceGenerator
{
private:
	//최대 부력을 발생시키기 전 개체의 최대 침수 깊이
	real MaxDepth;
	//물체의 부피
	real Volume;
	//수면이 y = 0 인 평면으로부터 이동한 높이
	//수면은 XZ 평면에 평행한 것으로 가정.
	real WaterHeight;
	//액체의 밀도
	//순수한 물의 밀도는 1000 kg/m^(-3)
	real LiquidDensity = 1000.f;
public:
	//주어진 인자를 바탕으로 새로운 부력 개체 생성.
	ParticleBuoyancyGenerator(real MaxDepth, real Volume, real WaterHeight, real LiquidDensity = 1000.f)
		:MaxDepth(MaxDepth), Volume(Volume), WaterHeight(WaterHeight), LiquidDensity(LiquidDensity) {}
	//주어진 입자에 부력 적용
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion