#include "SoyCrabEngine.h"

void ParticleContact::Resolve(real duration)
{
	ResolveVelocity(duration);
	ResolveInterpenetration(duration);
}

real ParticleContact::CalculateSeparatingVelocity() const
{
	Vector3 RelativeVelocity = Particles[0]->GetVelocity();
	if (Particles[1])
		RelativeVelocity -= Particles[1]->GetVelocity();
	return RelativeVelocity * ContactNormal;
}

void ParticleContact::ResolveVelocity(real duration)
{
	//���� ���⿡ ���� �ӵ��� ����Ѵ�.
	real SeparatingVelocity = CalculateSeparatingVelocity();
	//������ ó���ؾ� �ϴ��� �˻�.
	if (SeparatingVelocity > 0)
	{
		//������ �Ͼ���� ��ü�� �и��ǰ� �ְų�,
		//������ ���� ��� ��ݷ��� �ʿ� ����.
		return;
	}
	//���Ӱ� ���� �и� �ӵ�.
	real NewSepVelocity = -SeparatingVelocity * Restitution;

	//�ӵ��� ���ӵ����� ���� �������� �˻��Ѵ�.
	Vector3 AccCausedVelocity = Particles[0]->GetAcceleration();
	if (Particles[1])
		AccCausedVelocity -= Particles[1]->GetAcceleration();
	real AccCausedSepVelocity = AccCausedVelocity * ContactNormal * duration;

	//���ӵ��� ���� ���� �ӵ��� ��������, �̸� ���ο� �и� �ӵ����� �����Ѵ�.
	if (AccCausedSepVelocity < 0)
	{
		NewSepVelocity += Restitution * AccCausedSepVelocity;
		//���� �ʿ��� �� ���� �� ���� �������� �ʾҴ��� Ȯ���Ѵ�.
		if (NewSepVelocity < 0)
			NewSepVelocity = 0;
	}
	
	real DeltaVelocity = NewSepVelocity - SeparatingVelocity;

	//������ ��ü�� ���Ͽ� ������ �ݺ���Ͽ� �ӵ��� �����Ѵ�.
	//��, ������ ���� ��ü�ϼ��� �ӵ��� ��ȭ�� ���ϴ�.
	real TotalInverseMass = Particles[0]->GetInverseMass();
	if (Particles[1])
		TotalInverseMass += Particles[1]->GetInverseMass();

	//��� ��ü�� ������ ���Ѵ��̸� ��ݷ�(Impulse)�� ��� ȿ���� �ǹ̰� ����.
	if (TotalInverseMass <= 0)
		return;

	//������ ��ݷ��� ����Ѵ�.
	real Impulse = DeltaVelocity / TotalInverseMass;
	//���� ���� ������ ��ݷ��� ����Ѵ�.
	Vector3 ImpulsePerIMass = ContactNormal * Impulse;

	//��ݷ��� �����Ѵ�. ��ݷ��� ���� ��������,
	//������ �ݺ���Ͽ� �ۿ��Ѵ�.
	Particles[0]->SetVelocity(Particles[0]->GetVelocity() + ImpulsePerIMass * Particles[0]->GetInverseMass());
	if (Particles[1])
	{
		//���� 1���� �ݴ� �������� �ۿ��Ѵ�. (�׳� - ���ϸ� ��~)
		Particles[1]->SetVelocity(Particles[1]->GetVelocity() +ImpulsePerIMass * -Particles[1]->GetInverseMass());
	}
}

void ParticleContact::ResolveInterpenetration(real duration)
{
	//������ �κ��� ������ �� �ܰ�� �ǳʶڴ�.
	if (Penetration <= 0)
		return;
	
	//��ü�� �Ű��ִ� �Ÿ��� ������ �ݺ���ϹǷ� ������ �� ���Ѵ�.
	real TotalIverseMass = Particles[0]->GetInverseMass();
	if (Particles[1])
		TotalIverseMass += Particles[1]->GetInverseMass();

	//��� ��ü�� ������ ���Ѵ��̸� �ƹ��� ó���� ���� �ʴ´�.
	if (TotalIverseMass <= 0)
		return;

	//��ü�� �Űܰ� �Ÿ��� ������ �ݺ���Ѵ�.
	Vector3 MovePerIMass = ContactNormal * (Penetration / TotalIverseMass);

	//��ü�� �Űܰ� �Ÿ��� ����Ѵ�.
	ParticleMovement[0] = MovePerIMass * Particles[0]->GetInverseMass();
	if (Particles[1])
	{
		ParticleMovement[1] = MovePerIMass * -Particles[1]->GetInverseMass();
	}
	else
	{
		ParticleMovement[1].Clear();
	}

	//��ü�� �ű��.
	Particles[0]->SetPosition(Particles[0]->GetPosition() + ParticleMovement[0]);
	if (Particles[1])
	{
		Particles[1]->SetPosition(Particles[1]->GetPosition() + ParticleMovement[1]);
	}
}

void ParticleContactResolver::ResolveContacts(ParticleContact* ContactArray, unsigned NumContacts, real Duration)
{
	unsigned i;
	IterationsUsed = 0;
	while (IterationsUsed < Iterations)
	{
		//���� �ӵ��� ���� ū ������ ã�´�.
		real Max = REAL_MAX;
		unsigned MaxIndex = NumContacts;
		for (i = 0; i < NumContacts; i++)
		{
			real SepVel = ContactArray[i].CalculateSeparatingVelocity();
			if (SepVel < Max && (SepVel < 0 || ContactArray[i].Penetration>0))
			{
				Max = SepVel;
				MaxIndex = i;
			}
		}
		//���� üũ
		if(MaxIndex == NumContacts)
			break;

		//�� ������ ó���Ѵ�.
		ContactArray[MaxIndex].Resolve(Duration);

		//��� ������ ��ģ �κ��� ó���Ѵ�.
		Vector3* move = ContactArray[MaxIndex].ParticleMovement;
		for (i = 0; i < NumContacts; i++)
		{
			if (ContactArray[i].Particles[0] == ContactArray[MaxIndex].Particles[0])
			{
				ContactArray[i].Penetration -= move[0] * ContactArray[i].ContactNormal;
			}
			else if (ContactArray[i].Particles[0] == ContactArray[MaxIndex].Particles[1])
			{
				ContactArray[i].Penetration -= move[1] * ContactArray[i].ContactNormal;
			}
			if (ContactArray[i].Particles[1])
			{
				if (ContactArray[i].Particles[1] == ContactArray[MaxIndex].Particles[0])
				{
					ContactArray[i].Penetration += move[0] * ContactArray[i].ContactNormal;
				}
				else if (ContactArray[i].Particles[1] == ContactArray[MaxIndex].Particles[1])
				{
					ContactArray[i].Penetration += move[1] * ContactArray[i].ContactNormal;
				}
			}
		}
		IterationsUsed++;
	}
}
