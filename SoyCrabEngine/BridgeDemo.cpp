#include "BridgeDemo.h"

BridgeDemo::BridgeDemo()
	:MassAggregateApplication(12), Cables(nullptr), Supports(nullptr), Rods(nullptr), MassPosition(0, 0, 0.5f)
{
	//질량과 연결을 생성한다.
	for (unsigned i = 0; i < 12; i++)
	{
		unsigned x = (i % 12) / 2;
		ParticleArray[i].SetPosition(
			real(i / 2) * 2.0f - 5.0f,	//X
			4,							//Y
			real(i % 2) * 2.0f - 1.0f	//Z
		);
		ParticleArray[i].SetVelocity(0, 0, 0);
		ParticleArray[i].SetDamping(0.9f);
		ParticleArray[i].SetAcceleration(Vector3::GRAVITY);
		ParticleArray[i].ClearAccumulator();
	}

	//링크 생성
	Cables = new ParticleCable[CABLE_COUNT];
	for (unsigned i = 0; i < 10; i++)
	{
		Cables[i].Particles[0] = &ParticleArray[i];
		Cables[i].Particles[1] = &ParticleArray[i + 2];
		Cables[i].MaxLength = 1.9f;
		Cables[i].Restitution = 0.3f;
		World.GetContactGenerators().push_back(&Cables[i]);
	}

	Supports = new ParticleCableConstraint[SUPPORT_COUNT];
	for (unsigned i = 0; i < SUPPORT_COUNT; i++)
	{
		Supports[i].OtherParticle = ParticleArray + i;
		Supports[i].Anchor = Vector3(
			real(i / 2) * 2.2f - 5.5f,
			6,
			real(i % 2) * 1.6f - 0.8f
		);
		if (i < 6) Supports[i].MaxLength = real(i / 2) * 0.5f + 3.0f;
		else Supports[i].MaxLength = 5.5f - real(i / 2) * 0.5f;
		Supports[i].Restitution = 0.5f;
		World.GetContactGenerators().push_back(&Supports[i]);
	}

	Rods = new ParticleRod[ROD_COUNT];
	for (unsigned i = 0; i < 6; i++)
	{
		Rods[i].Particles[0] = &ParticleArray[i * 2];
		Rods[i].Particles[1] = &ParticleArray[i * 2 + 1];
		Rods[i].Length = 2;
		World.GetContactGenerators().push_back(&Rods[i]);
	}

	UpdateAdditionalMass();
}

BridgeDemo::~BridgeDemo()
{
	if (Cables) delete[] Cables;
	if (Rods) delete[] Rods;
	if (Supports) delete[] Supports;
}

void BridgeDemo::UpdateAdditionalMass()
{
	for (unsigned i = 0; i < 12; i++)
	{
		ParticleArray[i].SetMass(BASE_MASS);
	}

	// Find the coordinates of the mass as an index and proportion
	int x = int(MassPosition.X);
	real xp = real_fmod(MassPosition.X, real(1.0f));
	if (x < 0)
	{
		x = 0;
		xp = 0;
	}
	if (x >= 5)
	{
		x = 5;
		xp = 0;
	}

	int z = int(MassPosition.Z);
	real zp = real_fmod(MassPosition.Z, real(1.0f));
	if (z < 0)
	{
		z = 0;
		zp = 0;
	}
	if (z >= 1)
	{
		z = 1;
		zp = 0;
	}

	//질량을 그릴 위치 계산
	MassDisplayPosition.Clear();

	//초기화된 질량에 비율을 추가한다.
	ParticleArray[x * 2 + z].SetMass(BASE_MASS + EXTRA_MASS * (1 - xp) * (1 - zp));
	MassDisplayPosition.AddScaledVector(
		ParticleArray[x * 2 + z].GetPosition(), (1 - xp) * (1 - zp)
	);

	if (xp > 0)
	{
		ParticleArray[x * 2 + z + 2].SetMass(BASE_MASS + EXTRA_MASS * xp * (1 - zp));
		MassDisplayPosition.AddScaledVector(
			ParticleArray[x * 2 + z + 2].GetPosition(), xp * (1 - zp)
		);

		if (zp > 0)
		{
			ParticleArray[x * 2 + z + 3].SetMass(BASE_MASS + EXTRA_MASS * xp * zp);
			MassDisplayPosition.AddScaledVector(
				ParticleArray[x * 2 + z + 3].GetPosition(), xp * zp
			);
		}
	}
	if (zp > 0)
	{
		ParticleArray[x * 2 + z + 1].SetMass(BASE_MASS + EXTRA_MASS * (1 - xp) * zp);
		MassDisplayPosition.AddScaledVector(
			ParticleArray[x * 2 + z + 1].GetPosition(), (1 - xp) * zp
		);
	}
}

const char* BridgeDemo::getTitle()
{
	return "SoyCrab Engine - Bridge!";
}

void BridgeDemo::display()
{
	MassAggregateApplication::display();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	for (unsigned i = 0; i < ROD_COUNT; i++)
	{
		Particle** particles = Rods[i].Particles;
		const Vector3& p0 = particles[0]->GetPosition();
		const Vector3& p1 = particles[1]->GetPosition();
		glVertex3f(p0.X, p0.Y, p0.Z);
		glVertex3f(p1.X, p1.Y, p1.Z);
	}

	glColor3f(0, 1, 0);
	for (unsigned i = 0; i < CABLE_COUNT; i++)
	{
		Particle** particles = Cables[i].Particles;
		const Vector3& p0 = particles[0]->GetPosition();
		const Vector3& p1 = particles[1]->GetPosition();
		glVertex3f(p0.X, p0.Y, p0.Z);
		glVertex3f(p1.X, p1.Y, p1.Z);
	}

	glColor3f(0.7f, 0.7f, 0.7f);
	for (unsigned i = 0; i < SUPPORT_COUNT; i++)
	{
		const Vector3& p0 = Supports[i].OtherParticle->GetPosition();
		const Vector3& p1 = Supports[i].Anchor;
		glVertex3f(p0.X, p0.Y, p0.Z);
		glVertex3f(p1.X, p1.Y, p1.Z);
	}
	glEnd();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(MassDisplayPosition.X, MassDisplayPosition.Y + 0.25f, MassDisplayPosition.Z);
	glutSolidSphere(0.25f, 20, 10);
	glPopMatrix();
}

void BridgeDemo::update()
{
	MassAggregateApplication::update();

	UpdateAdditionalMass();
}

void BridgeDemo::key(unsigned char key)
{
	switch (key)
	{
	case 's': case 'S':
		MassPosition.Z += 0.1f;
		if (MassPosition.Z > 1.0f) MassPosition.Z = 1.0f;
		break;
	case 'w': case 'W':
		MassPosition.Z -= 0.1f;
		if (MassPosition.Z < 0.0f) MassPosition.Z = 0.0f;
		break;
	case 'a': case 'A':
		MassPosition.X -= 0.1f;
		if (MassPosition.X < 0.0f) MassPosition.X = 0.0f;
		break;
	case 'd': case 'D':
		MassPosition.X += 0.1f;
		if (MassPosition.X > 5.0f) MassPosition.X = 5.0f;
		break;

	default:
		MassAggregateApplication::key(key);
	}
}

Application* getApplication()
{
	return new BridgeDemo();
}