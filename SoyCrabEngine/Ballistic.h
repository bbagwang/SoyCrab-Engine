#pragma once
#include "SoyCrabEngine.h"

class BallisticDemo : public Application
{
	enum ShotType
	{
		UNUSED = 0,
		PISTOL,
		ARTILLERY,
		FIREBALL,
		LASER
	};

	struct AmmoRound
	{
		Particle particle;
		ShotType type;
		unsigned startTime;

		void render()
		{
			Vector3 position;
			particle.GetPosition(&position);

			glColor3f(0, 0, 0);
			glPushMatrix();
			glTranslatef(position.X, position.Y, position.Z);
			glutSolidSphere(0.3f, 5.f, 4.f);
			glPopMatrix();

			glColor3f(0.75f, 0.75f, 0.75f);
			glPushMatrix();
			glTranslatef(position.X, 0.f, position.Z);
			glScalef(1.0f, 0.1f, 1.0f);
			glutSolidSphere(0.6f, 5.f, 4.f);
			glPopMatrix();
		}
	};

	//�ִ� �߻� ����
	const static unsigned ammoRounds = 16;

	//ź�� ������ �迭
	AmmoRound ammo[ammoRounds];

	//���� �߻� Ÿ��
	ShotType currentShotType;

	//�߻�
	void fire();

public:
	BallisticDemo();

	virtual const char* getTitle();
	virtual void update();
	virtual void display();
	virtual void mouse(int button, int state, int x, int y);
	virtual void key(unsigned char key);
};