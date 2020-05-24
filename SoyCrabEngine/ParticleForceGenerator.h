#pragma once
#include "SoyCrabEngine.h"

class ParticleForceGenerator
{
public:
	//�־��� ���ڿ� �����ϴ� ���� ����ϰ� ������Ʈ �ϱ� ���� �� �޼��带 ������
	virtual void UpdateForce(Particle* Particle, real Duration) = 0;
};

#pragma region GravityGenerator
//�߷��� �����ϴ� �� �߻���
//�ν��Ͻ� �ϳ��� ���� ���� ���ڿ� ��� ����.

//�����ϴٴ� ������ ������, ���ΰ� �߷� ���ӵ��� ���� �ڵ� ���.
//������ �� �����Ӹ��� �̰� ������ ����ؾ��� �ʿ䰡 ���� ����.
//�̹� ���� �˰��ִ� ����� �ϴ� �Ͱ� �ٸ��� ����.
class ParticleGravityGenerator : public ParticleForceGenerator
{
private:
	//�߷� ���ӵ�
	Vector3 Gravity;
public:
	//�־��� ���ӵ��� �� �߻��� �ν��Ͻ� ����
	ParticleGravityGenerator(const Vector3& Gravity) : Gravity(Gravity) {}
	//�־��� ���ڿ� �߷� ����
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region DragGenerator
//�巹�� ���� �����ϴ� �� �߻���
//�ν��Ͻ� �ϳ��� ���� ���� ���ڿ� ��� ����.
class ParticleDragGenerator : public ParticleForceGenerator
{
	//Drag Force ��
	//�巹�� �� = -�ӵ�(�巹�� ���1(k1)*(���� �ӵ��� ũ��)+�巹�� ���2(k2)*(���� �ӵ��� ũ��)^2)
private:
	//Drag Coefficient (�巹�� ���)
	//�ӵ��� �������� �巡�� ��ʻ�� ����
	real k1;
	//�ӵ��� ������ �������� �巹�� ��ʻ�� ����
	//k2���� Ŭ���� �ӵ��� �������� ���� �ް��� �巹�װ� ������.
	//�ӵ��� �ι� �������� ���������� 4�谡 �Ǵ� �Ͱ� ���� �ƶ�.
	real k2;
public:
	//�־��� ��ʻ���� �� �߻��� ��ü ����
	ParticleDragGenerator(real k1, real k2) : k1(k1), k2(k2) {}
	//�־��� ���ڿ� �巹�� �� ����
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region SpringGenerator
//������ ���� �����ϴ� �� �߻���
class ParticleSpringGenerator : public ParticleForceGenerator
{
private:
	//�������� �ݴ��� ���� ����
	Particle* Other;
	//������ ���
	real SpringConstant;
	//������ ���� ����
	real RestLength;
public:
	//�־��� ���ڵ��� ���� ���ο� ������ ��ü�� �����ϴ� ������
	ParticleSpringGenerator(Particle* Other, real SpringConstant, real RestLength)
		:Other(Other), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//�־��� ���ڿ� ������ ���� �����Ѵ�.
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region ParticleAnchoredSpringGenerator
//���� ���� �������� ����� �������� ���� �����ϴ� �� �߻���
class ParticleAnchoredSpringGenerator : public ParticleForceGenerator
{
private:
	//������ ������ ��ġ
	Vector3* Anchor;
	//������ ���
	real SpringConstant;
	//������ ���� ����
	real RestLength;
public:
	//�־��� ���ڸ� �������� ���ο� ������ �ν��Ͻ��� �����ϴ� ������
	ParticleAnchoredSpringGenerator(Vector3* Anchor, real SpringConstant, real RestLength) :
		Anchor(Anchor), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//�־��� ���ڿ� ������ ���� �����Ѵ�.
	virtual void UpdateForce(Particle* Particle, real Duration);
	void SetAnchor(Vector3* NewAnchorPosition) { Anchor = NewAnchorPosition; }
	Vector3 GetAnchor() const { return *Anchor; }
};
#pragma endregion

#pragma region ParticleBungeeGenerator
//��� �ÿ��� ������ ������ ���� �����ϴ� �� �߻���
class ParticleBungeeGenerator : public ParticleForceGenerator
{
private:
	//������ �ݴ��� ���� �ִ� ����
	Particle* Other;
	//������ ���
	real SpringConstant;
	//���� �߻���Ű�� �����ϴ� �������� ������ ����
	real RestLength;
public:
	//�־��� ���ڸ� �������� ���ο� ������ �����ϴ� ������
	ParticleBungeeGenerator(Particle* Other, real SpringConstant, real RestLength) :
		Other(Other), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//�־��� ���ڿ� ������ ���� �����Ѵ�.
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region ParticleAnchoredBungeeGenerator
//���� ���� �������� ����� ��� �ÿ��� ������ �������� ���� �����ϴ� �� �߻���
class ParticleAnchoredBungeeGenerator : public ParticleForceGenerator
{
private:
	//������ ������ ��ġ
	Vector3* Anchor;
	//������ ���
	real SpringConstant;
	//���� �߻���Ű�� �����ϴ� �������� ������ ����
	real RestLength;
public:
	//�־��� ���ڸ� �������� ���ο� ������ �����ϴ� ������
	ParticleAnchoredBungeeGenerator(Vector3* Anchor, real SpringConstant, real RestLength) :
		Anchor(Anchor), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//�־��� ���ڿ� ������ ���� �����Ѵ�.
	virtual void UpdateForce(Particle* Particle, real Duration);
	void SetAnchor(Vector3* NewAnchorPosition) { Anchor = NewAnchorPosition; }
	Vector3 GetAnchor() const { return *Anchor; }
};
#pragma endregion

#pragma region ParticleBuoyancyGenerator
//XZ ��鿡 ������ ���鿡 ���� �η��� �����ϴ� �� �߻���
class ParticleBuoyancyGenerator : public ParticleForceGenerator
{
private:
	//�ִ� �η��� �߻���Ű�� �� ��ü�� �ִ� ħ�� ����
	real MaxDepth;
	//��ü�� ����
	real Volume;
	//������ y = 0 �� ������κ��� �̵��� ����
	//������ XZ ��鿡 ������ ������ ����.
	real WaterHeight;
	//��ü�� �е�
	//������ ���� �е��� 1000 kg/m^(-3)
	real LiquidDensity = 1000.f;
public:
	//�־��� ���ڸ� �������� ���ο� �η� ��ü ����.
	ParticleBuoyancyGenerator(real MaxDepth, real Volume, real WaterHeight, real LiquidDensity = 1000.f)
		:MaxDepth(MaxDepth), Volume(Volume), WaterHeight(WaterHeight), LiquidDensity(LiquidDensity) {}
	//�־��� ���ڿ� �η� ����
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion