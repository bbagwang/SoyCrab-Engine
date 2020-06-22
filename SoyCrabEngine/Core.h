#pragma once
#include "Precision.h"
class Vector3;
class Quaternion;
class Matrix3;
class Matrix4;

#pragma region Physics-Sleep
extern real sleepEpsilon = ((real)0.3);

void setSleepEpsilon(real value) { sleepEpsilon = value; }

real getSleepEpsilon() { return sleepEpsilon; }
#pragma endregion

#pragma region Vector3
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
	const static Vector3 UP;
	const static Vector3 RIGHT;
	const static Vector3 X_AXIS;
	const static Vector3 Y_AXIS;
	const static Vector3 Z_AXIS;

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
#pragma endregion

#pragma region Quaternion
class Quaternion
{
public:
	union {
		struct { real r, i, j, k; };

		real data[4];
	};

	Quaternion() : r(1), i(0), j(0), k(0) {}

	Quaternion(const real r, const real i, const real j, const real k)
		: r(r), i(i), j(j), k(k) {}

	void normalise()
	{
		real d = r * r + i * i + j * j + k * k;

		if (d < real_epsilon) {
			r = 1;
			return;
		}

		d = ((real)1.0) / real_sqrt(d);
		r *= d;
		i *= d;
		j *= d;
		k *= d;
	}

	void operator *=(const Quaternion& multiplier)
	{
		Quaternion q = *this;
		r = q.r * multiplier.r - q.i * multiplier.i -
			q.j * multiplier.j - q.k * multiplier.k;
		i = q.r * multiplier.i + q.i * multiplier.r +
			q.j * multiplier.k - q.k * multiplier.j;
		j = q.r * multiplier.j + q.j * multiplier.r +
			q.k * multiplier.i - q.i * multiplier.k;
		k = q.r * multiplier.k + q.k * multiplier.r +
			q.i * multiplier.j - q.j * multiplier.i;
	}

	void addScaledVector(const Vector3& vector, real scale)
	{
		Quaternion q(0,
			vector.X * scale,
			vector.Y * scale,
			vector.Z * scale);
		q *= *this;
		r += q.r * ((real)0.5);
		i += q.i * ((real)0.5);
		j += q.j * ((real)0.5);
		k += q.k * ((real)0.5);
	}

	void rotateByVector(const Vector3& vector)
	{
		Quaternion q(0, vector.X, vector.Y, vector.Z);
		(*this) *= q;
	}
};
#pragma endregion

#pragma region Matrix-4x4
class Matrix4
{
public:
    real data[12];

    Matrix4()
    {
        data[1] = data[2] = data[3] = data[4] = data[6] =
            data[7] = data[8] = data[9] = data[11] = 0;
        data[0] = data[5] = data[10] = 1;
    }

    void setDiagonal(real a, real b, real c)
    {
        data[0] = a;
        data[5] = b;
        data[10] = c;
    }

    Matrix4 operator*(const Matrix4& o) const
    {
        Matrix4 result;
        result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
        result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
        result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);

