#pragma once
#include "SoyCrabEngine.h"

//두 물체를 묶어두는 연결 고리.
//연결 고리에 대한 제역 조건을 어기면 접촉이 일어난 것으로 처리한다.
//이 클래스는 끈, 막대 등의 부모 클래스가 되며, 탄성 한계를 갖는 스프링의 부모 클래스로도 사용 가능하다.
class ParticleLink : public ParticleContactGenerator
{
public:
	//양쪽에 연결된 입자의 쌍.
	Particle* Particles[2];

protected:
	//연결 고리의 현재 길이 반환.
	real CurrentLength() const;

public:
	//물체간 연결 상태를 유지하기 위해 적절한 접촉을 생성한다.
	//이 클래스에서는 한 번에 한 개의 접촉만 생성할 수 있고,
	//이 포인터는 단일 요소를 가리키는 포인터만 될 수 있다.
	//인자 limit는 최소한 1 이상이 되어야 하며, 0은 유효하지 않다.
	//끈이 팽팽하게 늘어나 있지 않으면 0을 반환하고, 접촉이 필요하면 1을 반환한다.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const = 0;
	//코드 문서화 목적에 따라 충돌 등록 인터페이스를 다시 한 번 선언.
};

//끈(Cable)은 두 물체를 연결한다.
//두 물체가 너무 멀어지면 접촉을 일으킨다.
class ParticleCable : public ParticleLink
{
public:
	//끈의 최대 길이.
	real MaxLength;

	//끈의 반발 계수(탄력성).
	real Restitution;

public:
	//끈이 과도하게 늘어나지 않도록 하는데 필요한 접촉을 생성한다.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};

//막대기는 두 물체를 연결한다.
//막대기로 연결된 물체가 너무 멀어지거나 너무 가까워지면, 접촉을 일으킨다.
class ParticleRod : public ParticleLink
{
public:
	//막대기의 길이.
	real Length;

public:
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;

};