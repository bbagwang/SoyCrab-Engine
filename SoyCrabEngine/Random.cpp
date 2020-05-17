#include <cstdlib>
#include <ctime>
#include "Random.h"

Random::Random()
{
    seed(0);
}

Random::Random(unsigned seed)
{
    Random::seed(seed);
}

void Random::seed(unsigned s)
{
    if (s == 0) {
        s = (unsigned)clock();
    }

    //���۸� ���� �ѹ��� ä���.
    for (unsigned i = 0; i < 17; i++)
    {
        //https://ko.wikipedia.org/wiki/%EC%84%A0%ED%98%95_%ED%95%A9%EB%8F%99_%EC%83%9D%EC%84%B1%EA%B8%B0
        //������ ���� �յ� ����������� ���� ����
        s = s * 2891336453 + 1;
        buffer[i] = s;
    }

    p1 = 0;  p2 = 10;
}

unsigned Random::rotl(unsigned n, unsigned r)
{
    return	(n << r) | (n >> (32 - r));
}

unsigned Random::rotr(unsigned n, unsigned r)
{
    return	(n >> r) | (n << (32 - r));
}

unsigned Random::randomBits()
{
    unsigned result;

    //���۸� ȸ����Ű�� �ٽ� ��´�.
    result = buffer[p1] = rotl(buffer[p2], 13) + rotl(buffer[p1], 9);

    //��Ʈ ������ ȸ��
    if (--p1 < 0) p1 = 16;
    if (--p2 < 0) p2 = 16;

    return result;
}

#ifdef SINGLE_PRECISION
real Random::randomReal()
{
    //���� �� ������
    unsigned bits = randomBits();

    //������ ���� ���� �缳��
    union {
        real value;
        unsigned word;
    } convert;

    // word�� bit �� �Ҵ�. IEEE �������� �۵�
    // ��ȣ �� ���� ��Ʈ ���� (���� ����� 1 ~ 2�� �ǵ���)
    // ��Ʈ�� ����� float�� �κ� ������ ������.
    convert.word = (bits >> 9) | 0x3f800000;

    return convert.value - 1.0f;
}
#else
real Random::randomReal()
{
    // ���� ������
    unsigned bits = randomBits();

    //������ ���� ���� �缳��
    union {
        real value;
        unsigned words[2];
    } convert;

	// word�� bit �� �Ҵ�. IEEE �������� �۵�
	// ��ȣ �� ���� ��Ʈ ���� (���� ����� 1 ~ 2�� �ǵ���)
	// ��Ʈ�� ����� float�� �κ� ������ ������.
    convert.words[0] = bits << 20; // ���� 16��Ʈ�� ä���
    convert.words[1] = (bits >> 12) | 0x3FF00000; // �Ʒ��� 20��Ʈ�� ä���.

    return convert.value - 1.0;
}
#endif

real Random::randomReal(real min, real max)
{
    return randomReal() * (max - min) + min;
}

real Random::randomReal(real scale)
{
    return randomReal() * scale;
}

unsigned Random::randomInt(unsigned max)
{
    return randomBits() % max;
}

real Random::randomBinomial(real scale)
{
    return (randomReal() - randomReal()) * scale;
}

//Quaternion Random::randomQuaternion()
//{
//    Quaternion q(
//        randomReal(),
//        randomReal(),
//        randomReal(),
//        randomReal()
//    );
//    q.normalise();
//    return q;
//}

Vector3 Random::randomVector(real scale)
{
    return Vector3(
        randomBinomial(scale),
        randomBinomial(scale),
        randomBinomial(scale)
    );
}

Vector3 Random::randomXZVector(real scale)
{
    return Vector3(
        randomBinomial(scale),
        0,
        randomBinomial(scale)
    );
}

Vector3 Random::randomVector(const Vector3& scale)
{
    return Vector3(
        randomBinomial(scale.X),
        randomBinomial(scale.Y),
        randomBinomial(scale.Z)
    );
}

Vector3 Random::randomVector(const Vector3& min, const Vector3& max)
{
    return Vector3(
        randomReal(min.X, max.X),
        randomReal(min.Y, max.Y),
        randomReal(min.Z, max.Z)
    );
}
