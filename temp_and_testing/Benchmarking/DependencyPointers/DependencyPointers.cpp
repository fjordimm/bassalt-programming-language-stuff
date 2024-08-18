
#include <cstdio>
#include <chrono>
#include <random>
#include <cstdlib>
#include <memory>

// Forward declarations
std::chrono::steady_clock::time_point makeClock();
float clockTime(const std::chrono::steady_clock::time_point& clock);
std::size_t randNum(std::size_t lowerBound, std::size_t upperBound);
void benchmarkRawPtr(const std::size_t numObjs, const std::size_t numActions);

int main(void)
{
	const std::size_t numObjs = 150;
	const std::size_t numActions = 9000000;

	benchmarkRawPtr(numObjs, numActions);

	return 0;
}

struct Object_r
{
	float a;
	float b;
	float c;
};

void benchmarkRawPtr(const std::size_t numObjs, const std::size_t numActions)
{
	using Object = Object_r;

	std::printf("Benchmarking raw pointers...\n");

	auto clock = makeClock();
	float tCreationOfArray;
	float tObjInitialization;
	float tActions;
	float tDeletion;

	{
		Object** objects = new Object*[numObjs];
		Object** depobjects = new Object*[numObjs];
		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i] = nullptr;
			depobjects[i] = nullptr;
		}

		tCreationOfArray = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i] = new Object();
			objects[i]->a = (float)randNum(0, 100) / 100.0f;
			objects[i]->b = (float)randNum(0, 100) / 100.0f;
			objects[i]->c = (float)randNum(0, 100) / 100.0f;
		}

		tObjInitialization = clockTime(clock);

		for (std::size_t r = 0; r < numActions; r++)
		{
			int actn = randNum(0, 1);

			if (actn == 0) // set a random item of depobjects to point to a random item of objects
			{
				std::size_t indexDepobj = randNum(0, numObjs - 1);
				std::size_t indexObj = randNum(0, numObjs - 1);

				depobjects[indexDepobj] = objects[indexObj];
			}
			else if (actn == 1) // read the value from a random item of depobjects
			{
				std::size_t indexDepobj = randNum(0, numObjs - 1);

				Object* depobj = depobjects[indexDepobj];
				if (depobj != nullptr)
				{ float a = depobj->a; }
			}
		}

		tActions = clockTime(clock);

		delete[] objects;
		delete[] depobjects;

		tDeletion = clockTime(clock);
	}

	std::printf("...Done\n");
	std::printf("  Times:\n");
	std::printf("    creation of array:     %f\n", tCreationOfArray);
	std::printf("    object initialization: %f\n", tObjInitialization - tCreationOfArray);
	std::printf("    actions:               %f\n", tActions - tObjInitialization);
	std::printf("    deletion:              %f\n", tDeletion - tActions);
	std::printf("  Memory:\n");
	std::printf("    Object: %zub\n", sizeof(Object));
	std::printf("    pointer: %zub\n", sizeof(Object*));
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
