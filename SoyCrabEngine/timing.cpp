#include "timing.h"

//�����ս� ī���Ϳ� �÷���
static bool qpcFlag;

#if (__APLPE__ || __unix)
#define TIMING_UNIX	1

#include <stdlib.h>
#include <sys/time.h>

// UNIX TYPE
typedef unsigned long long	LONGLONG;
#else
#define TIMING_WINDOWS	1

// ������ Ÿ�̸� ������ (c. 4�и� ������ Ÿ�̹�).
#include <windows.h>
#include <mmsystem.h>

static double qpcFrequency;
#endif

//���νð� ������
unsigned systemTime()
{
#if TIMING_UNIX
	struct timeval tv;
	gettimeofday(&tv, 0);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;

#else
	if (qpcFlag)
	{
		static LONGLONG qpcMillisPerTick;
		QueryPerformanceCounter((LARGE_INTEGER*)&qpcMillisPerTick);
		return (unsigned)(qpcMillisPerTick * qpcFrequency);
	}
	else
	{
		return unsigned(timeGetTime());
	}
#endif

}

unsigned TimingData::getTime()
{
	return systemTime();
}

#if TIMING_WINDOWS
unsigned long systemClock()
{
#if _MSC_VER
#  ifdef _M_IX86
	__asm {
		rdtsc;
	}
#  else // _M_X64
	return __rdtsc();
#  endif
#else
#  ifdef __i386__
	unsigned long long int x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
#  else // __x86_64__
	unsigned hi, lo;
	__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
#  endif
#endif // _MSC_VER
}
#endif

unsigned long TimingData::getClock()
{

#if TIMING_UNIX
	struct timeval tv;
	gettimeofday(&tv, 0);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#else
	return systemClock();
#endif
}

//Ÿ�̹� �ý��� ���� �� �����ս� ī���� ���
void initTime()
{
#if TIMING_UNIX
	qpcFlag = false;
#else
	LONGLONG time;

	qpcFlag = (QueryPerformanceFrequency((LARGE_INTEGER*)&time) > 0);

	if (qpcFlag) qpcFrequency = 1000.0 / time;
#endif
}


//�̱��� �ν��Ͻ�
static TimingData* timingData = NULL;

//�̱��� ��������
TimingData& TimingData::get()
{
	return (TimingData&)*timingData;
}

void TimingData::update()
{
	if (!timingData) return;

	//������ �ѹ� ����
	if (!timingData->isPaused)
	{
		timingData->frameNumber++;
	}

	//Ÿ�̹� ���� ������Ʈ
	unsigned thisTime = systemTime();
	timingData->lastFrameDuration = thisTime -
		timingData->lastFrameTimestamp;
	timingData->lastFrameTimestamp = thisTime;

	//ƽ ���� ������Ʈ
	unsigned long thisClock = getClock();
	timingData->lastFrameClockTicks =
		thisClock - timingData->lastFrameClockstamp;
	timingData->lastFrameClockstamp = thisClock;

	//��� ������ �ð� ������Ʈ
	if (timingData->frameNumber > 1) {
		if (timingData->averageFrameDuration <= 0)
		{
			timingData->averageFrameDuration =
				(double)timingData->lastFrameDuration;
		}
		else
		{
			//100������ �Ѿ ���
			timingData->averageFrameDuration *= 0.99;
			timingData->averageFrameDuration +=
				0.01 * (double)timingData->lastFrameDuration;

			// �������� �ʴ������� ����
			timingData->fps =
				(float)(1000.0 / timingData->averageFrameDuration);
		}
	}
}

void TimingData::init()
{
	initTime();

	//Ÿ�̹� �ν��Ͻ� ������ ����
	if (!timingData) timingData = new TimingData();

	timingData->frameNumber = 0;

	timingData->lastFrameTimestamp = systemTime();
	timingData->lastFrameDuration = 0;

	timingData->lastFrameClockstamp = getClock();
	timingData->lastFrameClockTicks = 0;

	timingData->isPaused = false;

	timingData->averageFrameDuration = 0;
	timingData->fps = 0;
}

void TimingData::deinit()
{
	delete timingData;
	timingData = NULL;
}
