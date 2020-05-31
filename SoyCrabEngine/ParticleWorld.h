#pragma once
#include "SoyCrabEngine.h"

//��� ���ڵ��� �����ϰ�, �̵��� ������Ʈ�� ������ �����Ѵ�.
class ParticleWorld
{
protected:
	//���ڵ��� �����Ѵ�.
	std::vector<Particle*> Particles;

	//���尡 �� ������ Contact Resolver�� ������ Iteraction Count ����� �ʿ��� ��� ���� �ȴ�.
	bool CalculateIterations;

	//���忡 ���� ���ڵ鿡 ���� �� �߻��⸦ ����صд�.
	ParticleForceRegistry Registry;

	//���� ó����
	ParticleContactResolver Resolver;

	//���� �߻����
	std::vector<ParticleContactGenerator*> ContactGenerators;

	//���˵��� ���
	ParticleContact* Contacts;

	//�ִ� ���� ����. ��, ���� �迭�� ũ��.
	unsigned MaxContacts;

public:
	//�־��� ���ڵ��� ó���ϴ� ���ο� ���� �ùķ����� ��ü�� �����ϴ� ������.
	//�ݺ��� ���� ó���⸦ ������ ���� �ִ�.
	//�ݺ� Ƚ���� ������ �������� ������, ������ ������ 2��� ����ȴ�.
	ParticleWorld(unsigned MaxContacts, unsigned Iterations = 0);
	~ParticleWorld();

	//��ϵ� ��� �浹 �߻��⿡ ���� ������, ��� �浹 ó���� �ʿ����� �����´�.
	//��ȯ ���� ������ ������ �����̴�.
	unsigned GenerateContacts();

	//�־��� �ð����� ������� ��� ���ڵ��� ������ ���� ��ġ���� �����.
	void Integrate(real Duration);

	//���忡�� ���ư��� ��� ������ ����Ѵ�.
	void RunPhysics(real Duration);

	//�ùķ��̼� �������� ���� ���带 �ʱ�ȭ�Ѵ�.
	//�� �Լ������� ��� ���ڵ鿡 ���Ͽ� ������ ���� �����.
	//�� �Լ��� ȣ���� �������� �̹� ������ ���� �ۿ��� ���� ��������.
	void StartFrame();

	//�����ϴ� ���ڵ��� �迭�� �����Ѵ�.
	std::vector<Particle*>& GetParticles() { return Particles; }

	//�����ϴ� �浹 �߻������ �迭�� �����Ѵ�.
	std::vector<ParticleContactGenerator*>& GetContactGenerators() { return ContactGenerators; }

	//��ϵ� �� �߻��� �迭�� �����Ѵ�.
	ParticleForceRegistry& GetForceRegistry() { return Registry; }
};

//���� ������ STL ���͸� �����ٰ� ���� �浹�� �߻���Ű�� �浹 �߻���
class GroundContacts : public ParticleContactGenerator
{
	std::vector<Particle*>* Particles;

public:
	//�ʱ�ȭ
	void Init(std::vector<Particle*>* Particles);
	//�浹 ���
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};