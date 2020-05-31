#include "SoyCrabEngine.h"

//데모 인스턴스 가저오기 전방선언
extern Application* getApplication();
//데모 앱
Application* app;

//윈도우 생성
void createWindow(const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,320);
    glutInitWindowPosition(0,0);
    glutCreateWindow(title);
}

//업데이트
void update()
{
    //시간 업데이트
    TimingData::get().update();

    app->update();
}

//랜더
void display()
{
    //3D 씬 보여줌
    app->display();

    //버퍼 스왑으로 다음 프레임 넘김
    glFlush();
    glutSwapBuffers();
}

//마우스 입력
void mouse(int button, int state, int x, int y)
{
    app->mouse(button, state, x, y);
}

//윈도우 사이즈 변경
void reshape(int width, int height)
{
    app->resize(width, height);
}

//키 입력
void keyboard(unsigned char key, int x, int y)
{
    app->key(key);
}

//마우스 드레그 입력
void motion(int x, int y)
{
    app->mouseDrag(x, y);
}

int main(int argc, char** argv)
{
    //GLUT 시작
    glutInit(&argc, argv);
    //시간 초기화
    TimingData::init();

    //데모 인스턴스 생성
    app = getApplication();
    //윈도우 생성
    createWindow(app->getTitle());

    //GLUT 관련 초기화바인딩
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    //데모앱 시작
    app->initGraphics();
    //GLUT 루프
    glutMainLoop();

    //루프 끝나서 넘어오면 초기화 및 메모리 해제 진행
    app->deinit();
    delete app;
    TimingData::deinit();
}
