#pragma once
#include "SoyCrabEngine.h"

#define ROD_COUNT 6
#define CABLE_COUNT 10
#define SUPPORT_COUNT 12

#define BASE_MASS 1
#define EXTRA_MASS 10

class BridgeDemo : public MassAggregateApplication
{
	//지지대
	ParticleCableConstraint* Supports;
	//연결선
	ParticleCable* Cables;
	//막대
	ParticleRod* Rods;

	Vector3 MassPosition;
	Vector3 MassDisplayPosition;

	//다리를 건너는 입자의 질량을 고려하여 입자의 질량을 업데이트한다.
	void UpdateAdditionalMass();

public:
	BridgeDemo();
	virtual ~BridgeDemo();

	virtual const char* getTitle();
	virtual void display();
	virtual void update();
	virtual void key(unsigned char key);
};

