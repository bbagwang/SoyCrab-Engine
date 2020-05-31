#include "SoyCrabEngine.h"

//���� �ν��Ͻ� �������� ���漱��
extern Application* getApplication();
//���� ��
Application* app;

//������ ����
void createWindow(const char* title)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,320);
    glutInitWindowPosition(0,0);
    glutCreateWindow(title);
}

//������Ʈ
void update()
{
    //�ð� ������Ʈ
    TimingData::get().update();

    app->update();
}

//����
void display()
{
    //3D �� ������
    app->display();

    //���� �������� ���� ������ �ѱ�
    glFlush();
    glutSwapBuffers();
}

//���콺 �Է�
void mouse(int button, int state, int x, int y)
{
    app->mouse(button, state, x, y);
}

//������ ������ ����
void reshape(int width, int height)
{
    app->resize(width, height);
}

//Ű �Է�
void keyboard(unsigned char key, int x, int y)
{
    app->key(key);
}

//���콺 �巹�� �Է�
void motion(int x, int y)
{
    app->mouseDrag(x, y);
}

int main(int argc, char** argv)
{
    //GLUT ����
    glutInit(&argc, argv);
    //�ð� �ʱ�ȭ
    TimingData::init();

    //���� �ν��Ͻ� ����
    app = getApplication();
    //������ ����
    createWindow(app->getTitle());

    //GLUT ���� �ʱ�ȭ���ε�
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    //����� ����
    app->initGraphics();
    //GLUT ����
    glutMainLoop();

    //���� ������ �Ѿ���� �ʱ�ȭ �� �޸� ���� ����
    app->deinit();
    delete app;
    TimingData::deinit();
}
