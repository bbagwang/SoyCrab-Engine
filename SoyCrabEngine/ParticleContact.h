#pragma once
#include "SoyCrabEngine.h"

//두 물체의 접촉을 처리한다.
//접촉을 처리하기 위해서는 물체 간 간섭을 제거하고,
//적절한 반발력을 적용하여 물체를 떼어놓아야 한다.
//접촉하는 물체가 다시 튈 때도 있다.
//접촉에는 호출할 수 있는 함수가 없고,
//접촉의 세부내용을 갖고 있을 뿐이다.
//접촉을 처리하려면 입자 접촉 해소 클래스를 사용한다.

class ParticleContact
{
	//접촉에 포함되는 입자들을 저장한다.
	//배경과 접촉시 NULL 이 될 수 있다.
	Particle* Particles[2];

	//접촉점에서의 반발 계수.
	real Restitution;
	
	//월드 좌표계 기준 접촉 방향 벡터.
	Vector3 ContactNormal;
};

