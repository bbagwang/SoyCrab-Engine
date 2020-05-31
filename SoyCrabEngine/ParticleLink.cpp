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
	//���� ���� ���
	real Length = CurrentLength();

	//���� �����ϰ� �þ���� Ȯ��.
	if (Length < MaxLength)
		return 0;

	//�׷��� ������, ������ ��ȯ�Ѵ�.
	Contact->Particles[0] = Particles[0];
	Contact->Particles[1] = Particles[1];

	//���� ���͸� ����Ѵ�.
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
	//������� ���̸� ��´�.
	real CurrentLen = CurrentLength();

	//�����ϰ� �þ���� �˻�.
	if (CurrentLen == Length)
		return 0;

	//�׷��� ������, ������ ��ȯ�Ѵ�.
	Contact->Particles[0] = Particles[0];
	Contact->Particles[1] = Particles[1];

	//���� ���͸� ����Ѵ�.
	Vector3 Normal = Particles[1]->GetPosition() - Particles[0]->GetPosition();
	Normal.Normalize();

	//���� ���� ������ ������ ����Ⱑ �þ�ų� �پ����� ���ο� ���� �޶�����.
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

	//��ü�� Ƣ�� �ʵ���, �ݹ� ����� ������ 0���� �д�.
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
	//���� ���� ���
	real Length = CurrentLength();

	//���� �����ϰ� �þ���� Ȯ��.
	if (Length < MaxLength)
		return 0;

	//�׷��� ������, ������ ��ȯ�Ѵ�.
	Contact->Particles[0] = OtherParticle;
	Contact->Particles[1] = nullptr;

	//���� ���͸� ����Ѵ�.
	Vector3 Normal = Anchor - OtherParticle->GetPosition();
	Normal.Normalize();
	Contact->ContactNormal = Normal;
	Contact->Penetration = Length - MaxLength;
	Contact->Restitution = Restitution;

	return 1;
}

unsigned ParticleRodConstraint::AddContact(ParticleContact* Contact, unsigned Limit) const
{
	//������� ���̸� ��´�.
	real CurrentLen = CurrentLength();

	//�����ϰ� �þ���� �˻�.
	if (CurrentLen == Length)
		return 0;

	//�׷��� ������, ������ ��ȯ�Ѵ�.
	Contact->Particles[0] = OtherParticle;
	Contact->Particles[1] = nullptr;

	//���� ���͸� ����Ѵ�.
	Vector3 Normal = Anchor - OtherParticle->GetPosition();
	Normal.Normalize();

	//���� ���� ������ ������ ����Ⱑ �þ�ų� �پ����� ���ο� ���� �޶�����.
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

	//��ü�� Ƣ�� �ʵ���, �ݹ� ����� ������ 0���� �д�.
	Contact->Restitution = 0;
	return 1;
}