#pragma once
#include "SoyCrabEngine.h"

//모든 입자들을 저장하고, 이들을 업데이트할 수단을 제공한다.
class ParticleWorld
{
protected:
	//입자들을 저장한다.
	std::vector<Particle*> Particles;

	//월드가 매 프레임 Contact Resolver에 전달할 Iteraction Count 계산이 필요할 경우 참이 된다.
	bool CalculateIterations;

	//월드에 속한 입자들에 대한 힘 발생기를 등록해둔다.
	ParticleForceRegistry Registry;

	//접촉 처리기
	ParticleContactResolver Resolver;

	//접촉 발생기들
	std::vector<ParticleContactGenerator*> ContactGenerators;

	//접촉들의 목록
	ParticleContact* Contacts;

	//최대 접촉 개수. 즉, 접촉 배열의 크기.
	unsigned MaxContacts;

public:
	//주어진 입자들을 처리하는 새로운 입자 시뮬레이터 개체를 생성하는 생성자.
	//반복적 접촉 처리기를 지정할 수도 있다.
	//반복 횟수를 별도로 저장하지 않으면, 접촉점 개수의 2배로 저장된다.
	ParticleWorld(unsigned MaxContacts, unsigned Iterations = 0);
	~ParticleWorld();

	//등록된 모든 충돌 발생기에 콜을 때려서, 몇개의 충돌 처리가 필요한지 가져온다.
	//반환 값은 생성된 접촉의 개수이다.
	unsigned GenerateContacts();

	//주어진 시간동안 월드상의 모든 입자들을 적분해 다음 위치들을 계산함.
	void Integrate(real Duration);

	//월드에서 돌아가는 모든 물리를 계산한다.
	void RunPhysics(real Duration);

	//시뮬레이션 프레임을 위해 월드를 초기화한다.
	//이 함수에서는 모든 입자들에 대하여 누적된 힘을 지운다.
	//이 함수를 호출한 다음에는 이번 프레임 동안 작용한 힘이 더해진다.
	void StartFrame();

	//존재하는 입자들의 배열을 리턴한다.
	std::vector<Particle*>& GetParticles() { return Particles; }

	//존재하는 충돌 발생기들의 배열을 리턴한다.
	std::vector<ParticleContactGenerator*>& GetContactGenerators() { return ContactGenerators; }

	//등록된 힘 발생기 배열을 리턴한다.
	ParticleForceRegistry& GetForceRegistry() { return Registry; }
};

//입자 포인터 STL 벡터를 가져다가 지면 충돌을 발생시키는 충돌 발생기
class GroundContacts : public ParticleContactGenerator
{
	std::vector<Particle*>* Particles;

public:
	//초기화
	void Init(std::vector<Particle*>* Particles);
	//충돌 등록
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};