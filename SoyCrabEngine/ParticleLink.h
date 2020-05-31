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
	virtual unsigned AddContact(ParticleContact* Contact, unsigned Limit) const;

};