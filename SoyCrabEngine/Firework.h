#pragma once
#include "SoyCrabEngine.h"

static Random crandom;

class Firework : public Particle
{
public:
	//�Ҳ� ������ ��� ��Ģ�� ���ϴ� ���� ��
	unsigned type;

	//������ ������ ����� ���� ���������� ���Ѵ�.
	//������ ���ݾ� ���ҽ�Ű�� 0�� �Ǹ� ȭ���� ��Ʈ����.
	//������ ������ ���� �ð����� �����ϸ� �ȴ�.
	real lifetime;

	bool update(real duration)
	{
		//���� ������Ʈ
		Integrate(duration);

		//���� ����
		lifetime -= duration;
		return (lifetime < 0) || (Position.Y < 0);
	}
};

struct FireworkRule
{
	//�� ��Ģ�� ����Ǵ� �Ҳ��� ����
	unsigned type;
	//������ �ּ� ���ӽð�
	real minLifeTime;
	//������ �ִ� ���ӽð�
	real maxLifeTime;
	//�Ҳ� ������ �ּ� ��� �ӵ�
	Vector3 minVelocity;
	//�Ҳ� ������ �ִ� ��� �ӵ�
	Vector3 maxVelocity;
	//�Ҳ��� ���� ���
	real damping;

	struct Payload
	{
		//������ ���ο� ������ ����
		unsigned type;
		//�� ���̷ε� ������ ���
		unsigned count;
		//���̷ε��� �Ӽ� ����
		void set(unsigned type, unsigned count)
		{
			Payload::type = type;
			Payload::count = count;
		}
	};

	//�� �Ҳ� ������ ���� ���̷ε� ��
	unsigned payloadCount;
	//���̷ε��� ����
	Payload* payloads;

	FireworkRule() :payloadCount(0), payloads(nullptr) {}
	~FireworkRule() { if (payloads != nullptr) delete[] payloads; }

	void init(unsigned payloadCount)
	{
		FireworkRule::payloadCount = payloadCount;
		payloads = new Payload[payloadCount];
	}

	//��Ģ ������ �� �濡 ����
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
			//�θ� ������ ��ġ�� �ӵ��� ������
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

		//�Ҳ� ��ƼŬ�� ������ ���°� ���� �ǹ̰� ��� �߷¿��� �Ű澲�� �ǹǷ�
		//��� ��ƼŬ�� �ϳ��� ������ ������ �Ѵ�
		firework->SetMass(1);
		firework->SetDamping(damping);
		firework->SetAcceleration(Vector3::GRAVITY);
		firework->ClearAccumulator();
	}
};

class FireworksDemo : public Application
{
	//������� �Ҳ� ���� �ִ� ����
	const static unsigned maxFireworks = 1024;

	//�Ҳ� ���� �����͵� �ִ� �迭
	Firework fireworks[maxFireworks];

	//������ ����� �Ҳ� ���� �ε���
	unsigned nextFireworks;

	//���� ��Ģ ����
	const static unsigned ruleCount = 9;

	//���� ��Ģ ������ ���� �迭
	FireworkRule rules[ruleCount];

	//������������ ������ �߻��Ѵ�.
	void create(unsigned type, const Firework* parent = nullptr);

	//�־��� �θ��� �������κ��� ������ ������ŭ ������ �� �����Ѵ�
	void create(unsigned type, unsigned number, const Firework* parent = nullptr);

	//���� ��Ģ ����
	void initFireworkRules();

public:
	//�Ҳɳ��� ���� �ν��Ͻ� ����
	FireworksDemo();
	~FireworksDemo();

	//������ �ʱ�ȭ
	virtual void initGraphics();

	//â �̸� ����
	virtual const char* getTitle();

	//������Ʈ
	virtual void update();

	//����
	virtual void display();

	//�Է�
	virtual void key(unsigned char key);

	Application* getApplication()
	{
		return new FireworksDemo();
	}
};