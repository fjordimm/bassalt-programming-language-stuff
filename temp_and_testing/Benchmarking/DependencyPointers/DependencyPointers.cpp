
#include <cstdio>
#include <chrono>

// void Head::CalculateDeltaTime(float& deltaTime, float& totalTime)
// {
// 	using std::chrono::steady_clock;
// 	using std::chrono::duration;
// 	using durMillisecs = duration<float, std::ratio<1, 1000>>;

// 	static steady_clock::time_point initialTimePoint = steady_clock::now();

// 	static steady_clock::time_point timePoint = steady_clock::now();
// 	steady_clock::time_point oldTimePoint = timePoint;
// 	timePoint = steady_clock::now();
// 	durMillisecs timeElapsedSinceLastFrame = std::chrono::duration_cast<durMillisecs>(timePoint - oldTimePoint);
// 	deltaTime = timeElapsedSinceLastFrame.count();

// 	durMillisecs timeElapsedSinceStart = std::chrono::duration_cast<durMillisecs>(timePoint - initialTimePoint);
// 	totalTime = timeElapsedSinceStart.count();
// }

// Forward declarations
std::chrono::steady_clock::time_point makeClock();
float clockTime(const std::chrono::steady_clock::time_point& clock);

int main(void)
{
	std::printf("yoooo what up world\n");

	auto clock = makeClock();
	for (int i = 0; i < 100000; i++)
	{
		float z = ((float)i + 392.4) / 1.0000000012434;
	}
	float timeElapsed = clockTime(clock);
	std::printf("time elapsed: %f\n", timeElapsed);

	return 0;
}

std::chrono::steady_clock::time_point makeClock()
{
	using std::chrono::steady_clock;
	using std::chrono::duration;
	using durMillisecs = duration<float, std::ratio<1, 1000>>;

	return steady_clock::now();
}

float clockTime(const std::chrono::steady_clock::time_point& clock)
{
	using std::chrono::steady_clock;
	using std::chrono::duration;
	using durMillisecs = duration<float, std::ratio<1, 1000>>;

	steady_clock::time_point newNow = steady_clock::now();
	durMillisecs timeElapsedSinceLastFrame = std::chrono::duration_cast<durMillisecs>(newNow - clock);
	return timeElapsedSinceLastFrame.count();
}
