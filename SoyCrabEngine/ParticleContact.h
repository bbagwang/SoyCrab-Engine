#pragma once
#include "SoyCrabEngine.h"

class ParticleContactResolver;

//�� ��ü�� ������ ó���Ѵ�.
//������ ó���ϱ� ���ؼ��� ��ü �� ������ �����ϰ�,
//������ �ݹ߷��� �����Ͽ� ��ü�� ������ƾ� �Ѵ�.
//�����ϴ� ��ü�� �ٽ� ƥ ���� �ִ�.
//���˿��� ȣ���� �� �ִ� �Լ��� ����,
//������ ���γ����� ���� ���� ���̴�.
//������ ó���Ϸ��� ���� ���� �ؼ� Ŭ������ ����Ѵ�.

class ParticleContact
{

	//ContactResolver�� Contact�� ������ �ۿ��ؾ� �ϹǷ� friend ������.
	friend class ParticleContactResolver;

	//���˿� ���ԵǴ� ���ڵ��� �����Ѵ�.
	//���� ���˽� NULL �� �� �� �ִ�.
	Particle* Particles[2];

	//������������ �ݹ� ���.
	real Restitution;
	
	//���� ��ǥ�� ���� ���� ���� ����.
	Vector3 ContactNormal;

	//������ ��� �󸶳� ������ �ִ����� �����Ѵ�.
	real Penetration;

	//�� ���ڰ� �����ϴµ��� �̵��� ���� �����Ѵ�.
	Vector3 ParticleMovement[2];

protected:
	//�ӵ��� ��ġ�� �κ��� ������Ʈ�Ͽ� ������ �ؼ��Ѵ�.
	void Resolve(real duration);
	//�������� ���� �и� �ӵ��� ����Ѵ�.
	real CalculateSeparatingVelocity() const;

private:
	//���˿� ���� ��ݷ� ����� ó���Ѵ�.
	void ResolveVelocity(real duration);
	//���˿� ���� ��ġ�� �κ��� ó���Ѵ�.
	void ResolveInterpenetration(real duration);
};

