#pragma once
class Timer
{

public:

	Timer();
	~Timer();

	int fps = 0;
	int frames = 0;
	float frameTimer = 0;

	float totalRunTimeSeconds = 0;
	float lastTimeSeconds = 0;
	float DeltaTime = 0;


	float RecordNewTime();

protected:
	float GetCurrentTimeSeconds();


private:
	void CalculateFrames(float DeltaTime);
};

