#include "SoyCrabEngine.h"
#include <cstdlib>

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
