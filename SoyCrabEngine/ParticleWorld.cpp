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
		//더 이상 접촉을 기록할 공간이 없다.
		//이제부터 생성되는 접촉은 기록되지 못하고 없어진다.
		if (Limit <= 0)
			break;
	}

	//실제 사용된 접촉의 개수를 반환한다.
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
	//제일 먼저, 힘 발생기를 적용한다.
	Registry.UpdateForces(Duration);

	//그 다음 개체를 적분한다.
	Integrate(Duration);

	//접촉을 생성한다.
	unsigned UsedContacts = GenerateContacts();

	//접촉 처리 진행.
	if (UsedContacts)
	{
		if (CalculateIterations)
			Resolver.SetIterations(UsedContacts * 2);
		Resolver.ResolveContacts(Contacts, UsedContacts, Duration);
	}
}

void ParticleWorld::StartFrame()
{
	//프레임 던지면 계산된 합력을 다음 계산을 위해 초기화한다.
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
			//바닥충돌 관련 변수 설정.
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
