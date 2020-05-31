#pragma once
#include "SoyCrabEngine.h"

class Application
{
protected:
    //데모 창 높이
    int height;
    //데모 창 넓이
    int width;

public:
    //창 타이틀 이름
    virtual const char* getTitle();

    //그래픽 라이브러리 초기화
    virtual void initGraphics();

    //FOV 업데이트
    virtual void setView();

    //인스턴스 파괴
    virtual void deinit();

    //렌더링
    virtual void display();

    //업데이트
    virtual void update();

    //키입력
    virtual void key(unsigned char key);

    //윈도우 사이즈 변경
    virtual void resize(int width, int height);

    //마우스 입력
    virtual void mouse(int button, int state, int x, int y);

    //마우스 드레그 입력
    virtual void mouseDrag(int x, int y);

    //텍스트 렌더
    void renderText(float x, float y, const char *text, void* font=NULL);
};

//질량 집합을 이용하는 데모를 위해 기능을 추가한 어플리케이션
class MassAggregateApplication : public Application
{
protected:
    ParticleWorld World;
    Particle* ParticleArray;
    GroundContacts GroundContactGenerator;

public:
	MassAggregateApplication(unsigned int particleCount);
	virtual ~MassAggregateApplication();

	//그래픽 초기화
	virtual void initGraphics();

	//파티클 렌더링
	virtual void display();

	//파티클 위치 업데이트
	virtual void update();
};