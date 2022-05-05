#pragma once
#include "Timer.h"
#include <chrono>
using namespace std;

Timer::Timer()
{
    lastTimeSeconds = GetCurrentTimeSeconds();
}

Timer::~Timer()
{
}




float Timer::RecordNewTime()
{
    float newTimeSeconds = GetCurrentTimeSeconds();
    DeltaTime = newTimeSeconds - lastTimeSeconds;
    // Keep track of this time until next method call
    lastTimeSeconds = newTimeSeconds;

    totalRunTimeSeconds += DeltaTime;

    CalculateFrames(DeltaTime);

    return DeltaTime;
}

void Timer::CalculateFrames(float DeltaTime)
{
    frameTimer += DeltaTime;
    if (frameTimer >= 1) {
        fps = frames;
        frames = 0;
        frameTimer = 0;
    }
    frames++;
}





float Timer::GetCurrentTimeSeconds()
{
    auto time = std::chrono::high_resolution_clock::now();
    auto timeMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();

    return (timeMilliseconds / 1000.0f);
}

