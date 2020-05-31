#include "SoyCrabEngine.h"

BallisticDemo::BallisticDemo()
    : currentShotType(LASER)
{
    for (AmmoRound* shot = ammo; shot < ammo + ammoRounds; shot++)
    {
        shot->type = UNUSED;
    }
}

const char* BallisticDemo::getTitle()
{
    return "SoyCrab Engine - Ballistic";
}

void BallisticDemo::fire()
{
    AmmoRound* shot;
    for (shot = ammo; shot < ammo + ammoRounds; shot++)
    {
        if (shot->type == UNUSED) break;
    }

    if (shot >= ammo + ammoRounds) return;

    switch (currentShotType)
    {
    case PISTOL:
        shot->particle.SetMass(2.0f); // 2.0kg
        shot->particle.SetVelocity(0.0f, 0.0f, 35.0f); // 35m/s
        shot->particle.SetAcceleration(0.0f, -1.0f, 0.0f);
        shot->particle.SetDamping(0.99f);
        break;

    case ARTILLERY:
		shot->particle.SetMass(200.0f); // 200.0kg
		shot->particle.SetVelocity(0.0f, 30.0f, 40.0f); // 50m/s
		shot->particle.SetAcceleration(0.0f, -20.0f, 0.0f);
		shot->particle.SetDamping(0.99f);
        break;

    case FIREBALL:
        shot->particle.SetMass(1.0f); // 1.0kg
        shot->particle.SetVelocity(0.0f, 0.0f, 10.0f); // 5m/s
        shot->particle.SetAcceleration(0.0f, 0.6f, 0.0f); // 띄움
        shot->particle.SetDamping(0.9f);
        break;

    case LASER:
        shot->particle.SetMass(0.1f); // 0.1kg
        shot->particle.SetVelocity(0.0f, 0.0f, 100.0f); // 100m/s
        shot->particle.SetAcceleration(0.0f, 0.0f, 0.0f); // 중력 없음
        shot->particle.SetDamping(0.99f);
        break;
    }

    shot->particle.SetPosition(0.0f, 1.5f, 0.0f);
    shot->startTime = TimingData::get().lastFrameTimestamp;
    shot->type = currentShotType;

    //힘 클리어
    shot->particle.ClearAccumulator();
}

void BallisticDemo::update()
{
    //이전 프레임 시간 저장
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    for (AmmoRound* shot = ammo; shot < ammo + ammoRounds; shot++)
    {
        if (shot->type != UNUSED)
        {
            //물리 연산 진행
            shot->particle.Integrate(duration);

            //입자가 파괴될지 말지 결정
            if (shot->particle.GetPosition().Y < 0.0f ||
                shot->startTime + 5000 < TimingData::get().lastFrameTimestamp ||
                shot->particle.GetPosition().Z > 200.0f)
            {
                shot->type = UNUSED;
            }
        }
    }

    Application::update();
}

void BallisticDemo::display()
{
    //뷰포트 클리어
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //카메라 방향 설정
    gluLookAt(-25.0, 8.0, 5.0, 0.0, 5.0, 22.0, 0.0, 1.0, 0.0);

    // 발사 지점에 구(Sphere)를 그리고, 그림자를 바닥에 투영시킴
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glutSolidSphere(0.1f, 5, 5);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidSphere(0.1f, 5, 5);
    glPopMatrix();

    //스케일 라인 그림
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_LINES);
    for (unsigned i = 0; i < 200; i += 10)
    {
        glVertex3f(-5.0f, 0.0f, i);
        glVertex3f(5.0f, 0.0f, i);
    }
    glEnd();

    //각 탄약 렌더링
    for (AmmoRound* shot = ammo; shot < ammo + ammoRounds; shot++)
    {
        if (shot->type != UNUSED)
        {
            shot->render();
        }
    }

    //안내문 그리기
    glColor3f(0.0f, 0.0f, 0.0f);
    renderText(10.0f, 34.0f, "Left Mouse Click : Fire\n1-4: Change Ballistic");

    //현재 사격 타입 변경
    switch (currentShotType)
    {
    case PISTOL: renderText(10.0f, 10.0f, "Current Fire Type : PISTOL"); break;
    case ARTILLERY: renderText(10.0f, 10.0f, "Current Fire Type : ARTILLERY"); break;
    case FIREBALL: renderText(10.0f, 10.0f, "Current Fire Type : FIREBALL"); break;
    case LASER: renderText(10.0f, 10.0f, "Current Fire Type : LASER"); break;
    }
}

void BallisticDemo::mouse(int button, int state, int x, int y)
{
    // 현재 무기 발사
    if (state == GLUT_DOWN) fire();
}

void BallisticDemo::key(unsigned char key)
{
    switch (key)
    {
    case '1': currentShotType = PISTOL; break;
    case '2': currentShotType = ARTILLERY; break;
    case '3': currentShotType = FIREBALL; break;
    case '4': currentShotType = LASER; break;
    }
}

Application* getApplication()
{
    return new BallisticDemo();
}