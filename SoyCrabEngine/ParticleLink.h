#pragma once
#include "SoyCrabEngine.h"

//�� ��ü�� ����δ� ���� ��.
//���� ���� ���� ���� ������ ���� ������ �Ͼ ������ ó���Ѵ�.
//�� Ŭ������ ��, ���� ���� �θ� Ŭ������ �Ǹ�, ź�� �Ѱ踦 ���� �������� �θ� Ŭ�����ε� ��� �����ϴ�.
class ParticleLink : public ParticleContactGenerator
{
public:
	//���ʿ� ����� ������ ��.
	Particle* Particles[2];

protected:
	//���� ���� ���� ���� ��ȯ.
	real CurrentLength() const;

public:
	//��ü�� ���� ���¸� �����ϱ� ���� ������ ������ �����Ѵ�.
	//�� Ŭ���������� �� ���� �� ���� ���˸� ������ �� �ְ�,
	//�� �����ʹ� ���� ��Ҹ� ����Ű�� �����͸� �� �� �ִ�.
	//���� limit�� �ּ��� 1 �̻��� �Ǿ�� �ϸ�, 0�� ��ȿ���� �ʴ�.
	//���� �����ϰ� �þ ���� ������ 0�� ��ȯ�ϰ�, ������ �ʿ��ϸ� 1�� ��ȯ�Ѵ�.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const = 0;
	//�ڵ� ����ȭ ������ ���� �浹 ��� �������̽��� �ٽ� �� �� ����.
};

//��(Cable)�� �� ��ü�� �����Ѵ�.
//�� ��ü�� �ʹ� �־����� ������ ����Ų��.
class ParticleCable : public ParticleLink
{
public:
	//���� �ִ� ����.
	real MaxLength;

	//���� �ݹ� ���(ź�¼�).
	real Restitution;

public:
	//���� �����ϰ� �þ�� �ʵ��� �ϴµ� �ʿ��� ������ �����Ѵ�.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};

//������ �� ��ü�� �����Ѵ�.
//������ ����� ��ü�� �ʹ� �־����ų� �ʹ� ���������, ������ ����Ų��.
class ParticleRod : public ParticleLink
{
public:
	//������� ����.
	real Length;

public:
	//����Ⱑ �þ�ų� �پ���� �ʵ��� �ϴµ� �ʿ��� ������ ����Ų��.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};

//����Ʈ����Ʈ�� �׳� ��ũ�� �Ȱ�����,
//�� ���� �ٸ����� �������� �ʴ� ��Ŀ ����Ʈ�� ����Ǿ��ִٴ� ���̴�.
class ParticleConstraint : public ParticleContactGenerator
{
public:
	//���� �� ����Ʈ����Ʈ�� ���� ���ڵ��� �迭.
	Particle* OtherParticle;

	//��Ŀ ����Ʈ
	Vector3 Anchor;

protected:
	//���� ��ũ�� ���̸� ��ȯ
	real CurrentLength() const;

public:
	//�� ��ũ�� ����� �ʵ��� ���������� ������ �����Ѵ�.
	//�� Ŭ������ ���� ������ ������ �� �����Ƿ�,
	//�����ʹ� ���� ��ҿ� ���� �����Ͱ� �� �� ������,
	//Limit�� �ּ� 1 (0�� ��ȿ���� ����)�� �����Ǹ�,
	//���̺��� �����ϰ� �þ�� ���� ��� ��ȯ���� 0�̰�,
	//������ �ʿ��� ��� 1�� ��ȯ�Ѵ�.
	//�̰� ���̽� Ŭ������, ���� Ŭ�����鿡�� ������.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const = 0;
};

//����Ʈ����Ʈ ���̺��� ��Ŀ ����Ʈ�� ���ڸ� ��ũ�صд�.
//�ʹ� �þ ��� ������ �߻���Ų��.
class ParticleCableConstraint : public ParticleConstraint
{
public:
	//���̺��� �ִ� ����
	real MaxLength;

	//���̺��� �ݹ߰�� (Bounciness)
	real Restitution;

public:
	//���̺��� �ʹ� �þ�� �ʵ���, �־��� ���˱���ü�� ���� ü���ִ´�.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};

//����Ʈ����Ʈ ������ ��Ŀ ����Ʈ�� ���ڸ� ��ũ�صд�.
//�ʹ� �þ ��� ������ �߻���Ų��.
class ParticleRodConstraint : public ParticleConstraint
{
public:
	//������� �ִ� ����
	real Length;

public:
	//����Ⱑ �þ�ų� �پ���� �ʵ���, �־��� ���˱���ü�� ���� ü���ִ´�.
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;
};