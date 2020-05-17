#pragma once

#include "Core.h"

class Random
{
public:
	//Bit-Wise ���� ȸ��
	unsigned rotl(unsigned n, unsigned r);

	//Bit-Wise ������ ȸ��
	unsigned rotr(unsigned n, unsigned r);

	Random();

	//�޾ƿ� �õ�� ���ο� ���� �� ����
	Random(unsigned seed);
	
	//�õ� ����
	void seed(unsigned seed);

	//���ο� ���� ��Ʈ ��Ʈ�� ���� (���� ���� ���)
	unsigned randomBits();

	//0 ~ 1 ���� ���� �Ǽ� ����
	real randomReal();
	
	//0 ~ Scale ���� ���� �Ǽ� ����
	real randomReal(real scale);

	//min ~ max ���� ���� �Ǽ� ����
	real randomReal(real min, real max);

	//0 ~ max ���� ���� ���� ����
	unsigned randomInt(unsigned max);

	//-scale ~ +scale ���� �̻� Ȯ�� ������ ���� �Ǽ� ���� �����´�.
	real randomBinomial(real scale);

	//-scale ~ +scale ���� ������ ��Ұ� �̻� Ȯ�� ������ ���� ���� ���� �����´�.
	Vector3 randomVector(real scale);

	//-scale ~ +scale ���� �޾ƿ� ���Ϳ� ���� ������ ��Ұ� �̻� Ȯ�� ������ ���� ���� ���� �����´�.
	Vector3 randomVector(const Vector3& scale);

	//�ΰ��� ���ͷ� ������� ť�� �������� ������ ���� �����´�.
	Vector3 randomVector(const Vector3& min, const Vector3& max);

	//Y�� ������ -scale ~ +scale ����  X,Y ��ǥ�� �̻� Ȯ�� ������ ���� ���� ���� �����´�.
	Vector3 randomXZVector(real scale);

	//������ ���ʹϾ��� �����´�
	//Quaternion randomQuaternion();

private:
	//���� ����
	int p1, p2;
	unsigned buffer[17];
};
