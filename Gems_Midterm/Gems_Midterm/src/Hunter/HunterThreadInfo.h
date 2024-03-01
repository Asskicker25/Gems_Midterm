#pragma once
#include <Windows.h>

class Hunter;

#pragma once
class HunterThreadInfo
{
public:
    Hunter* mHunter = nullptr;

    double mDesiredUpdateTime = 0.0f;
    bool mIsRunning = false;
    bool isAlive = true;

    DWORD sleepTime = 0;
    DWORD ThreadId = 0;
    HANDLE threadHandle = 0;

    CRITICAL_SECTION* mMaze_CS = nullptr;
};