        result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
        result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
        result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);

        result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
        result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
        result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);

        result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
        result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
        result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];

        return result;
    }

    Vector3 operator*(const Vector3& vector) const
    {
        return Vector3(
            vector.X * data[0] +
            vector.Y * data[1] +
            vector.Z * data[2] + data[3],

			vector.X* data[4] +
			vector.Y * data[5] +
			vector.Z * data[6] + data[7],

			vector.X* data[8] +
			vector.Y * data[9] +
			vector.Z * data[10] + data[11]
        );
    }

    Vector3 transform(const Vector3& vector) const
    {
        return (*this) * vector;
    }

    real getDeterminant() const;

    void setInverse(const Matrix4& m);

    Matrix4 inverse() const
    {
        Matrix4 result;
        result.setInverse(*this);
        return result;
    }

    void invert()
    {
        setInverse(*this);
    }

    Vector3 transformDirection(const Vector3& vector) const
    {
        return Vector3(
			vector.X * data[0] +
			vector.Y * data[1] +
			vector.Z * data[2],

			vector.X* data[4] +
			vector.Y * data[5] +
			vector.Z * data[6],

			vector.X* data[8] +
			vector.Y * data[9] +
			vector.Z * data[10]
        );
    }

    Vector3 transformInverseDirection(const Vector3& vector) const
    {
        return Vector3(
			vector.X * data[0] +
			vector.Y * data[4] +
			vector.Z * data[8],

			vector.X * data[1] +
			vector.Y * data[5] +
			vector.Z * data[9],

			vector.X * data[2] +
			vector.Y * data[6] +
			vector.Z * data[10]
        );
    }

    Vector3 transformInverse(const Vector3& vector) const
    {
        Vector3 tmp = vector;
		tmp.X -= data[3];
		tmp.Y -= data[7];
		tmp.Z -= data[11];
        return Vector3(
			tmp.X * data[0] +
			tmp.Y * data[4] +
			tmp.Z * data[8],

			tmp.X * data[1] +
			tmp.Y * data[5] +
			tmp.Z * data[9],

			tmp.X * data[2] +
			tmp.Y * data[6] +
			tmp.Z * data[10]
        );
    }

    Vector3 getAxisVector(int i) const
    {
        return Vector3(data[i], data[i + 4], data[i + 8]);
    }

    void setOrientationAndPos(const Quaternion& q, const Vector3& pos)
    {
        data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
        data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
        data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
        data[3] = pos.X;

        data[4] = 2 * q.i * q.j - 2 * q.k * q.r;
        data[5] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
        data[6] = 2 * q.j * q.k + 2 * q.i * q.r;
        data[7] = pos.Y;

        data[8] = 2 * q.i * q.k + 2 * q.j * q.r;
        data[9] = 2 * q.j * q.k - 2 * q.i * q.r;
        data[10] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
        data[11] = pos.Z;
    }

    void fillGLArray(float array[16]) const
    {
        array[0] = (float)data[0];
        array[1] = (float)data[4];
        array[2] = (float)data[8];
        array[3] = (float)0;

        array[4] = (float)data[1];
        array[5] = (float)data[5];
        array[6] = (float)data[9];
        array[7] = (float)0;

        array[8] = (float)data[2];
        array[9] = (float)data[6];
        array[10] = (float)data[10];
        array[11] = (float)0;

        array[12] = (float)data[3];
        array[13] = (float)data[7];
        array[14] = (float)data[11];
        array[15] = (float)1;
    }
};
#pragma endregion

#pragma region Matrix-3x3
class Matrix3
{
public:
    real data[9];

    Matrix3()
    {
        data[0] = data[1] = data[2] = data[3] = data[4] = data[5] =
            data[6] = data[7] = data[8] = 0;
    }

    Matrix3(const Vector3& compOne, const Vector3& compTwo,
        const Vector3& compThree)
    {
        setComponents(compOne, compTwo, compThree);
    }

    Matrix3(real c0, real c1, real c2, real c3, real c4, real c5,
        real c6, real c7, real c8)
    {
        data[0] = c0; data[1] = c1; data[2] = c2;
        data[3] = c3; data[4] = c4; data[5] = c5;
        data[6] = c6; data[7] = c7; data[8] = c8;
    }

    void setDiagonal(real a, real b, real c)
    {
        setInertiaTensorCoeffs(a, b, c);
    }

    void setInertiaTensorCoeffs(real ix, real iy, real iz,
        real ixy = 0, real ixz = 0, real iyz = 0)
    {
        data[0] = ix;
        data[1] = data[3] = -ixy;
        data[2] = data[6] = -ixz;
        data[4] = iy;
        data[5] = data[7] = -iyz;
        data[8] = iz;
    }

    void setBlockInertiaTensor(const Vector3& halfSizes, real mass)
    {
        Vector3 squares = halfSizes.ComponentProduct(halfSizes);
        setInertiaTensorCoeffs(0.3f * mass * (squares.Y + squares.Z),
            0.3f * mass * (squares.X + squares.Z),
            0.3f * mass * (squares.X + squares.Y));
    }

    void setSkewSymmetric(const Vector3 vector)
    {
        data[0] = data[4] = data[8] = 0;
        data[1] = -vector.Z;
        data[2] = vector.Y;
        data[3] = vector.Z;
        data[5] = -vector.X;
        data[6] = -vector.Y;
        data[7] = vector.X;
    }

    void setComponents(const Vector3& compOne, const Vector3& compTwo,
        const Vector3& compThree)
    {
        data[0] = compOne.X;
        data[1] = compTwo.X;
        data[2] = compThree.X;
        data[3] = compOne.Y;
        data[4] = compTwo.Y;
        data[5] = compThree.Y;
        data[6] = compOne.Z;
        data[7] = compTwo.Z;
        data[8] = compThree.Z;

    }

