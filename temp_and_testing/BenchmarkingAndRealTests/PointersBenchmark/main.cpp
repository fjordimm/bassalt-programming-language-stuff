
#include <cstdio>
#include <memory>
#include <chrono>
#include <random>
#include "nptr.hpp"
#include "uptr.hpp"

// Forward declarations
std::chrono::steady_clock::time_point makeClock();
float clockTime(std::chrono::steady_clock::time_point const& clock);
std::size_t randNum(std::size_t lowerBound, std::size_t upperBound);
void benchmarkRawPtr(const std::size_t numObjs, const std::size_t numActions);

int main(void)
{
	const std::size_t numObjs = 1500000;
	const std::size_t numActions = 10000000;

	benchmarkRawPtr(numObjs, numActions);
	// benchmarkSharedPtr(numObjs, numActions);
	// benchmarkDependencyPtr(numObjs, numActions);

	return 0;
}

struct SomeStruct
{
	float a;
	float b;
	float c;
};

void benchmarkRawPtr(const std::size_t numObjs, const std::size_t numActions)
{
	std::printf("Benchmarking raw pointers...\n");

	auto clock = makeClock();
	float tCreationOfArray;
	float tObjInitialization;
	float tActions;
	float tDeletion;
	float tDeletionOfArrays;

	{
		SomeStruct** objects = new SomeStruct*[numObjs];
		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i] = nullptr;
		}

		tCreationOfArray = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i] = new SomeStruct();
			objects[i]->a = (float)randNum(0, 100) / 100.0f;
			objects[i]->b = (float)randNum(0, 100) / 100.0f;
			objects[i]->c = (float)randNum(0, 100) / 100.0f;
		}

		tObjInitialization = clockTime(clock);

		for (std::size_t r = 0; r < numActions; r++)
		{
			std::size_t index1 = randNum(0, numObjs - 1);
			std::size_t index2 = randNum(0, numObjs - 1);

			float aAvg = 0.5f * (objects[index1]->a + objects[index2]->a);
			float bAvg = 0.5f * (objects[index1]->b + objects[index2]->b);
			float cAvg = 0.5f * (objects[index1]->c + objects[index2]->c);
			objects[index1]->a = aAvg;
			objects[index1]->b = bAvg;
			objects[index1]->c = cAvg;
		}

		tActions = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			delete objects[i];
		}

		tDeletion = clockTime(clock);

		delete[] objects;

		tDeletionOfArrays = clockTime(clock);
	}

	std::printf("...Done\n");
	std::printf("  Times:\n");
	std::printf("    creation of array:     %f\n", tCreationOfArray);
	std::printf("    object initialization: %f\n", tObjInitialization - tCreationOfArray);
	std::printf("    actions:               %f\n", tActions - tObjInitialization);
	std::printf("    deletion:              %f\n", tDeletion - tActions);
	std::printf("    deletion of arrays:    %f\n", tDeletionOfArrays - tDeletion);
	std::printf("  Memory:\n");
	std::printf("    raw pointer: %zub\n", sizeof(SomeStruct*));
}

std::chrono::steady_clock::time_point makeClock()
{
	using std::chrono::steady_clock;
	using std::chrono::duration;

	return steady_clock::now();
}

float clockTime(std::chrono::steady_clock::time_point const& clock)
{
	using std::chrono::steady_clock;
	using std::chrono::duration;

	steady_clock::time_point newNow = steady_clock::now();
	duration<float, std::milli> timeElapsedSinceLastFrame = std::chrono::duration_cast<duration<float, std::milli>>(newNow - clock);
	return timeElapsedSinceLastFrame.count();
}

std::size_t randNum(std::size_t lowerBound, std::size_t upperBound)
{
	static long long seed = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
	static std::default_random_engine gen(seed);
	std::uniform_int_distribution<std::size_t> dist(lowerBound, upperBound);
	static std::size_t _ = dist(gen); // to make it go once
	(void)_;
	
	return dist(gen);
}
