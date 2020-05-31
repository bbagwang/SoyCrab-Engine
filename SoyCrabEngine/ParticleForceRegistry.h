#pragma once
#include "SoyCrabEngine.h"

class ParticleForceGenerator;

class ParticleForceRegistry
{
protected:
	//하나의 힘 발생기와 힘을 적용할 입자를 추적
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* fg;
	};

	//registrations라는 목록으로 유지
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;
public:
	//힘 발생기를 입자에 등록해 Registry에 넣음
	void Add(Particle* particle, ParticleForceGenerator* fg);

	//Registry에서 주어진 쌍을 제거
	//등록된게 없다면 아무것도 안함.
	void Remove(Particle* particle, ParticleForceGenerator* fg);

	//Registry 초기화
	void Clear();

	//Rgistry에 등록된 모든 힘 발생기 호출
	void UpdateForces(real duration);
};