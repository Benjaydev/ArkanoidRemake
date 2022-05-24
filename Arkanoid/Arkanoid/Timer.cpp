#pragma once
#include "Timer.h"

Timer::Timer()
{
    GetCurrentTimeMilliseconds();
    lastTime = currentTime;
}

Timer::~Timer()
{
}




float Timer::RecordNewTime()
{
    GetCurrentTimeMilliseconds();
    std::chrono::duration<float> difference = currentTime - lastTime;
    DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(difference).count();
    // Keep track of this time until next method call
    lastTime = currentTime;

    //DeltaTime /= 1000.0f;

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





float Timer::GetCurrentTimeMilliseconds()
{
    currentTime = std::chrono::system_clock::now();

    return 0.0f;
}