    Vector3 operator*(const Vector3& vector) const
    {
        return Vector3(
			vector.X * data[0] + vector.Y * data[1] + vector.Z * data[2],
			vector.X * data[3] + vector.Y * data[4] + vector.Z * data[5],
			vector.X * data[6] + vector.Y * data[7] + vector.Z * data[8]
        );
    }

    Vector3 transform(const Vector3& vector) const
    {
        return (*this) * vector;
    }

    Vector3 transformTranspose(const Vector3& vector) const
    {
        return Vector3(
			vector.X * data[0] + vector.Y * data[3] + vector.Z * data[6],
			vector.X * data[1] + vector.Y * data[4] + vector.Z * data[7],
			vector.X * data[2] + vector.Y * data[5] + vector.Z * data[8]
        );
    }

    Vector3 getRowVector(int i) const
    {
        return Vector3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }

    Vector3 getAxisVector(int i) const
    {
        return Vector3(data[i], data[i + 3], data[i + 6]);
    }

    void setInverse(const Matrix3& m)
    {
        real t4 = m.data[0] * m.data[4];
        real t6 = m.data[0] * m.data[5];
        real t8 = m.data[1] * m.data[3];
        real t10 = m.data[2] * m.data[3];
        real t12 = m.data[1] * m.data[6];
        real t14 = m.data[2] * m.data[6];

        real t16 = (t4 * m.data[8] - t6 * m.data[7] - t8 * m.data[8] +
            t10 * m.data[7] + t12 * m.data[5] - t14 * m.data[4]);

        if (t16 == (real)0.0f) return;
        real t17 = 1 / t16;

        data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * t17;
        data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * t17;
        data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * t17;
        data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * t17;
        data[4] = (m.data[0] * m.data[8] - t14) * t17;
        data[5] = -(t6 - t10) * t17;
        data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * t17;
        data[7] = -(m.data[0] * m.data[7] - t12) * t17;
        data[8] = (t4 - t8) * t17;
    }

    Matrix3 inverse() const
    {
        Matrix3 result;
        result.setInverse(*this);
        return result;
    }

    void invert()
    {
        setInverse(*this);
    }

    void setTranspose(const Matrix3& m)
    {
        data[0] = m.data[0];
        data[1] = m.data[3];
        data[2] = m.data[6];
        data[3] = m.data[1];
        data[4] = m.data[4];
        data[5] = m.data[7];
        data[6] = m.data[2];
        data[7] = m.data[5];
        data[8] = m.data[8];
    }

    Matrix3 transpose() const
    {
        Matrix3 result;
        result.setTranspose(*this);
        return result;
    }

    Matrix3 operator*(const Matrix3& o) const
    {
        return Matrix3(
            data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
            data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
            data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

            data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
            data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
            data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

            data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
            data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
            data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
        );
    }

    void operator*=(const Matrix3& o)
    {
        real t1;
        real t2;
        real t3;

        t1 = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
        t2 = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
        t3 = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
        data[0] = t1;
        data[1] = t2;
        data[2] = t3;

        t1 = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
        t2 = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
        t3 = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];
        data[3] = t1;
        data[4] = t2;
        data[5] = t3;

        t1 = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
        t2 = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
        t3 = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
        data[6] = t1;
        data[7] = t2;
        data[8] = t3;
    }

    void operator*=(const real scalar)
    {
        data[0] *= scalar; data[1] *= scalar; data[2] *= scalar;
        data[3] *= scalar; data[4] *= scalar; data[5] *= scalar;
        data[6] *= scalar; data[7] *= scalar; data[8] *= scalar;
    }

    void operator+=(const Matrix3& o)
    {
        data[0] += o.data[0]; data[1] += o.data[1]; data[2] += o.data[2];
        data[3] += o.data[3]; data[4] += o.data[4]; data[5] += o.data[5];
        data[6] += o.data[6]; data[7] += o.data[7]; data[8] += o.data[8];
    }

    void setOrientation(const Quaternion& q)
    {
        data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
        data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
        data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
        data[3] = 2 * q.i * q.j - 2 * q.k * q.r;
        data[4] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
        data[5] = 2 * q.j * q.k + 2 * q.i * q.r;
        data[6] = 2 * q.i * q.k + 2 * q.j * q.r;
        data[7] = 2 * q.j * q.k - 2 * q.i * q.r;
        data[8] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
    }

    static Matrix3 linearInterpolate(const Matrix3& a, const Matrix3& b, real prop);
};
#pragma endregion