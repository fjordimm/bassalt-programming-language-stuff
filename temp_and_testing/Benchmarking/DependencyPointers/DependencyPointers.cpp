
#include <cstdio>
#include <chrono>
#include <random>
#include <cstdlib>

// Forward declarations
std::chrono::steady_clock::time_point makeClock();
float clockTime(const std::chrono::steady_clock::time_point& clock);
std::size_t randNum(std::size_t lowerBound, std::size_t upperBound);
void benchmarkRawPtr(const std::size_t n);

int main(void)
{
	std::printf("yoooo what up world\n");

	for (int i = 0; i < 15; i++)
	{
		std::printf("random number = %zu\n", randNum(1, 3));
	}

	// auto clock = makeClock();
	// for (int i = 0; i < 100000; i++)
	// {
	// 	float z = ((float)i + 392.4) / 1.0000000012434;
	// }
	// float timeElapsed = clockTime(clock);
	// std::printf("time elapsed: %f\n", timeElapsed);

	return 0;
}

void benchmarkRawPtr(const std::size_t n)
{
	std::printf("Benchmarking raw pointers...\n");
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
	static std::uniform_int_distribution<std::size_t> dist(lowerBound, upperBound);
	static std::size_t _ = dist(gen); // to make it go once
	
	return dist(gen);
}
