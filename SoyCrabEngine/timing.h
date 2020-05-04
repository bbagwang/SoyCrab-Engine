#pragma once

struct TimingData
{
    //프레임 넘버
    unsigned frameNumber;

    //마지막 프레임 끝난 시간
    unsigned lastFrameTimestamp;

    //마지막 프레임 지속 시간
    unsigned lastFrameDuration;

    //마지막 프레임 클락 스탬프
    unsigned long lastFrameClockstamp;

    //마지막 프레임 클락 틱
    unsigned long lastFrameClockTicks;

    //랜더링 멈췄는가 체크
    bool isPaused;

    //가장 최근 계산한 가중치가 적용된 프레임 시간
    double averageFrameDuration;

    //초당 프레임 수
    float fps;

    //전역 타이밍 데이터 게터
    static TimingData& get();

    //업데이트
    static void update();

    //초기화
    static void init();

    //해제
    static void deinit();

    //밀리세컨드 기준 현재 시간 가저옴
    static unsigned getTime();

    //프로세스 시작한 이후부터 클락 틱 가져옴
    static unsigned long getClock();


private:
    //싱글턴으로 쓰기 위해 생성자 PRIVATE
    TimingData() {}
    TimingData(const TimingData &) {}
    TimingData& operator=(const TimingData &);
};