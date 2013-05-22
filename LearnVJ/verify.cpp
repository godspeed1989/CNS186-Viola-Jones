#include "adaboost.h"
#include "features.h"
#include "../IntegralImage/integral_image.h"
#include <cstdio>
#include <fstream>
using namespace cv;
using namespace std;

void load_cascade(vector<AdaBoostFeature*>& first_set, const char *file);
void verify(const vector<AdaBoostFeature*> &features, const char *imfile);

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s image cascade\n", argv[0]);
		return 1;
	}
	vector<AdaBoostFeature*> first_set;
	load_cascade(first_set, argv[2]);
	verify(first_set, argv[1]);
	return 0;
}

void verify(const vector<AdaBoostFeature*> &afeatures, const char * imfile)
{
	Mat orig;
	orig = imread(imfile, 0);
	int img_width = orig.cols;
	int img_height = orig.rows;
	while(img_width > SUBWINDOW_SIZE && img_height > SUBWINDOW_SIZE)
	{
		int found;
		Mat img, iimg;
		resize(orig, img, Size(img_width, img_height));
		iimg = IntegralImage(img);
		found = 0;
		for(int x = 1; x < img_width - SUBWINDOW_SIZE; x++)
		{
			for(int y = 1; y < img_height - SUBWINDOW_SIZE; y++)
			{
				double stage_sum = 0;
				double stage_threshold = 0;
				for(size_t fi = 0; fi < afeatures.size(); fi++)
				{
					double alpha;
					Feature feature;
					int feature_val, polarity, threshold;
					alpha = -log(afeatures[fi]->beta_t);
					polarity = afeatures[fi]->polarity;
					threshold = afeatures[fi]->threshold;
					feature.type = afeatures[fi]->feature->type;
					feature.x1 = afeatures[fi]->feature->x1 + x;
					feature.x2 = afeatures[fi]->feature->x2 + x;
					feature.y1 = afeatures[fi]->feature->y1 + y;
					feature.y2 = afeatures[fi]->feature->y2 + y;
					feature_val = CalculateFeature(&feature, iimg);
					if(feature_val * polarity < threshold * polarity)
						stage_sum += alpha;
					stage_threshold += alpha;
				}
				if(stage_sum >= 0.5*stage_threshold)// pass the stage
				{
					Point pt1, pt2;
					Scalar scalar(0, 0, 0, 0);
					found++;
					pt1.x = x; pt2.x = x + SUBWINDOW_SIZE;
					pt1.y = y; pt2.y = y + SUBWINDOW_SIZE;
					rectangle(img, pt1, pt2, scalar);
				}
			}
		}
		if(found)
		{
			printf("Found %d at %d x %d\n", found, img_width, img_height);
			imshow("Image", img);
			waitKey();
		}
		img_width *= 0.8;
		img_height *= 0.8;
	}
}

void load_cascade(vector<AdaBoostFeature*>& first_set, const char *file)
{
	AdaBoostFeature * afeature;
	int size, count, num;
	ifstream fin;
	fin.open(file);
	fin >> size >> count;
	printf("size %d\n", size);
	num = 0;
	first_set.clear();
	while(!fin.eof() && num < count)
	{
		int type;
		afeature = new AdaBoostFeature;
		afeature->feature = new Feature;
		fin >> type;
		afeature->feature->type = (FeatureTypeT)type;
		fin >> afeature->feature->x1 >> afeature->feature->y1
			>> afeature->feature->x2 >> afeature->feature->y2
			>> afeature->threshold >> afeature->polarity
			>> afeature->beta_t >> afeature->false_pos_rate;
		first_set.push_back(afeature);
		num++;
	}
	printf("%d features loaded\n", num);
	fin.close();
}

