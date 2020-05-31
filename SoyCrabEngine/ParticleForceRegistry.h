#pragma once
#include "SoyCrabEngine.h"

class ParticleForceGenerator;

class ParticleForceRegistry
{
protected:
	//�ϳ��� �� �߻���� ���� ������ ���ڸ� ����
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* fg;
	};

	//registrations��� ������� ����
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;
public:
	//�� �߻��⸦ ���ڿ� ����� Registry�� ����
	void Add(Particle* particle, ParticleForceGenerator* fg);

	//Registry���� �־��� ���� ����
	//��ϵȰ� ���ٸ� �ƹ��͵� ����.
	void Remove(Particle* particle, ParticleForceGenerator* fg);

	//Registry �ʱ�ȭ
	void Clear();

	//Rgistry�� ��ϵ� ��� �� �߻��� ȣ��
	void UpdateForces(real duration);
};