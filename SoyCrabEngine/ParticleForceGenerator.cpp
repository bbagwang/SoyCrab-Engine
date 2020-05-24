#include "ParticleForceGenerator.h"

#pragma region GravityGenerator
void ParticleGravityGenerator::UpdateForce(Particle* Particle, real Duration)
{
	if (!Particle->HasFiniteMass())
		return;
	//ÀÔÀÚÀÇ Áú·® Å©±â¿¡ µû¸¥ ÈûÀ» Àû¿ë½ÃÅ´
	Particle->AddForce(Gravity * Particle->GetMass());
}
#pragma endregion

#pragma region DragGenerator
void ParticleDragGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetVelocity(&Force);
	//ÅäÅ» µå·¹±× °è¼ö¸¦ °è»êÇÑ´Ù.
	real DragCoefficient = Force.Magnitude();
	DragCoefficient = k1 * DragCoefficient + k2 * real_pow(DragCoefficient, 2);

	//ÃÖÁ¾ ÈûÀ» °è»êÇÏ¿© Àû¿ëÇÑ´Ù.
	Force.Normalize();
	Force *= -DragCoefficient;
	Particle->AddForce(Force);
}
#pragma endregion

#pragma region SpringGenerator
void ParticleSpringGenerator::UpdateForce(Particle* Particle, real Duration)
{
	if (!Particle)
		return;
	
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= Other->GetPosition();
	
	//ï¿½ï¿½Å©ï¿½ï¿½ ï¿½ï¿½Ä¢(Hook's Law)ï¿½ï¿½ ï¿½Ç°ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ Å©ï¿½ï¿½ ï¿½ï¿½ï¿½
	real Magnitude = Force.Magnitude();
	Magnitude = real_abs(Magnitude - RestLength);
	Magnitude *= SpringConstant;
	
	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ï¿½Ú¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ñ´ï¿½.
	Force.Normalize();
	Force *= -Magnitude;
	Particle->AddForce(Force);
}
#pragma endregion
#pragma region AnchoredSpringGenerator
void ParticleAnchoredSpringGenerator::UpdateForce(Particle* Particle, real Duration)
{
	Vector3 Force;
	Particle->GetPosition(&Force);
	Force -= *Anchor;

	//ï¿½ï¿½Å©ï¿½ï¿½ ï¿½ï¿½Ä¢(Hook's Law)ï¿½ï¿½ ï¿½Ç°ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ Å©ï¿½ï¿½ ï¿½ï¿½ï¿½
	real Magnitude = Force.Magnitude();
	Magnitude = (RestLength - Magnitude) * SpringConstant;

	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ï¿½ï¿½
	Force.Normalize();
	Force *= Magnitude;
	Particle->AddForce(Force);
}
#pragma endregion
#pragma endregion