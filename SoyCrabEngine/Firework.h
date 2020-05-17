#pragma once
#include "SoyCrabEngine.h"

static Random crandom;

class Firework : public Particle
{
public:
	//불꽃 입자의 계산 규칙을 정하는 정수 값
	unsigned type;

	//입자의 수명을 계산해 언제 폭발할지를 정한다.
	//수명은 조금씩 감소시키고 0이 되면 화약을 터트린다.
	//수명을 심지의 남은 시간으로 생각하면 된다.
	real lifetime;

	bool update(real duration)
	{
		//물리 업데이트
		Integrate(duration);

		//수명 감소
		lifetime -= duration;
		return (lifetime < 0) || (Position.Y < 0);
	}
};

struct FireworkRule
{
	//이 규칙이 적용되는 불꽃의 종류
	unsigned type;
	//심지의 최소 지속시간
	real minLifeTime;
	//심지의 최대 지속시간
	real maxLifeTime;
	//불꽃 입자의 최소 상대 속도
	Vector3 minVelocity;
	//불꽃 입자의 최대 상대 속도
	Vector3 maxVelocity;
	//불꽃의 댐핑 계수
	real damping;

	struct Payload
	{
		//생성할 새로운 입자의 종류
		unsigned type;
		//이 페이로드 입자의 계수
		unsigned count;
		//페이로드의 속성 설정
		void set(unsigned type, unsigned count)
		{
			Payload::type = type;
			Payload::count = count;
		}
	};

	//이 불꽃 종류에 대한 페이로드 수
	unsigned payloadCount;
	//페이로드의 집합
	Payload* payloads;

	FireworkRule() :payloadCount(0), payloads(nullptr) {}
	~FireworkRule() { if (payloads != nullptr) delete[] payloads; }

	void init(unsigned payloadCount)
	{
		FireworkRule::payloadCount = payloadCount;
		payloads = new Payload[payloadCount];
	}

	//규칙 변수들 한 방에 설정
	void SetParameters(unsigned type, real minLifeTime, real maxLifeTime, const Vector3& minVelocity, const Vector3& maxVelocity, real damping)
	{
		FireworkRule::type = type;
		FireworkRule::minLifeTime = minLifeTime;
		FireworkRule::maxLifeTime = maxLifeTime;
		FireworkRule::minVelocity = minVelocity;
		FireworkRule::maxVelocity = maxVelocity;
		FireworkRule::damping = damping;
	}

	void create(Firework* firework, const Firework* parent = nullptr) const 
	{
		firework->type = type;
		firework->lifetime = crandom.randomReal(minLifeTime, maxLifeTime);

		Vector3 vel;
		if (parent)
		{
			//부모 기준의 위치와 속도을 가저옴
			firework->SetPosition(parent->GetPosition());
			vel += parent->GetVelocity();
		}
		else
		{
			Vector3 start;
			int x = (int)crandom.randomInt(3) - 1;
			start.X = 5.0f * real(x);
			firework->SetPosition(start);
		}

		vel += crandom.randomVector(minVelocity, maxVelocity);
		firework->SetVelocity(vel);

		//불꽃 파티클이 질량을 갖는게 딱히 의미가 없어서 중력에만 신경쓰면 되므로
		//모든 파티클이 하나의 질량을 갖도록 한다
		firework->SetMass(1);
		firework->SetDamping(damping);
		firework->SetAcceleration(Vector3::GRAVITY);
		firework->ClearAccumulator();
	}
};

class FireworksDemo : public Application
{
	//만들어질 불꽃 입자 최대 갯수
	const static unsigned maxFireworks = 1024;

	//불꽃 입자 데이터들 넣는 배열
	Firework fireworks[maxFireworks];

	//다음에 사용할 불꽃 입자 인덱스
	unsigned nextFireworks;

	//폭발 규칙 갯수
	const static unsigned ruleCount = 9;

	//폭발 규칙 데이터 저장 배열
	FireworkRule rules[ruleCount];

	//원점에서부터 폭죽을 발사한다.
	void create(unsigned type, const Firework* parent = nullptr);

	//주어진 부모의 원점으로부터 설정한 갯수만큼 폭죽을 더 생성한다
	void create(unsigned type, unsigned number, const Firework* parent = nullptr);

	//폭발 규칙 생성
	void initFireworkRules();

public:
	//불꽃놀이 데모 인스턴스 생성
	FireworksDemo();
	~FireworksDemo();

	//렌더링 초기화
	virtual void initGraphics();

	//창 이름 설정
	virtual const char* getTitle();

	//업데이트
	virtual void update();

	//렌더
	virtual void display();

	//입력
	virtual void key(unsigned char key);

	Application* getApplication()
	{
		return new FireworksDemo();
	}
};