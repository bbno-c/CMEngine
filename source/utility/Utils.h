#include <chrono>

class CUtils
{
public:
	CUtils() = delete;

	static void InitOnceSDL2();
	static void ValidateSDL2Errors();
};

class CChronometer
{
public:
	CChronometer();
	float GrabDeltaTime();

private:
	std::chrono::system_clock::time_point m_lastTime;
};