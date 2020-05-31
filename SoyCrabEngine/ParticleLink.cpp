#include "SoyCrabEngine.h"

#pragma region ParticleLink
real ParticleLink::CurrentLength() const
{
	Vector3 RelativePos = Particles[0]->GetPosition() - Particles[1]->GetPosition();
	return RelativePos.Magnitude();
}
#pragma endregion

#pragma region ParticleCable
unsigned ParticleCable::AddContact(ParticleContact* Contact, unsigned Limit) const
{
	//끈의 길이 계산
	real Length = CurrentLength();

	//끈이 과도하게 늘어났는지 확인.
	if (Length < MaxLength)
		return 0;

	//그렇지 않으면, 접촉을 반환한다.
	Contact->Particles[0] = Particles[0];
	Contact->Particles[1] = Particles[1];

	//법선 벡터를 계산한다.
	Vector3 Normal = Particles[1]->GetPosition() - Particles[0]->GetPosition();
	Normal.Normalize();
	Contact->ContactNormal = Normal;
	Contact->Penetration = Length - MaxLength;
	Contact->Restitution = Restitution;

	return 1;
}
#pragma endregion

#pragma region ParticleRod
unsigned ParticleRod::AddContact(ParticleContact* Contact, unsigned Limit) const
{
	//막대기의 길이를 얻는다.
	real CurrentLen = CurrentLength();

	//과도하게 늘어났는지 검사.
	if (CurrentLen == Length)
		return 0;

	//그렇지 않으면, 접촉을 반환한다.
	Contact->Particles[0] = Particles[0];
	Contact->Particles[1] = Particles[1];

	//법선 벡터를 계산한다.
	Vector3 Normal = Particles[1]->GetPosition() - Particles[0]->GetPosition();
	Normal.Normalize();

	//접촉 법선 벡터의 방향은 막대기가 늘어나거나 줄어드는지 여부에 따라 달라진다.
	if (CurrentLen > Length)
	{
		Contact->ContactNormal = Normal;
		Contact->Penetration = CurrentLen - Length;
	}
	else
	{
		Contact->ContactNormal = Normal * -1;
		Contact->Penetration = Length - CurrentLen;
	}

	//물체가 튀지 않도록, 반발 계수는 언제나 0으로 둔다.
	Contact->Restitution = 0;
	return 1;
}
#pragma endregion

real ParticleConstraint::CurrentLength() const
{
	Vector3 RelativePos = OtherParticle->GetPosition() - Anchor;
	return RelativePos.Magnitude();
}

unsigned ParticleCableConstraint::AddContact(ParticleContact* Contact, unsigned Limit) const
{
	//끈의 길이 계산
	real Length = CurrentLength();

	//끈이 과도하게 늘어났는지 확인.
	if (Length < MaxLength)
		return 0;

	//그렇지 않으면, 접촉을 반환한다.
	Contact->Particles[0] = OtherParticle;
	Contact->Particles[1] = nullptr;

	//법선 벡터를 계산한다.
	Vector3 Normal = Anchor - OtherParticle->GetPosition();
	Normal.Normalize();
	Contact->ContactNormal = Normal;
	Contact->Penetration = Length - MaxLength;
	Contact->Restitution = Restitution;

	return 1;
}

unsigned ParticleRodConstraint::AddContact(ParticleContact* Contact, unsigned Limit) const
{
	//막대기의 길이를 얻는다.
	real CurrentLen = CurrentLength();

	//과도하게 늘어났는지 검사.
	if (CurrentLen == Length)
		return 0;

	//그렇지 않으면, 접촉을 반환한다.
	Contact->Particles[0] = OtherParticle;
	Contact->Particles[1] = nullptr;

	//법선 벡터를 계산한다.
	Vector3 Normal = Anchor - OtherParticle->GetPosition();
	Normal.Normalize();

	//접촉 법선 벡터의 방향은 막대기가 늘어나거나 줄어드는지 여부에 따라 달라진다.
	if (CurrentLen > Length)
	{
		Contact->ContactNormal = Normal;
		Contact->Penetration = CurrentLen - Length;
	}
	else
	{
		Contact->ContactNormal = Normal * -1;
		Contact->Penetration = Length - CurrentLen;
	}

	//물체가 튀지 않도록, 반발 계수는 언제나 0으로 둔다.
	Contact->Restitution = 0;
	return 1;
}