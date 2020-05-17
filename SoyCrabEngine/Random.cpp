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

    //버퍼를 랜덤 넘버로 채운다.
    for (unsigned i = 0; i < 17; i++)
    {
        //https://ko.wikipedia.org/wiki/%EC%84%A0%ED%98%95_%ED%95%A9%EB%8F%99_%EC%83%9D%EC%84%B1%EA%B8%B0
        //간단한 선형 합동 생성기법으로 난수 생성
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

    //버퍼를 회전시키고 다시 담는다.
    result = buffer[p1] = rotl(buffer[p2], 13) + rotl(buffer[p1], 9);

    //비트 포인터 회전
    if (--p1 < 0) p1 = 16;
    if (--p2 < 0) p2 = 16;

    return result;
}

#ifdef SINGLE_PRECISION
real Random::randomReal()
{
    //랜덤 수 가저옴
    unsigned bits = randomBits();

    //변경을 위한 구조 재설정
    union {
        real value;
        unsigned word;
    } convert;

    // word에 bit 값 할당. IEEE 고정시켜 작동
    // 부호 및 지수 비트 설정 (최종 결과가 1 ~ 2가 되도록)
    // 비트를 사용해 float의 부분 공간을 생성함.
    convert.word = (bits >> 9) | 0x3f800000;

    return convert.value - 1.0f;
}
#else
real Random::randomReal()
{
    // 난수 가저옴
    unsigned bits = randomBits();

    //변경을 위한 구조 재설정
    union {
        real value;
        unsigned words[2];
    } convert;

	// word에 bit 값 할당. IEEE 고정시켜 작동
	// 부호 및 지수 비트 설정 (최종 결과가 1 ~ 2가 되도록)
	// 비트를 사용해 float의 부분 공간을 생성함.
    convert.words[0] = bits << 20; // 위의 16비트를 채운다
    convert.words[1] = (bits >> 12) | 0x3FF00000; // 아래의 20비트를 채운다.

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
