#include "Particle.h"
#include <assert.h>

Particle::Particle()
{

}

Particle::~Particle()
{

}

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
	//���Ѵ� ������ ���ڴ� �������� �ʴ´�.
	if (InverseMass <= 0.f)
		return;

	assert(duration > 0.0f);

	//��ġ�� ������Ʈ�Ѵ�.
	Position.AddScaledVector(Velocity, duration);

	//�����κ��� ���ӵ��� ����Ѵ�.
	//���� ���� ������ �ִٸ�, �� ���Ϳ� ���ӵ��� �����ش�.
	Vector3 resultingAcc = Acceleration;

	//���ӵ��κ��� �ӵ��� ������Ʈ�Ѵ�.
	Velocity.AddScaledVector(resultingAcc, duration);

	//�巡�׸� �����Ѵ�.
	Velocity *= real_pow(Damping, duration);

	//�� �׸��� �����.
	ClearAccumulator();
}
