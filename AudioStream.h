#pragma once

#include "GPIO.h"

#include <thread>
#include <cstdio>
#include <unistd.h>

class AudioStream {
private:
    int bitClockInputPin;
    int bitClockOutputPin;
    int frameClockOutputPin;
    int audioDataCh1OutputPin;
    int audioDataCh2OutputPin;
    int audioDataCh3OutputPin;

    volatile uint64_t isrGot = 0;
    volatile uint64_t isrProcessed = 0;
    volatile uint64_t isrScalerCounter = 0;
    volatile uint64_t isrScalerTarget = 100;
    volatile bool clockState = false;

    volatile int currentBit = -1;
    volatile uint64_t currentBitMask = 0;
    volatile uint64_t ch1Data = 10000;
    volatile uint64_t ch2Data = 10000;
    volatile uint64_t ch3Data = 10000;

    std::thread* isrGetterThread;
    std::thread* writerThread;

    void ISRGetterThreadRoutine();
    void WriterThreadRoutine();
    void WriteISR();

    volatile uint64_t queueReadPos = 0;
    volatile uint64_t queueWritePos = 0;

    volatile uint64_t queue_ch1[1048576];
    volatile uint64_t queue_ch2[1048576];
    volatile uint64_t queue_ch3[1048576];

public:
    AudioStream(int freqIn, int freqOut, int lrck, int ch1, int ch2, int ch3);
    void EnqueueSample(uint32_t l1, uint32_t r1, uint32_t l2, uint32_t r2, uint32_t l3, uint32_t r3);
};