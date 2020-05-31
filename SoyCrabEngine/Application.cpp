#include "SoyCrabEngine.h"

void Application::initGraphics()
{
    glClearColor(0.9f, 0.95f, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    setView();
}

void Application::setView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width/(double)height, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void Application::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(639, 319);
    glEnd();
}

const char* Application::getTitle()
{
    return "SoyCrab Engine";
}

void Application::deinit()
{
}

void Application::update()
{
    glutPostRedisplay();
}

void Application::key(unsigned char key)
{
}


void Application::resize(int width, int height)
{
    // 0으로 나누지 못하도록 함
    if (height <= 0) height = 1;

    // 뷰 업데이트
    Application::width = width;
    Application::height = height;
    glViewport(0, 0, width, height);
    setView();
}

void Application::mouse(int button, int state, int x, int y)
{
}

void Application::mouseDrag(int x, int y)
{
}

void Application::renderText(float x, float y, const char *text, void *font)
{
    glDisable(GL_DEPTH_TEST);

    // 임시적으로 정사영 설정함
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

    // 모델 뷰 모드로 넘어감
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 폰트 체크
    if (font == NULL) {
        font = GLUT_BITMAP_HELVETICA_10;
    }

    // 텍스트 루프
    size_t len = strlen(text);

    glRasterPos2f(x, y);
    for (const char *letter = text; letter < text+len; letter++) {

        // 개행 문자 찾으면 폰트 크기만큼 내려서 그림
        if (*letter == '\n') {
            y -= 12.0f;
            glRasterPos2f(x, y);
        }
        glutBitmapCharacter(font, *letter);
    }

    // 이전 상태로 행렬들 빼고 리셋함
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

MassAggregateApplication::MassAggregateApplication(unsigned int ParticleCount) : World(ParticleCount * 10)
{
    ParticleArray = new Particle[ParticleCount];
    
    for (unsigned i = 0; i < ParticleCount; i++)
    {
        World.GetParticles().push_back(ParticleArray + i);
    }

    GroundContactGenerator.Init(&World.GetParticles());
    World.GetContactGenerators().push_back(&GroundContactGenerator);
}

MassAggregateApplication::~MassAggregateApplication()
{
    delete[] ParticleArray;
}

void MassAggregateApplication::initGraphics()
{
    //부모 클래스 호출
    Application::initGraphics();
}

void MassAggregateApplication::display()
{
    //뷰포트 초기화 후 카메라 방향 설정
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //방향 설정
    gluLookAt(
        0.0, 3.5, 8.0,
        0.0, 3.5, 0.0,
        0.0, 1.0, 0.0);

    glColor3f(0, 0, 0);

    std::vector<Particle*>& Particles = World.GetParticles();
    for (auto Iter : Particles)
    {
        Particle* particle = Iter;
        Vector3 Position = particle->GetPosition();
        glPushMatrix();
        glTranslatef(Position.X, Position.Y, Position.Z);
        glutSolidSphere(0.1, 20, 10);
        glPopMatrix();
    }
}

void MassAggregateApplication::update()
{
    //합력 초기화
    World.StartFrame();

    //델타 타임 가져옴
    float Duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (Duration <= 0.0f)
        return;

    //시뮬레이션 시작~
    World.RunPhysics(Duration);

    Application::update();
}