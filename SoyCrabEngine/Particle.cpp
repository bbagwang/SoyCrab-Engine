#include "Particle.h"
#include <assert.h>
#include <iostream>

void Particle::SetMass(const real mass)
{
	assert(mass != 0);
	Particle::InverseMass = ((real)1.0) / mass;
}

real Particle::GetMass() const
{
	if (InverseMass == 0)
		return REAL_MAX;
	else
		return ((real)1.0 / InverseMass);
}

void Particle::SetInverseMass(const real inverseMass)
{
	InverseMass = inverseMass;
}

real Particle::GetInverseMass() const
{
	return InverseMass;
}

void Particle::SetDamping(const real damping)
{
	Damping = damping;
}

real Particle::GetDamping() const
{
	return Damping;
}

void Particle::SetPosition(const Vector3& position)
{
	Position = position;
}

void Particle::SetPosition(const real x, const real y, const real z)
{
	Position.X = x;
	Position.Y = y;
	Position.Z = z;
}

Vector3 Particle::GetPosition() const
{
	return Position;
}

void Particle::GetPosition(Vector3* position) const
{
	*position = Position;
}

void Particle::SetVelocity(const Vector3& velocity)
{
	Velocity = velocity;
}

void Particle::SetVelocity(const real x, const real y, const real z)
{
	Velocity.X = x;
	Velocity.Y = y;
	Velocity.Z = z;
}

Vector3 Particle::GetVelocity() const
{
	return Velocity;
}

void Particle::GetVelocity(Vector3* velocity) const
{
	*velocity = Velocity;
}

void Particle::SetAcceleration(const Vector3& acceleration)
{
	Acceleration = acceleration;
}

void Particle::SetAcceleration(const real x, const real y, const real z)
{
	Acceleration.X = x;
	Acceleration.Y = y;
	Acceleration.Z = z;
}

Vector3 Particle::GetAcceleration() const
{
	return Acceleration;
}

void Particle::GetAcceleration(Vector3* acceleration) const
{
	*acceleration = Acceleration;
}

void Particle::ClearAccumulator()
{
	ForceAccum.Clear();
}

void Particle::AddForce(const Vector3& force)
{
	ForceAccum += force;
}

void Particle::Integrate(real duration)
{
	//무한대 질량인 입자는 적분하지 않는다.
	if (InverseMass <= 0.f)
		return;

	assert(duration > 0.0f);

	//위치를 업데이트한다.
	Position.AddScaledVector(Velocity, duration);

	//힘으로부터 가속도를 계산한다.
	//힘이 여러 종류가 있다면, 이 벡터에 가속도를 더해준다.
	Vector3 resultingAcc = Acceleration;
	resultingAcc.AddScaledVector(ForceAccum, InverseMass);
	//AddForce(resultingAcc * GetMass());
	//가속도로부터 속도를 업데이트한다.
	Velocity.AddScaledVector(resultingAcc, duration);

	//드래그를 적용한다.
	Velocity *= real_pow(Damping, duration);

	std::cout << "X : " << Velocity.X << " Y : " << Velocity.Y << " Z : " << Velocity.Z << std::endl;

	//합력을 지운다.
	ClearAccumulator();
}

real Particle::CalculateKineticEnergy()
{
	//운동 에너지 공식  :  1/2 * m|v|^2
	//m : 질량 | |v| : 속도
	return (real)0.5 * GetMass() * (Velocity * Velocity);
}
