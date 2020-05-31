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
	//막대기가 늘어나거나 줄어들지 않도록 하는데 필요한 접촉을 일으킨다.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};

//컨스트레인트는 그냥 링크와 똑같지만,
//한 가지 다른점은 움직이지 않는 앵커 포인트에 연결되어있다는 것이다.
class ParticleConstraint : public ParticleContactGenerator
{
public:
	//현재 이 컨스트레인트에 잡힌 입자들의 배열.
	Particle* OtherParticle;

	//앵커 포인트
	Vector3 Anchor;

protected:
	//현재 링크의 길이를 반환
	real CurrentLength() const;

public:
	//이 링크를 벗어나지 않도록 지속적으로 접촉을 생성한다.
	//이 클래스는 단일 접점만 생성할 수 있으므로,
	//포인터는 단일 요소에 대한 포인터가 될 수 있으며,
	//Limit는 최소 1 (0은 유효하지 않음)로 가정되며,
	//케이블이 과도하게 늘어나지 않은 경우 반환값은 0이고,
	//접촉이 필요한 경우 1을 반환한다.
	//이건 베이스 클래스고, 하위 클래스들에서 구현함.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const = 0;
};

//컨스트레인트 케이블은 앵커 포인트로 입자를 링크해둔다.
//너무 늘어날 경우 접촉을 발생시킨다.
class ParticleCableConstraint : public ParticleConstraint
{
public:
	//케이블의 최대 길이
	real MaxLength;

	//케이블의 반발계수 (Bounciness)
	real Restitution;

public:
	//케이블이 너무 늘어나지 않도록, 주어진 접촉구조체에 값을 체워넣는다.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};

//컨스트레인트 막대기는 앵커 포인트로 입자를 링크해둔다.
//너무 늘어날 경우 접촉을 발생시킨다.
class ParticleRodConstraint : public ParticleConstraint
{
public:
	//막대기의 최대 길이
	real Length;

public:
	//막대기가 늘어나거나 줄어들지 않도록, 주어진 접촉구조체에 값을 체워넣는다.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};