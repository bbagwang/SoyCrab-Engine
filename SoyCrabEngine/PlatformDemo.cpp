#include "PlatformDemo.h"

void PlatformDemo::UpdateAdditionalMass()
{
	for (unsigned i = 2; i < 6; i++)
	{
		ParticleArray[i].SetMass(BASE_MASS);
	}

	// 인덱스와 질량을 통해 질량 위치를 찾음.
	real xp = MassPos.X;
	if (xp < 0) xp = 0;
	if (xp > 1) xp = 1;

	real zp = MassPos.Z;
	if (zp < 0) zp = 0;
	if (zp > 1) zp = 1;

	// 질량을 그릴 위치 계산~
	MassDisplayPos.Clear();

	// 정확한 질량 비율을 추가함.
	ParticleArray[2].SetMass(BASE_MASS + EXTRA_MASS * (1 - xp) * (1 - zp));
	MassDisplayPos.AddScaledVector(
		ParticleArray[2].GetPosition(), (1 - xp) * (1 - zp)
	);

	if (xp > 0)
	{
		ParticleArray[4].SetMass(BASE_MASS + EXTRA_MASS * xp * (1 - zp));
		MassDisplayPos.AddScaledVector(
			ParticleArray[4].GetPosition(), xp * (1 - zp)
		);

		if (zp > 0)
		{
			ParticleArray[5].SetMass(BASE_MASS + EXTRA_MASS * xp * zp);
			MassDisplayPos.AddScaledVector(
				ParticleArray[5].GetPosition(), xp * zp
			);
		}
	}
	if (zp > 0)
	{
		ParticleArray[3].SetMass(BASE_MASS + EXTRA_MASS * (1 - xp) * zp);
		MassDisplayPos.AddScaledVector(
			ParticleArray[3].GetPosition(), (1 - xp) * zp
		);
	}
}

PlatformDemo::PlatformDemo() : MassAggregateApplication(6), Rods(nullptr), MassPos(0, 0, 0.5f)
{
	//입자, 질량 및 링크를 설정한다.
	ParticleArray[0].SetPosition(0, 0, 1);
	ParticleArray[1].SetPosition(0, 0, -1);
	ParticleArray[2].SetPosition(-3, 2, 1);
	ParticleArray[3].SetPosition(-3, 2, -1);
	ParticleArray[4].SetPosition(4, 2, 1);
	ParticleArray[5].SetPosition(4, 2, -1);
	for (unsigned i = 0; i < 6; i++)
	{
		ParticleArray[i].SetMass(BASE_MASS);
		ParticleArray[i].SetVelocity(0, 0, 0);
		ParticleArray[i].SetDamping(0.9f);
		ParticleArray[i].SetAcceleration(Vector3::GRAVITY);
		ParticleArray[i].ClearAccumulator();
	}

	//막대기 생성
	Rods = new ParticleRod[ROD_COUNT];

	Rods[0].Particles[0] = &ParticleArray[0];
	Rods[0].Particles[1] = &ParticleArray[1];
	Rods[0].Length = 2;

	Rods[1].Particles[0] = &ParticleArray[2];
	Rods[1].Particles[1] = &ParticleArray[3];
	Rods[1].Length = 2;

	Rods[2].Particles[0] = &ParticleArray[4];
	Rods[2].Particles[1] = &ParticleArray[5];
	Rods[2].Length = 2;

	Rods[3].Particles[0] = &ParticleArray[2];
	Rods[3].Particles[1] = &ParticleArray[4];
	Rods[3].Length = 7;

	Rods[4].Particles[0] = &ParticleArray[3];
	Rods[4].Particles[1] = &ParticleArray[5];
	Rods[4].Length = 7;

	Rods[5].Particles[0] = &ParticleArray[0];
	Rods[5].Particles[1] = &ParticleArray[2];
	Rods[5].Length = 3.606;

	Rods[6].Particles[0] = &ParticleArray[1];
	Rods[6].Particles[1] = &ParticleArray[3];
	Rods[6].Length = 3.606;

	Rods[7].Particles[0] = &ParticleArray[0];
	Rods[7].Particles[1] = &ParticleArray[4];
	Rods[7].Length = 4.472;

	Rods[8].Particles[0] = &ParticleArray[1];
	Rods[8].Particles[1] = &ParticleArray[5];
	Rods[8].Length = 4.472;

	Rods[9].Particles[0] = &ParticleArray[0];
	Rods[9].Particles[1] = &ParticleArray[3];
	Rods[9].Length = 4.123;

	Rods[10].Particles[0] = &ParticleArray[2];
	Rods[10].Particles[1] = &ParticleArray[5];
	Rods[10].Length = 7.28;

	Rods[11].Particles[0] = &ParticleArray[4];
	Rods[11].Particles[1] = &ParticleArray[1];
	Rods[11].Length = 4.899;

	Rods[12].Particles[0] = &ParticleArray[1];
	Rods[12].Particles[1] = &ParticleArray[2];
	Rods[12].Length = 4.123;

	Rods[13].Particles[0] = &ParticleArray[3];
	Rods[13].Particles[1] = &ParticleArray[4];
	Rods[13].Length = 7.28;

	Rods[14].Particles[0] = &ParticleArray[5];
	Rods[14].Particles[1] = &ParticleArray[0];
	Rods[14].Length = 4.899;

	//막대기 접촉 등록
	for (unsigned i = 0; i < ROD_COUNT; i++)
	{
		World.GetContactGenerators().push_back(&Rods[i]);
	}

	UpdateAdditionalMass();
}

PlatformDemo::~PlatformDemo()
{
	if (Rods)
		delete[] Rods;
}

const char* PlatformDemo::getTitle()
{
	return "SoyCrab Engine - Mass Platform";
}

void PlatformDemo::display()
{
	MassAggregateApplication::display();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	for (unsigned i = 0; i < ROD_COUNT; i++)
	{
		Particle** Particles = Rods[i].Particles;
		const Vector3& p0 = Particles[0]->GetPosition();
		const Vector3& p1 = Particles[1]->GetPosition();
		glVertex3f(p0.X, p0.Y, p0.Z);
		glVertex3f(p1.X, p1.Y, p1.Z);
	}
	glEnd();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(MassDisplayPos.X, MassDisplayPos.Y + 0.25f, MassDisplayPos.Z);
	glutSolidSphere(0.25f, 20, 10);
	glPopMatrix();
}

void PlatformDemo::update()
{
	MassAggregateApplication::update();

	UpdateAdditionalMass();
}

void PlatformDemo::key(unsigned char key)
{
	switch (key)
	{
	case 'w': case 'W':
		MassPos.Z += 0.1f;
		if (MassPos.Z > 1.0f) MassPos.Z = 1.0f;
		break;
	case 's': case 'S':
		MassPos.Z -= 0.1f;
		if (MassPos.Z < 0.0f) MassPos.Z = 0.0f;
		break;
	case 'a': case 'A':
		MassPos.X -= 0.1f;
		if (MassPos.X < 0.0f) MassPos.X = 0.0f;
		break;
	case 'd': case 'D':
		MassPos.X += 0.1f;
		if (MassPos.X > 1.0f) MassPos.X = 1.0f;
		break;

	default:
		MassAggregateApplication::key(key);
	}
}

Application* getApplication()
{
	return new PlatformDemo();
}