#pragma once
#include "SoyCrabEngine.h"

class ParticleContactResolver;

//두 물체의 접촉을 처리한다.
//접촉을 처리하기 위해서는 물체 간 간섭을 제거하고,
//적절한 반발력을 적용하여 물체를 떼어놓아야 한다.
//접촉하는 물체가 다시 튈 때도 있다.
//접촉에는 호출할 수 있는 함수가 없고,
//접촉의 세부내용을 갖고 있을 뿐이다.
//접촉을 처리하려면 입자 접촉 해소 클래스를 사용한다.

class ParticleContact
{
	//ContactResolver가 Contact에 접근해 작용해야 하므로 friend 선언함.
	friend class ParticleContactResolver;

public:
	//접촉에 포함되는 입자들을 저장한다.
	//배경과 접촉시 NULL 이 될 수 있다.
	Particle* Particles[2];

	//접촉점에서의 반발 계수.
	real Restitution;
	
	//월드 좌표계 기준 접촉 방향 벡터.
	Vector3 ContactNormal;

	//접촉의 결과 얼마나 겹쳐져 있는지를 저장한다.
	real Penetration;

	//각 입자가 접촉하는동안 이동한 양을 저장한다.
	Vector3 ParticleMovement[2];

protected:
	//속도와 겹치는 부분을 업데이트하여 접촉을 해소한다.
	void Resolve(real duration);
	//접촉으로 인한 분리 속도를 계산한다.
	real CalculateSeparatingVelocity() const;

private:
	//접촉에 대한 충격량 계산을 처리한다.
	void ResolveVelocity(real duration);
	//접촉에 의해 겹치는 부분을 처리한다.
	void ResolveInterpenetration(real duration);
};

//입자 접촉에 대한 접촉 처리기.
//인스턴스는 하나만 만들어서 돌려쓰면 된다.
class ParticleContactResolver
{
protected:
	//반복 횟수.
	unsigned Iterations;

	//실제 동작한 반복 횟수를 기록한다.
	//성능 측정이 목적이다.
	unsigned IterationsUsed;

public:
	//새로운 접촉 처리기 개체를 생성한다.
	ParticleContactResolver() {}
	ParticleContactResolver(unsigned Iterations) : Iterations(Iterations) {}

	//최대 반복 횟수를 지정한다.
	void SetIterations(unsigned Iterations) { ParticleContactResolver::Iterations = Iterations; }

	//겹쳐진 부분과 속도에 대해 입자들의 접촉을 처리한다.
	void ResolveContacts(ParticleContact* ContactArray, unsigned NumContacts, real Duration);
};

//입자 접촉 처리를 위한 기본 인터페이스 클레스
class ParticleContactGenerator
{
public:
	ParticleContactGenerator() {}
	//생성된 접촉의 내용을 채워준다.
	//포인터 Contact에는 접촉 구조체 배열의 시작 주소가 들어있고,
	//Limit는 배열의 사용 가능한 최대 크기가 들어 있다.
	//실제 기록된 접촉 구조체의 개수를 반환한다.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const = 0;
};
