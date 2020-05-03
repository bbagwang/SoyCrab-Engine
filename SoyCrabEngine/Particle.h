#pragma once
#include "Precision.h"
#include "Core.h"

class Particle
{

protected:
	
	//���� �������� ������ ��ġ
	Vector3 Position;
	
	//���� �������� ������ �ӵ�
	Vector3 Velocity;
	
	//������ ���ӵ�
	Vector3 Acceleration;

	//���� ��� ���� ����(����)�� ������ ����.
	//���б⿡�� ��ġ ���� ������ ���� �߰��� �������� �ٿ��ִ� ������ ��.
	real Damping;

	//���� ������ ������ ����.
	//���� ������ �� �����ϰ� ������ ���Ѵ��� ��ü(����)�� ������ 0�� ��ü���� ���ӻ��� �� �����Ƿ�,
	//���� ������ ������ ���� ���� �� ����.
	//(��ġ �ùķ��̼ǿ��� ������ �Ҿ�����)
	real InverseMass;

	//���� �ùķ��̼ǿ� ���� ���� ����.
	//�� ���ͷ��̼Ǹ��� 0���� �ʱ�ȭ �ȴ�.
	Vector3 ForceAccum;

public:
	//���� ����
	void SetMass(const real mass);
	//���� ��������
	real GetMass() const;
	//������ ����
	void SetInverseMass(const real inverseMass);
	//������ ��������
	real GetInverseMass() const;

	//����(����) ����
	void SetDamping(const real damping);
	//����(����) ��������
	real GetDamping() const;

	//��ġ ����
	void SetPosition(const Vector3& position);
	void SetPosition(const real x, const real y, const real z);
	//��ġ ��������
	Vector3 GetPosition() const;
	void GetPosition(Vector3* position) const;
	//�ӵ� ����
	void SetVelocity(const Vector3& velocity);
	void SetVelocity(const real x, const real y, const real z);
	//�ӵ� ��������
	Vector3 GetVelocity() const;
	void GetVelocity(Vector3* velocity) const;

	//���ӵ� ����
	void SetAcceleration(const Vector3& acceleration);
	void SetAcceleration(const real x, const real y, const real z);
	//���ӵ� ��������
	Vector3 GetAcceleration() const;
	void GetAcceleration(Vector3* acceleration) const;

	void ClearAccumulator();
	void AddForce(const Vector3& force);
	//���ڿ� ���Ͽ�, �־��� �ð� ���ݸ�ŭ ������ �����Ѵ�.
	//�� �Լ��� ����-���Ϸ� ���й��� ����ϴµ�,
	//���� ������ �����ϱ� ���� ���� �ٻ�(Lienar Approximation)�� ����Ѵ�.
	//�׷��� ���δ� ����Ȯ�� �� �ִ�.
	void Integrate(real duration);

	//Damping�� �ӵ��� �׳� ���ع����� ���
	void TEST_IntegrateNotFixed(real duration);

	//�������� 3.3 ���� �߰���
	//������� ���
	real CalculateKineticEnergy();
};

