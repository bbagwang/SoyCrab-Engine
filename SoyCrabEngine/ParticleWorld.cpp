#include "SoyCrabEngine.h"

ParticleWorld::ParticleWorld(unsigned MaxContacts, unsigned Iterations /*= 0*/) :Resolver(Iterations), MaxContacts(MaxContacts)
{
	Contacts = new ParticleContact[MaxContacts];
	CalculateIterations = (Iterations == 0);
}

ParticleWorld::~ParticleWorld()
{
	delete[] Contacts;
}

unsigned ParticleWorld::GenerateContacts()
{
	unsigned Limit = MaxContacts;
	ParticleContact* NextContact = Contacts;
	for (auto& Iter : ContactGenerators)
	{
		unsigned Used = Iter->AddContact(NextContact, Limit);
		Limit -= Used;
		NextContact += Used;
		//�� �̻� ������ ����� ������ ����.
		//�������� �����Ǵ� ������ ��ϵ��� ���ϰ� ��������.
		if (Limit <= 0)
			break;
	}

	//���� ���� ������ ������ ��ȯ�Ѵ�.
	return MaxContacts - Limit;
}

void ParticleWorld::Integrate(real Duration)
{
	for (auto& Iter : Particles)
	{
		Iter->Integrate(Duration);
	}
}

void ParticleWorld::RunPhysics(real Duration)
{
	//���� ����, �� �߻��⸦ �����Ѵ�.
	Registry.UpdateForces(Duration);

	//�� ���� ��ü�� �����Ѵ�.
	Integrate(Duration);

	//������ �����Ѵ�.
	unsigned UsedContacts = GenerateContacts();

	//���� ó�� ����.
	if (UsedContacts)
	{
		if (CalculateIterations)
			Resolver.SetIterations(UsedContacts * 2);
		Resolver.ResolveContacts(Contacts, UsedContacts, Duration);
	}
}

void ParticleWorld::StartFrame()
{
	//������ ������ ���� �շ��� ���� ����� ���� �ʱ�ȭ�Ѵ�.
	for (auto& Iter : Particles)
	{
		Iter->ClearAccumulator();
	}
}

void GroundContacts::Init(std::vector<Particle*>* Particles)
{
	GroundContacts::Particles = Particles;
}

unsigned GroundContacts::AddContact(ParticleContact* Contact, unsigned Limit) const
{
	unsigned Count = 0;

	for (auto* Iter : *Particles)
	{
		real Y = Iter->GetPosition().Y;
		
		if (Y < 0.0f)
		{
			//�ٴ��浹 ���� ���� ����.
			Contact->ContactNormal = Vector3::UP;
			Contact->Particles[0] = Iter;
			Contact->Particles[1] = nullptr;
			Contact->Penetration = -Y;
			Contact->Restitution = 0.2f;
			Contact++;
			Count++;
		}

		if (Count >= Limit)
			return Count;
	}
	return Count;
}
