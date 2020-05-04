#include "SoyCrabEngine.h"

//int main()
//{
//	Particle DampingNormal;
//	Particle DampingDragFix;
//
//	DampingNormal.SetMass(2.f);
//	DampingDragFix.SetMass(2.f);
//
//	DampingNormal.SetVelocity(0.0f, 0.0f, 100.f);
//	DampingDragFix.SetVelocity(0.0f, 0.0f, 100.f);
//
//	DampingNormal.SetAcceleration(0.f, -1.f, 0.f);
//	DampingDragFix.SetAcceleration(0.f, -1.f, 0.f);
//
//	DampingNormal.SetDamping(0.99f);
//	DampingDragFix.SetDamping(0.99f);
//	
//	real OneTime = 1.0f;
//	int i = 0;
//	while (OneTime > 0)
//	{
//		real rangedrandom = (rand() % (100 - 33)) + (33 - 1);
//		real randomDeltaTime = rangedrandom / (real)1000;
//		OneTime -= randomDeltaTime;
//		i++;
//		std::cout << "Not Fixed Damping # " << i << std::endl;
//		DampingNormal.TEST_IntegrateNotFixed(randomDeltaTime);
//		std::cout << "Fixed Damping # " << i << std::endl;
//		DampingDragFix.Integrate(randomDeltaTime);
//	}
//}