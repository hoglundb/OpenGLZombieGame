#include "Random.h"
#include<stdlib.h>
#include<cmath>


	 float Random::Randf(float low, float high)
	{
		float r = (float)rand();		// 0 - RAND_MAX
		float t = r / (float)RAND_MAX;	// 0. - 1.

		return   low + t * (high - low);
	}

	 int Random::Randf(int ilow, int ihigh)
	{
		float low = (float)ilow;
		float high = ceil((float)ihigh);

		return (int)Randf(low, high);
	}
