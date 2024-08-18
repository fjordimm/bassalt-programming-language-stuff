
#include <cstdio>
#include "Dptr.hpp"

int main(void)
{
	std::printf("Start of program\n");

	dptr<float> d = dptr<float>::make(2.99832f);
	std::printf("d has %f\n", *d);
	wdptr<float> wd = wdptr<float>::from(d);
	std::printf("d has %f\n", *d);
	std::printf("wd has %f\n", *wd);
	*d = 0.001f;
	std::printf("d has %f\n", *d);
	std::printf("wd has %f\n", *wd);
	*wd = 111.888f;
	std::printf("d has %f\n", *d);
	std::printf("wd has %f\n", *wd);

	return 0;
}
