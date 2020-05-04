#pragma once

struct TimingData
{
    //������ �ѹ�
    unsigned frameNumber;

    //������ ������ ���� �ð�
    unsigned lastFrameTimestamp;

    //������ ������ ���� �ð�
    unsigned lastFrameDuration;

    //������ ������ Ŭ�� ������
    unsigned long lastFrameClockstamp;

    //������ ������ Ŭ�� ƽ
    unsigned long lastFrameClockTicks;

    //������ ����°� üũ
    bool isPaused;

    //���� �ֱ� ����� ����ġ�� ����� ������ �ð�
    double averageFrameDuration;

    //�ʴ� ������ ��
    float fps;

    //���� Ÿ�̹� ������ ����
    static TimingData& get();

    //������Ʈ
    static void update();

    //�ʱ�ȭ
    static void init();

    //����
    static void deinit();

    //�и������� ���� ���� �ð� ������
    static unsigned getTime();

    //���μ��� ������ ���ĺ��� Ŭ�� ƽ ������
    static unsigned long getClock();


private:
    //�̱������� ���� ���� ������ PRIVATE
    TimingData() {}
    TimingData(const TimingData &) {}
    TimingData& operator=(const TimingData &);
};