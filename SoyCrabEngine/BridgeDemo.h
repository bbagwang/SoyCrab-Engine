#pragma once
#include "SoyCrabEngine.h"

#define ROD_COUNT 6
#define CABLE_COUNT 10
#define SUPPORT_COUNT 12

#define BASE_MASS 1
#define EXTRA_MASS 10

class BridgeDemo : public MassAggregateApplication
{
	//������
	ParticleCableConstraint* Supports;
	//���ἱ
	ParticleCable* Cables;
	//����
	ParticleRod* Rods;

	Vector3 MassPosition;
	Vector3 MassDisplayPosition;

	//�ٸ��� �ǳʴ� ������ ������ ����Ͽ� ������ ������ ������Ʈ�Ѵ�.
	void UpdateAdditionalMass();

public:
	BridgeDemo();
	virtual ~BridgeDemo();

	virtual const char* getTitle();
	virtual void display();
	virtual void update();
	virtual void key(unsigned char key);
};

