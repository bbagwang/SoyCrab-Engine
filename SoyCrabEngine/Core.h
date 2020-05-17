#pragma once
#include "Precision.h"

class Vector3
{
public:
	real X;
	real Y;
	real Z;

private:
	//4 Word(8Byte) 맞춤을 위해 덧붙임 
	real pad = 0;

public:
	//생성, 소멸자
	Vector3() : X(0), Y(0), Z(0) {}
	Vector3(const real X, const real Y, const real Z) :X(X), Y(Y), Z(Z) {}

	//상수
	const static Vector3 GRAVITY;

	//연산자
	Vector3 operator+(const Vector3& V) const { return Vector3(X + V.X, Y + V.Y, Z + V.Z); }
	void operator+=(const Vector3& V) { X += V.X, Y += V.Y, Z += V.Z; }
	Vector3 operator-(const Vector3& V) const { return Vector3(X - V.X, Y - V.Y, Z - V.Z); }
	void operator-=(const Vector3& V) { X -= V.X, Y -= V.Y, Z -= V.Z; }
	Vector3 operator*(const real Value) const { return Vector3(X * Value, Y * Value, Z * Value); }
	void operator *= (const real Value) { X *= Value, Y *= Value, Z *= Value; }
	//이 벡터와 주어진 벡터의 내적을 계산하여 반환한다. (ScalarProduct)
	real operator*(const Vector3& V) const { return X * V.X + Y * V.Y + Z * V.Z; }

	//모든 요소들을 0으로 초기화한다.
	void Clear()
	{
		X = Y = Z = 0;
	}

	//부호 반전
	void Invert()
	{
		X = -X;
		Y = -Y;
		Z = -Z;
	}

	//벡터 크기 계산
	real Magnitude() const { return real_sqrt(X * X + Y * Y + Z * Z); }

	//벡터 크기의 제곱 계산
	real SquareMagnitude() const { return X * X + Y * Y + Z * Z; }

	//영벡터가 아닌 벡터를 단위 벡터로 변환
	void Normalize()
	{
		real l = Magnitude();
		if (l > 0)
			(*this) *= (((real)1) / l);
	}

	//주어진 벡터를 주어진 스칼라만큼 더한 다음 이 벡터에 더해준다.
	void AddScaledVector(const Vector3& Vector, real Scale)
	{
		X += Vector.X * Scale;
		Y += Vector.Y * Scale;
		Z += Vector.Z * Scale;
	}

	//성분 곱셈 : 이 벡터와 주어진 벡터와의 성분 간 곱셈을 하여 그 결과를 반환한다.
	Vector3 ComponentProduct(const Vector3& Vector) const
	{
		return Vector3(X * Vector.X, Y * Vector.Y, Z * Vector.Z);
	}

	//주어진 벡터의 성분을 이 벡터의 각 성분에 곱해준다.
	void ComponentProductUpdate(const Vector3& Vector)
	{
		X *= Vector.X;
		Y *= Vector.Y;
		Z *= Vector.Z; 
	}

	//이 벡터와 주어진 벡터의 내적을 계산하여 반환한다.
	real ScalarProduct(const Vector3& Vector) const
	{
		return X * Vector.X + Y * Vector.Y + Z * Vector.Z;
	}

	//이 벡터와 주어진 벡터의 벡터곱(Vector Product)을 계산하여 반환한다.
	//엄격한 의미로 외적은 Cross Product가 아니고 Cross Product의 크기를 의미한다. |a X b|
	Vector3 VectorProduct(const Vector3& Vector) const
	{
		return Vector3(
			Y * Vector.Z - Z * Vector.Y,
			Z * Vector.X - X * Vector.Z,
			X * Vector.Y - Y * Vector.X);
	}

	//이 벡터와 주어진 벡터의 벡터곱을 계산하여 반환한다.
	Vector3 operator%(const Vector3& Vector) const
	{
		return Vector3(
			Y * Vector.Z - Z * Vector.Y,
			Z * Vector.X - X * Vector.Z,
			X * Vector.Y - Y * Vector.X);
	}

	//이 벡터와 주어진 벡터의 벡터곱을 계산한 다음 이 벡터에 업데이트한다.
	void operator%=(const Vector3& Vector)
	{
		*this = VectorProduct(Vector);
	}

	//정규 직교 기저 구하기 (오른손 좌표계 기준)
	//왼손 좌표계로 할려면 크로스 프로덕트를 계산할 때 순서 반대로 하면 된다.
	void MakeOrthonormalBasis(Vector3* a, Vector3* b, Vector3* c)
	{
		//시작 벡터 a를 정규화한다.
		a->Normalize();

		//크로스 프로덕트를 계산하여 c를 찾는다. c = a X b
		(*c) = (*a) % (*b);

		//c의 길이가 0이면 실패: 즉, a와 b가 평행한 것이다.
		if (c->SquareMagnitude() == 0.0) return;	//아니면 에러를 발생시킴

		//c를 정규화 한다.
		c->Normalize();

		//a와 b가 서로 수직이 되도록 맞춰준다.
		//방법은 a와 c의 크로스 프로덕트를 구해 b를 다시 계산하는 것이다.(b = c X a)
		//곱하는 순서에 주의해야한다.
		//벡터 a와 c가 길이가 1이고 서로 수직이므로 벡터 b의 길이는 1이다.
		(*b) = (*c) % (*a);
	}

};