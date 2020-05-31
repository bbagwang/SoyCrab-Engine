#include "SoyCrabEngine.h"

FireworksDemo::FireworksDemo():nextFireworks(0)
{
	//모든 폭죽을 미사용 상태로 초기화함
	for (Firework* firework = fireworks; firework < fireworks + maxFireworks; firework++)
	{
		firework->type = 0;
	}

	//폭죽 타입 생성
	initFireworkRules();
}

FireworksDemo::~FireworksDemo()
{

}

void FireworksDemo::initFireworkRules()
{
	// 돌면서 규칙 설정 진행
	rules[0].init(2);
	rules[0].SetParameters(
		1, // type
		0.5f, 1.4f, // age range
		Vector3(-5, 25, -5), // min velocity
		Vector3(5, 28, 5), // max velocity
		0.1 // damping
	);
	rules[0].payloads[0].set(3, 5);
	rules[0].payloads[1].set(5, 5);

	rules[1].init(1);
	rules[1].SetParameters(
		2, // type
		0.5f, 1.0f, // age range
		Vector3(-5, 10, -5), // min velocity
		Vector3(5, 20, 5), // max velocity
		0.8 // damping
	);
	rules[1].payloads[0].set(4, 2);

	rules[2].init(0);
	rules[2].SetParameters(
		3, // type
		0.5f, 1.5f, // age range
		Vector3(-5, -5, -5), // min velocity
		Vector3(5, 5, 5), // max velocity
		0.1 // damping
	);

	rules[3].init(0);
	rules[3].SetParameters(
		4, // type
		0.25f, 0.5f, // age range
		Vector3(-20, 5, -5), // min velocity
		Vector3(20, 5, 5), // max velocity
		0.2 // damping
	);

	rules[4].init(1);
	rules[4].SetParameters(
		5, // type
		0.5f, 1.0f, // age range
		Vector3(-20, 2, -5), // min velocity
		Vector3(20, 18, 5), // max velocity
		0.01 // damping
	);
	rules[4].payloads[0].set(3, 5);

	rules[5].init(0);
	rules[5].SetParameters(
		6, // type
		3, 5, // age range
		Vector3(-5, 5, -5), // min velocity
		Vector3(5, 10, 5), // max velocity
		0.95 // damping
	);

	rules[6].init(1);
	rules[6].SetParameters(
		7, // type
		4, 5, // age range
		Vector3(-5, 50, -5), // min velocity
		Vector3(5, 60, 5), // max velocity
		0.01 // damping
	);
	rules[6].payloads[0].set(8, 10);

	rules[7].init(0);
	rules[7].SetParameters(
		8, // type
		0.25f, 0.5f, // age range
		Vector3(-1, -1, -1), // min velocity
		Vector3(1, 1, 1), // max velocity
		0.01 // damping
	);

	rules[8].init(0);
	rules[8].SetParameters(
		9, // type
		3, 5, // age range
		Vector3(-15, 10, -5), // min velocity
		Vector3(15, 15, 5), // max velocity
		0.95 // damping
	);
	// 다른 타입 추가 필요시 위처럼 진행
}

void FireworksDemo::initGraphics()
{
	Application::initGraphics();

	//배경 컬러 변경
	glClearColor(0.f, 0.f, 0.1f, 1.f);
}

const char* FireworksDemo::getTitle()
{
	return "SoyCrab Engine - Fireworks";
}

void FireworksDemo::create(unsigned type, const Firework* parent /*= nullptr*/)
{
	//폭죽 생성을 위한 규칙을 가져옴
	FireworkRule* rule = rules + (type - 1);

	//폭죽 생성
	rule->create(fireworks + nextFireworks, parent);

	//다음 폭죽을 위해 인덱스 상승
	nextFireworks = (nextFireworks + 1) % maxFireworks;
}

void FireworksDemo::create(unsigned type, unsigned number, const Firework* parent /*= nullptr*/)
{
	for (unsigned i = 0; i < number; i++)
	{
		create(type, parent);
	}
}

