
#include <cstdio>
#include <memory>
#include <chrono>
#include <random>
#include "nptr.hpp"
#include "uptr.hpp"

// Forward declarations
std::chrono::steady_clock::time_point makeClock();
float clockTime(const std::chrono::steady_clock::time_point& clock);
std::size_t randNum(std::size_t lowerBound, std::size_t upperBound);

int main(void)
{
	std::printf("Start\n");

	for (size_t i = 0; i < 30; i++)
	{
		std::printf("%zu\n", randNum(i, i+1));
	}
	
	std::printf("End\n");
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

std::size_t randNum(std::size_t lowerBound, std::size_t upperBound)
{
	static long long seed = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
	static std::default_random_engine gen(seed);
	std::uniform_int_distribution<std::size_t> dist(lowerBound, upperBound);
	static std::size_t _ = dist(gen); // to make it go once
	
	return dist(gen);
}
