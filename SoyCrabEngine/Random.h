#pragma once

#include "Core.h"

class Random
{
public:
	//Bit-Wise 왼쪽 회전
	unsigned rotl(unsigned n, unsigned r);

	//Bit-Wise 오른쪽 회전
	unsigned rotr(unsigned n, unsigned r);

	Random();

	//받아온 시드로 새로운 랜덤 수 생성
	Random(unsigned seed);
	
	//시드 설정
	void seed(unsigned seed);

	//새로운 랜덤 비트 스트림 생성 (가장 빠른 방법)
	unsigned randomBits();

	//0 ~ 1 사이 랜덤 실수 생성
	real randomReal();
	
	//0 ~ Scale 사이 랜던 실수 생성
	real randomReal(real scale);

	//min ~ max 사이 랜덤 실수 생성
	real randomReal(real min, real max);

	//0 ~ max 사이 랜덤 정수 생성
	unsigned randomInt(unsigned max);

	//-scale ~ +scale 까지 이산 확률 분포에 따른 실수 값을 가저온다.
	real randomBinomial(real scale);

	//-scale ~ +scale 까지 각각의 요소가 이산 확률 분포에 따른 벡터 값을 가저온다.
	Vector3 randomVector(real scale);

	//-scale ~ +scale 까지 받아온 벡터에 대해 각각의 요소가 이산 확률 분포에 따른 벡터 값을 가저온다.
	Vector3 randomVector(const Vector3& scale);

	//두개의 벡터로 만들어진 큐브 공간내의 랜덤한 점을 가져온다.
	Vector3 randomVector(const Vector3& min, const Vector3& max);

	//Y를 제외한 -scale ~ +scale 까지  X,Y 좌표의 이산 확률 분포에 따른 벡터 값을 가저온다.
	Vector3 randomXZVector(real scale);

	//랜덤한 쿼터니언을 가저온다
	//Quaternion randomQuaternion();

private:
	//내부 변수
	int p1, p2;
	unsigned buffer[17];
};