void FireworksDemo::update()
{

	// Find the duration of the last frame in seconds
	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f) return;

	for (Firework* firework = fireworks;
		firework < fireworks + maxFireworks;
		firework++)
	{
		// Check if we need to process this firework.
		if (firework->type > 0)
		{
			// Does it need removing?
			if (firework->update(duration))
			{
				// Find the appropriate rule
				FireworkRule* rule = rules + (firework->type - 1);

				// Delete the current firework (this doesn't affect its
				// position and velocity for passing to the create function,
				// just whether or not it is processed for rendering or
				// physics.
				firework->type = 0;

				// Add the payload
				for (unsigned i = 0; i < rule->payloadCount; i++)
				{
					FireworkRule::Payload* payload = rule->payloads + i;
					create(payload->type, payload->count, firework);
				}
			}
		}
	}

	Application::update();

	////델타 타임 가져옴
	//float duration = (float)TimingData::get().lastFrameDuration * 0.001f;

	//if (duration <= 0.f) return;

	//for (Firework* firework = fireworks; fireworks < fireworks + maxFireworks; firework++)
	//{
	//	//처리해야할 불꽃인지 확인
	//	if (firework->type > 0)
	//	{
	//		//삭제해야할 불꽃인지 확인
	//		if (firework->update(duration))
	//		{
	//			//규칙 탐색
	//			FireworkRule* rule = rules + (firework->type - 1);

	//			//현재 불꽃을 삭제한다. (비활성 한다)
	//			//이 작업은 생성에서 위치나 속도에 영향을 미치지않는다.
	//			//렌더링이나 물리를 적용할지 말지에 대한 부분임.
	//			firework->type = 0;

	//			//페이로드 추가
	//			for (unsigned i = 0; i < rule->payloadCount; i++)
	//			{
	//				FireworkRule::Payload* payload = rule->payloads + i;
	//				create(payload->type, payload->count, firework);
	//			}
	//		}
	//	}
	//}

	////업데이트 함수 호출
	//Application::update();
}

void FireworksDemo::display()
{
	const static real size = 0.1f;

	//뷰포트 초기화 및 카메라 방향 설정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0f, 4.0f, 10.0f,
			  0.0f, 4.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	//각각의 폭죽을 업데이트한다.
	glBegin(GL_QUADS);
	for (Firework* firework = fireworks; firework < fireworks + maxFireworks; firework++)
	{
		if (firework->type > 0)
		{
			switch (firework->type)
			{
			case 1: glColor3f(1.f, 0.f, 0.f); break;
			case 2: glColor3f(1.f, 0.5f, 0.f); break;
			case 3: glColor3f(1.f, 1.f, 0.f); break;
			case 4: glColor3f(0.f, 1.f, 0.f); break;
			case 5: glColor3f(0.f, 1.f, 1.f); break;
			case 6: glColor3f(0.4f, 0.4f, 1.f); break;
			case 7: glColor3f(1.f, 0.f, 1.f); break;
			case 8: glColor3f(1.f, 1.f, 1.f); break;
			case 9: glColor3f(1.f, 0.5f, 0.5f); break;
			};

			const Vector3& pos = firework->GetPosition();
			glVertex3f(pos.X - size, pos.Y - size, pos.Z);
			glVertex3f(pos.X + size, pos.Y - size, pos.Z);
			glVertex3f(pos.X + size, pos.Y + size, pos.Z);
			glVertex3f(pos.X - size, pos.Y + size, pos.Z);

			//불꽃의 반사 반사를 그림
			glVertex3f(pos.X - size, -pos.Y - size, pos.Z);
			glVertex3f(pos.X + size, -pos.Y - size, pos.Z);
			glVertex3f(pos.X + size, -pos.Y + size, pos.Z);
			glVertex3f(pos.X - size, -pos.Y + size, pos.Z);
		}
	}
	glEnd();
}

void FireworksDemo::key(unsigned char key)
{
	//키 별 발사 타입 설정
	switch (key)
	{
	case '1': create(1, 1, NULL); break;
	case '2': create(2, 1, NULL); break;
	case '3': create(3, 1, NULL); break;
	case '4': create(4, 1, NULL); break;
	case '5': create(5, 1, NULL); break;
	case '6': create(6, 1, NULL); break;
	case '7': create(7, 1, NULL); break;
	case '8': create(8, 1, NULL); break;
	case '9': create(9, 1, NULL); break;
	}
}

Application* getApplication()
{
	return new FireworksDemo();
}