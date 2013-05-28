#include "adaboost.h"
#include <cstdio>
#include <cstdlib>
using namespace std;

int main(int argc, const char* argv[])
{
	vector<AdaBoostFeature*> afeatures;
	const char* const filename = "billabingbong.txt";
	if(argc < 5)
	{
		printf("\nUsage: %s n_positive n_negative "
		       "how_many_feature n_random_feature\n\n", argv[0]);
		return 1;
	}
	unsigned int n_pos = atoi(argv[1]);
	unsigned int n_neg = atoi(argv[2]);
	unsigned int how_many = atoi(argv[3]);
	unsigned int n_random = atoi(argv[4]);
	afeatures = RunAdaBoost(n_pos, n_neg, how_many, n_random);
	SaveAdaBoost(afeatures, filename);
	return 0;
}

