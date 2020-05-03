#pragma once
#include "Precision.h"
#include "Core.h"

class Particle
{

protected:
	
	//월드 공간에서 입자의 위치
	Vector3 Position;
	
	//월드 공간에서 입자의 속도
	Vector3 Velocity;
	
	//입자의 가속도
	Vector3 Acceleration;

	//직선 운동에 대한 댐핑(감쇄)의 정도를 저장.
	//적분기에서 수치 연산 오차로 인해 중가한 에너지를 줄여주는 역할을 함.
	real Damping;

	//입자 질량의 역수를 저장.
	//적분 과정이 더 간단하고 질량이 무한대인 물체(고정)가 질량이 0인 물체보다 쓰임새가 더 많으므로,
	//입자 질량은 역수를 갖는 것이 더 편리함.
	//(수치 시뮬레이션에서 완전히 불안정함)
	real InverseMass;

	//다음 시뮬레이션에 사용될 힘을 저장.
	//매 이터레이션마다 0으로 초기화 된다.
	Vector3 ForceAccum;

public:
	//질량 설정
	void SetMass(const real mass);
	//질량 가저오기
	real GetMass() const;
	//역질량 설정
	void SetInverseMass(const real inverseMass);
	//역질량 가저오기
	real GetInverseMass() const;

	//뎀핑(감쇄) 설정
	void SetDamping(const real damping);
	//뎀핑(감쇄) 가저오기
	real GetDamping() const;

	//위치 설정
	void SetPosition(const Vector3& position);
	void SetPosition(const real x, const real y, const real z);
	//위치 가저오기
	Vector3 GetPosition() const;
	void GetPosition(Vector3* position) const;
	//속도 설정
	void SetVelocity(const Vector3& velocity);
	void SetVelocity(const real x, const real y, const real z);
	//속도 가저오기
	Vector3 GetVelocity() const;
	void GetVelocity(Vector3* velocity) const;

	//가속도 설정
	void SetAcceleration(const Vector3& acceleration);
	void SetAcceleration(const real x, const real y, const real z);
	//가속도 가저오기
	Vector3 GetAcceleration() const;
	void GetAcceleration(Vector3* acceleration) const;

	void ClearAccumulator();
	void AddForce(const Vector3& force);
	//입자에 대하여, 주어진 시간 간격만큼 적분을 수행한다.
	//이 함수는 뉴턴-오일러 적분법을 사용하는데,
	//적분 오차를 보정하기 위해 선형 근사(Lienar Approximation)를 사용한다.
	//그래서 때로는 부정확할 수 있다.
	void Integrate(real duration);

	//Damping을 속도에 그냥 곱해버리는 방식
	void TEST_IntegrateNotFixed(real duration);

	//연습문제 3.3 에서 추가됨
	//운동에너지 계산
	real CalculateKineticEnergy();
};

