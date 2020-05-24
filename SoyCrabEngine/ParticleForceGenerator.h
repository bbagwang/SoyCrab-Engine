#pragma once
#include "SoyCrabEngine.h"

class ParticleForceGenerator
{
public:
	//ÁÖ¾îÁø ÀÔÀÚ¿¡ Àû¿ëÇÏ´Â ÈûÀ» °è»êÇÏ°í ¾÷µ¥ÀÌÆ® ÇÏ±â À§ÇØ ÀÌ ¸Ş¼­µå¸¦ ÀçÁ¤ÀÇ
	virtual void UpdateForce(Particle* Particle, real Duration) = 0;
};

#pragma region GravityGenerator
//Áß·ÂÀ» Àû¿ëÇÏ´Â Èû ¹ß»ı±â
//ÀÎ½ºÅÏ½º ÇÏ³ª·Î ¿©·¯ °³ÀÇ ÀÔÀÚ¿¡ »ç¿ë °¡´É.

//°£´ÜÇÏ´Ù´Â ÀåÁ¡ÀÌ ÀÖÁö¸¸, ´ïÇÎ°ú Áß·Â °¡¼Óµµ´Â ±âÁ¸ ÄÚµå »ç¿ë.
//ÀÌÀ¯´Â ¸Å ÇÁ·¹ÀÓ¸¶´Ù ÀÌ°É ÀÏÀÏÈ÷ °è»êÇØ¾ßÇÒ ÇÊ¿ä°¡ ¾ø±â ¶§¹®.
//ÀÌ¹Ì ´äÀ» ¾Ë°íÀÖ´Â °è»êÀ» ÇÏ´Â °Í°ú ´Ù¸§ÀÌ ¾ø´Ù.
class ParticleGravityGenerator : public ParticleForceGenerator
{
private:
	//Áß·Â °¡¼Óµµ
	Vector3 Gravity;
public:
	//ÁÖ¾îÁø °¡¼Óµµ·Î Èû ¹ß»ı±â ÀÎ½ºÅÏ½º »ı¼º
	ParticleGravityGenerator(const Vector3& Gravity) : Gravity(Gravity) {}
	//ÁÖ¾îÁø ÀÔÀÚ¿¡ Áß·Â Àû¿ë
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region DragGenerator
//µå·¹±× ÈûÀ½ Àû¿ëÇÏ´Â Èû ¹ß»ı±â
//ÀÎ½ºÅÏ½º ÇÏ³ª·Î ¿©·¯ °³ÀÇ ÀÔÀÚ¿¡ »ç¿ë °¡´É.
class ParticleDragGenerator : public ParticleForceGenerator
{
	//Drag Force ½Ä
	//µå·¹±× Èû = -¼Óµµ(µå·¹±× »ó¼ö1(k1)*(Á¤±Ô ¼ÓµµÀÇ Å©±â)+µå·¹±× »ó¼ö2(k2)*(Á¤±Ô ¼ÓµµÀÇ Å©±â)^2)
private:
	//Drag Coefficient (µå·¹±× °è¼ö)
	//¼Óµµ¿¡ °öÇØÁö´Â µå·¡±× ºñ·Ê»ó¼ö ÀúÀå
	real k1;
	//¼ÓµµÀÇ Á¦°ö¿¡ °öÇØÁö´Â µå·¹±× ºñ·Ê»ó¼ö ÀúÀå
	//k2°ªÀÌ Å¬¼ö·Ï ¼Óµµ°¡ »¡¶óÁü¿¡ µû¶ó ±Ş°İÈ÷ µå·¹±×°¡ °­ÇØÁü.
	//¼Óµµ°¡ µÎ¹è »¡¶óÁö¸é °ø±âÀúÇ×Àº 4¹è°¡ µÇ´Â °Í°ú °°Àº ¸Æ¶ô.
	real k2;
public:
	//ÁÖ¾îÁø ºñ·Ê»ó¼ö·Î Èû ¹ß»ı±â °³Ã¼ »ı¼º
	ParticleDragGenerator(real k1, real k2) : k1(k1), k2(k2) {}
	//ÁÖ¾îÁø ÀÔÀÚ¿¡ µå·¹±× Èû Àû¿ë
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region SpringGenerator
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ ï¿½ß»ï¿½ï¿½ï¿½
class ParticleSpringGenerator : public ParticleForceGenerator
{
private:
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½İ´ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	Particle* Other;
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
	real SpringConstant;
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	real RestLength;
public:
	//ï¿½Ö¾ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Úµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Î¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	ParticleSpringGenerator(Particle* Other, real SpringConstant, real RestLength)
		:Other(Other), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//ï¿½Ö¾ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ú¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ñ´ï¿½.
	virtual void UpdateForce(Particle* Particle, real Duration);
};
#pragma endregion

#pragma region ParticleAnchoredSpringGenerator
class ParticleAnchoredSpringGenerator : public ParticleForceGenerator
{
private:
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡
	Vector3* Anchor;
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
	real SpringConstant;
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	real RestLength;
public:
	//ï¿½Ö¾ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ú¸ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Î¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Î½ï¿½ï¿½Ï½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	ParticleAnchoredSpringGenerator(Vector3* Anchor, real SpringConstant, real RestLength) :
		Anchor(Anchor), SpringConstant(SpringConstant), RestLength(RestLength) {}
	//ï¿½Ö¾ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ú¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ñ´ï¿½.
	virtual void UpdateForce(Particle* Particle, real Duration);
	void SetAnchor(Vector3* NewAnchorPosition) { Anchor = NewAnchorPosition; }
	Vector3 GetAnchor() const { return *Anchor; }
};
#pragma endregion
#pragma endregion