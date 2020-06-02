#pragma once
#include "SoyCrabEngine.h"

#define ROD_COUNT 15

#define BASE_MASS 1
#define EXTRA_MASS 10

class PlatformDemo : public MassAggregateApplication
{
	//막대기들
	ParticleRod* Rods;

	Vector3 MassPos;
	Vector3 MassDisplayPos;
	
	void UpdateAdditionalMass();

public:
	PlatformDemo();
	virtual ~PlatformDemo();

	virtual const char* getTitle();
	virtual void display();
	virtual void update();
	virtual void key(unsigned char key);
};

