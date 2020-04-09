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
	real pad;

public:
	//생성, 소멸자
	Vector3() : X(0), Y(0), Z(0) {}
	Vector3(const real X, const real Y, const real Z) :X(X), Y(Y), Z(Z) {}
	
	//연산자
	Vector3 operator+(const Vector3& V) const { return Vector3(X + V.X, Y + V.Y, Z + V.Z); }
	void operator+=(const Vector3& V) { X += V.X, Y += V.Y, Z += V.Z; }
	Vector3 operator-(const Vector3& V) const { return Vector3(X - V.X, Y - V.Y, Z - V.Z); }
	void operator-=(const Vector3& V) { X -= V.X, Y -= V.Y, Z -= V.Z; }
	Vector3 operator*(const real Value) const { return Vector3(X * Value, Y * Value, Z * Value); }
	void operator *= (const real Value) { X *= Value, Y *= Value, Z *= Value; }
	
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
	void ComponentProjectUpdate(const Vector3& Vector)
	{
		X *= Vector.X;
		Y *= Vector.Y;
		Z *= Vector.Z;
	}

}