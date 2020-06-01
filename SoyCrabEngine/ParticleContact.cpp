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
	//접촉 방향에 대한 속도를 계산한다.
	real SeparatingVelocity = CalculateSeparatingVelocity();
	//접촉을 처리해야 하는지 검사.
	if (SeparatingVelocity > 0)
	{
		//접촉이 일어났으나 물체가 분리되고 있거나,
		//가만히 있을 경우 충격량이 필요 없다.
		return;
	}
	//새롭게 계산된 분리 속도.
	real NewSepVelocity = -SeparatingVelocity * Restitution;

	//속도가 가속도만에 의한 것인지를 검사한다.
	Vector3 AccCausedVelocity = Particles[0]->GetAcceleration();
	if (Particles[1])
		AccCausedVelocity -= Particles[1]->GetAcceleration();
	real AccCausedSepVelocity = AccCausedVelocity * ContactNormal * duration;

	//가속도에 의해 접근 속도가 생겼으면, 이를 새로운 분리 속도에서 제거한다.
	if (AccCausedSepVelocity < 0)
	{
		NewSepVelocity += Restitution * AccCausedSepVelocity;
		//실제 필요한 것 보다 더 많이 빼내지는 않았는지 확인한다.
		if (NewSepVelocity < 0)
			NewSepVelocity = 0;
	}
	
	real DeltaVelocity = NewSepVelocity - SeparatingVelocity;

	//각각의 물체에 대하여 질량에 반비례하여 속도를 변경한다.
	//즉, 질량이 작은 물체일수록 속도의 변화가 심하다.
	real TotalInverseMass = Particles[0]->GetInverseMass();
	if (Particles[1])
		TotalInverseMass += Particles[1]->GetInverseMass();

	//모든 물체의 질량이 무한대이면 충격량(Impulse)의 계산 효과가 의미가 없다.
	if (TotalInverseMass <= 0)
		return;

	//적용할 충격량을 계산한다.
	real Impulse = DeltaVelocity / TotalInverseMass;
	//접촉 법선 방향의 충격량을 계산한다.
	Vector3 ImpulsePerIMass = ContactNormal * Impulse;

	//충격량을 적용한다. 충격량은 접촉 방향으로,
	//질량에 반비례하여 작용한다.
	Particles[0]->SetVelocity(Particles[0]->GetVelocity() + ImpulsePerIMass * Particles[0]->GetInverseMass());
	if (Particles[1])
	{
		//입자 1에는 반대 방향으로 작용한다. (그냥 - 곱하면 끝~)
		Particles[1]->SetVelocity(Particles[1]->GetVelocity() +ImpulsePerIMass * -Particles[1]->GetInverseMass());
	}
}

void ParticleContact::ResolveInterpenetration(real duration)
{
	//겹쳐진 부분이 없으면 이 단계는 건너뛴다.
	if (Penetration <= 0)
		return;
	
	//물체를 옮겨주는 거리는 질량에 반비례하므로 질량을 다 더한다.
	real TotalIverseMass = Particles[0]->GetInverseMass();
	if (Particles[1])
		TotalIverseMass += Particles[1]->GetInverseMass();

	//모든 물체의 질량이 무한대이면 아무런 처리도 하지 않는다.
	if (TotalIverseMass <= 0)
		return;

	//물체가 옮겨갈 거리는 질량에 반비례한다.
	Vector3 MovePerIMass = ContactNormal * (Penetration / TotalIverseMass);

	//물체가 옮겨갈 거리를 계산한다.
	ParticleMovement[0] = MovePerIMass * Particles[0]->GetInverseMass();
	if (Particles[1])
	{
		ParticleMovement[1] = MovePerIMass * -Particles[1]->GetInverseMass();
	}
	else
	{
		ParticleMovement[1].Clear();
	}

	//물체를 옮긴다.
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
		//접근 속도가 가장 큰 접촉을 찾는다.
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
		//오류 체크
		if(MaxIndex == NumContacts)
			break;

		//이 접촉을 처리한다.
		ContactArray[MaxIndex].Resolve(Duration);

		//모든 입자의 겹친 부분을 처리한다.
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
