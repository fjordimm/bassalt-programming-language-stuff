
////////// calling them 'joint pointers' now

#include <cstdio>
#include <chrono>
#include <random>
#include <cstdlib>
#include <memory>
#include <vector>

// Forward declarations
std::chrono::steady_clock::time_point makeClock();
float clockTime(const std::chrono::steady_clock::time_point& clock);
std::size_t randNum(std::size_t lowerBound, std::size_t upperBound);
void benchmarkRawPtr(const std::size_t numObjs, const std::size_t numActions);
void benchmarkSharedPtr(const std::size_t numObjs, const std::size_t numActions);
void benchmarkDependencyPtr(const std::size_t numObjs, const std::size_t numActions);
void benchmarkDynamicDependencyPtr(const std::size_t numObjs, const std::size_t numActions);

int main(void)
{
	const std::size_t numObjs = 1500000;
	const std::size_t numActions = 10000000;

	// benchmarkRawPtr(numObjs, numActions);
	// benchmarkSharedPtr(numObjs, numActions);
	benchmarkDependencyPtr(numObjs, numActions);

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
	float tDeletionOfArrays;

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

				Object*& depobj = depobjects[indexDepobj];
				if (depobj != nullptr)
				{ float a = depobj->a; }
			}
		}

		tActions = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			delete objects[i];
		}

		tDeletion = clockTime(clock);

		delete[] objects;
		delete[] depobjects;

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
	std::printf("    Object: %zub\n", sizeof(Object));
	std::printf("    raw pointer: %zub\n", sizeof(Object*));
}

struct Object_s
{
	float a;
	float b;
	float c;
};

void benchmarkSharedPtr(const std::size_t numObjs, const std::size_t numActions)
{
	using Object = Object_s;

	std::printf("Benchmarking shared pointers...\n");

	auto clock = makeClock();
	float tCreationOfArray;
	float tObjInitialization;
	float tActions;
	float tDeletion;
	float tDeletionOfArrays;

	{
		std::shared_ptr<Object>* objects = new std::shared_ptr<Object>[numObjs];
		std::weak_ptr<Object>* depobjects = new std::weak_ptr<Object>[numObjs];
		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i] = nullptr;
			depobjects[i].reset();
		}

		tCreationOfArray = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i] = std::make_shared<Object>();
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

				std::weak_ptr<Object>& depobj = depobjects[indexDepobj];
				if (!depobj.expired())
				{ float a = depobj.lock()->a; }
			}
		}

		tActions = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			objects[i].reset();
		}

		tDeletion = clockTime(clock);

		delete[] objects;
		delete[] depobjects;

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
	std::printf("    Object: %zub\n", sizeof(Object));
	std::printf("    shared pointer: %zub\n", sizeof(std::shared_ptr<Object>));
	std::printf("    weak pointer: %zub\n", sizeof(std::weak_ptr<Object>));
}

struct Object_d
{
	float a;
	float b;
	float c;

	Object_d** dep;
};

void benchmarkDependencyPtr(const std::size_t numObjs, const std::size_t numActions)
{
	using Object = Object_d;

	std::printf("Benchmarking dependency pointers...\n");

	auto clock = makeClock();
	float tCreationOfArray;
	float tObjInitialization;
	float tActions;
	float tDeletion;
	float tDeletionOfArrays;

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
			objects[i]->dep = nullptr;
		}

		tObjInitialization = clockTime(clock);

		for (std::size_t r = 0; r < numActions; r++)
		{
			int actn = randNum(0, 1);

			if (actn == 0) // set a random item of depobjects to point to a random item of objects
			{
				std::size_t indexDepobj = randNum(0, numObjs - 1);
				std::size_t indexObj = randNum(0, numObjs - 1);

				if (depobjects[indexDepobj] != nullptr)
				{ depobjects[indexDepobj]->dep = nullptr; }
				depobjects[indexDepobj] = objects[indexObj];
				depobjects[indexDepobj]->dep = &(depobjects[indexObj]);
			}
			else if (actn == 1) // read the value from a random item of depobjects
			{
				std::size_t indexDepobj = randNum(0, numObjs - 1);

				Object*& depobj = depobjects[indexDepobj];
				if (depobj != nullptr)
				{ float a = depobj->a; }
			}
		}

		tActions = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			if (objects[i]->dep != nullptr)
			{ *(objects[i]->dep) = nullptr; }
			delete objects[i];
		}

		tDeletion = clockTime(clock);

		delete[] objects;
		delete[] depobjects;

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
	std::printf("    Object: %zub\n", sizeof(Object));
	std::printf("    raw pointer: %zub\n", sizeof(Object*));
}

struct Object_dd
{
	float a;
	float b;
	float c;

	std::vector<Object_dd*> deps;
};

void benchmarkDynamicDependencyPtr(const std::size_t numObjs, const std::size_t numActions)
{
	using Object = Object_dd;

	std::printf("Benchmarking dynamic dependency pointers...\n");

	auto clock = makeClock();
	float tCreationOfArray;
	float tObjInitialization;
	float tActions;
	float tDeletion;
	float tDeletionOfArrays;

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
			// objects[i]->deps is also constructed
		}

		tObjInitialization = clockTime(clock);

		for (std::size_t r = 0; r < numActions; r++)
		{
			int actn = randNum(0, 1);

			if (actn == 0) // set a random item of depobjects to point to a random item of objects
			{
				std::size_t indexDepobj = randNum(0, numObjs - 1);
				std::size_t indexObj = randNum(0, numObjs - 1);

				if (depobjects[indexDepobj] != nullptr)
				{
					for (std::size_t i = 0; i < depobjects[indexDepobj]->deps.size(); i++)
					{
						if (depobjects[indexDepobj]->deps[i] == depobjects[indexDepobj])
						{ depobjects[indexDepobj]->deps.remo }
					}
				}
				depobjects[indexDepobj] = objects[indexObj];
				
				// depobjects[indexDepobj]->deps.
				// depobjects[indexDepobj]->dep = &(depobjects[indexObj]);
			}
			else if (actn == 1) // read the value from a random item of depobjects
			{
				std::size_t indexDepobj = randNum(0, numObjs - 1);

				Object*& depobj = depobjects[indexDepobj];
				if (depobj != nullptr)
				{ float a = depobj->a; }
			}
		}

		tActions = clockTime(clock);

		for (std::size_t i = 0; i < numObjs; i++)
		{
			if (objects[i]->dep != nullptr)
			{ *(objects[i]->dep) = nullptr; }
			delete objects[i];
		}

		tDeletion = clockTime(clock);

		delete[] objects;
		delete[] depobjects;

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
	std::printf("    Object: %zub\n", sizeof(Object));
	std::printf("    raw pointer: %zub\n", sizeof(Object*));
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
