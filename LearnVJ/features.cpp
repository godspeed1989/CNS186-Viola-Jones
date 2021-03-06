#include "features.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace cv;

// Check whether the feature fit into SUBWINDOW_SIZE*SUBWINDOW_SIZE
static bool IsValidFeature(Feature* to_check)
{
	if(to_check->x1 > to_check->x2) { return false; }
	if(to_check->y1 > to_check->y2) { return false; }
	if(to_check->x1 < 0 || to_check->x2 >= SUBWINDOW_SIZE) { return false; }
	if(to_check->y1 < 0 || to_check->y2 >= SUBWINDOW_SIZE) { return false; }
	if((to_check->type == TWO_REC_HORIZ || to_check->type == THREE_REC_HORIZ || 
		to_check->type == FOUR_REC) && ((to_check->x2 + (to_check->x2 - to_check->x1)) >= SUBWINDOW_SIZE)) {
		return false;
	}
	if((to_check->type == TWO_REC_VERT || to_check->type == THREE_REC_VERT || 
		to_check->type == FOUR_REC) && ((to_check->y2 + (to_check->y2 - to_check->y1)) >= SUBWINDOW_SIZE)) {
		return false;
	}
	if((to_check->type == THREE_REC_HORIZ) && 
			((to_check->x2 + 2*(to_check->x2 - to_check->x1)) >= SUBWINDOW_SIZE)) {
		return false;
	}
	if((to_check->type == THREE_REC_VERT) && 
			((to_check->y2 + 2*(to_check->y2 - to_check->y1)) >= SUBWINDOW_SIZE)) {
		return false;
	}
	return true;
}

array<Feature*>* GenerateAllFeatures(int step)
{
	int x, y, w, h, type;
	int width, height;
	Feature feature;
	array<Feature*>* storage = new array<Feature*>();
	for(type = 0; type < FOUR_REC+1; type++) //type
	{
		for(w = 1; w < SUBWINDOW_SIZE; w += step) //width
		{
			switch(type)
			{
				case TWO_REC_HORIZ:      width = 2*w; break;
				case THREE_REC_HORIZ:    width = 3*w; break;
				case FOUR_REC:           width = 2*w; break;
				default:                 width = w;   break;
			}
			if(width > SUBWINDOW_SIZE)
				break;
			for(h = 1; h < SUBWINDOW_SIZE; h += step) //height
			{
				switch(type)
				{
					case TWO_REC_VERT:      height = 2*h; break;
					case THREE_REC_VERT:    height = 3*h; break;
					case FOUR_REC:          height = 2*h; break;
					default:                height = h;   break;
				}
				if(height > SUBWINDOW_SIZE)
					break;
				// UL position
				for(x = 0; x < SUBWINDOW_SIZE-width; x += step)
				{
					for(y = 0; y < SUBWINDOW_SIZE-height; y += step)
					{
						feature.type = (FeatureTypeT)type;
						feature.x1 = x;
						feature.y1 = y;
						feature.x2 = x+w;
						feature.y2 = y+h;
						if(IsValidFeature(&feature))
						{
							Feature* new_creation = new Feature(feature);
							storage->push_back(new_creation);
						}
					}
				}
			}// for height
		}// for width
	}// for type
	return storage;
}

array<Feature*>* GenerateRandomFeatures(int num_features)
{
	array<Feature*>* storage = new array<Feature*>();
	srand(time(NULL));
	for(int i=0; i<num_features; ++i)
	{
		double lowest = 0; double highest = FOUR_REC; double range = (highest - lowest) + 1;
		int type = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));

		int x1=0, y1=0, x2=0, y2=0;
		if(type == TWO_REC_HORIZ || type == TWO_REC_VERT) {
			lowest = 0; highest = SUBWINDOW_SIZE - 1 - 2; range = (highest - lowest) + 1;
			x1 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 0; highest = SUBWINDOW_SIZE - 1 - 1; range = (highest - lowest) + 1;
			y1 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - x1) / 2; range = (highest - lowest) + 1; 
			int x_diff = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			x2 = x1 + x_diff;
			lowest = y1 + 1; highest = SUBWINDOW_SIZE - 1; range = (highest - lowest) + 1;
			y2 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1))); 
		} // For type = 1 we will simply reverse x and y
		else if(type == THREE_REC_HORIZ || type == THREE_REC_VERT) { 
			lowest = 0; highest = SUBWINDOW_SIZE - 1 - 3; range = (highest - lowest) + 1;
			x1 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 0; highest = SUBWINDOW_SIZE - 1 - 1; range = (highest - lowest) + 1;
			y1 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - x1) / 3; range = (highest - lowest) + 1; 
			int x_diff = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			x2 = x1 + x_diff;
			lowest = y1 + 1; highest = SUBWINDOW_SIZE - 1; range = (highest - lowest) + 1;
			y2 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
		} // For type = 3 we will simply reverse x and y
		else if(type == FOUR_REC) { 
			lowest = 0; highest = SUBWINDOW_SIZE - 1 - 2; range = (highest - lowest) + 1;
			x1 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 0; highest = SUBWINDOW_SIZE - 1 - 2; range = (highest - lowest) + 1;
			y1 = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - x1) / 2; range = (highest - lowest) + 1; 
			int x_diff = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			
			lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - y1) / 2; range = (highest - lowest) + 1;
			int y_diff = lowest + (int)(range * (rand()/((double)RAND_MAX + 1)));
			x2 = x1 + x_diff;
			y2 = y1 + y_diff;
		}
		if(type == TWO_REC_VERT || type == THREE_REC_VERT) {
			int temp = x1; x1 = y1; y1 = temp;
			temp = x2; x2 = y2; y2 = temp;
		}
		Feature* new_creation = new Feature();
		new_creation->x1 = x1;
		new_creation->x2 = x2;
		new_creation->y1 = y1;
		new_creation->y2 = y2;
		new_creation->type = (FeatureTypeT)type;
		
		if(!IsValidFeature(new_creation)) { return NULL; }
		
		storage->push_back(new_creation);
	}
	return storage;
}

// Calculate Feature weight
int CalculateFeature(Feature* feature, const Mat& integral_image)
{
	double current_sum = 0;
	// First rectangle is the same for all.
	current_sum += integral_image.at<double>(feature->y1, feature->x1) + 
				   integral_image.at<double>(feature->y2, feature->x2) -
				   integral_image.at<double>(feature->y1, feature->x2) -
				   integral_image.at<double>(feature->y2, feature->x1);
	// Second rectangle for horizontal 2 , 3 and 4
	if(feature->type == TWO_REC_HORIZ || feature->type == THREE_REC_HORIZ || 
			feature->type == FOUR_REC) {
		int second_x = feature->x2 + (feature->x2 - feature->x1);
		current_sum -= integral_image.at<double>(feature->y1, feature->x2) +
					   integral_image.at<double>(feature->y2, second_x) -
					   integral_image.at<double>(feature->y1, second_x) -
					   integral_image.at<double>(feature->y2, feature->x2);
	}
	// Second rectangle for vertical 2 and 3, third rectangle for 4
	if(feature->type == TWO_REC_VERT || feature->type == THREE_REC_VERT ||
			feature->type == FOUR_REC) {
		int second_y = feature->y2 + (feature->y2 - feature->y1);
		current_sum -= integral_image.at<double>(feature->y2, feature->x1) +
					   integral_image.at<double>(second_y, feature->x2) -
					   integral_image.at<double>(feature->y2, feature->x2) -
					   integral_image.at<double>(second_y, feature->x1);
	}
	// Third rectangle for horizontal 3
	if(feature->type == THREE_REC_HORIZ) {
		int second_x = feature->x2 + (feature->x2 - feature->x1);
		int third_x = feature->x2 + 2*(feature->x2 - feature->x1);
		current_sum += integral_image.at<double>(feature->y1, second_x) +
					   integral_image.at<double>(feature->y2, third_x) -
					   integral_image.at<double>(feature->y1, third_x) -
					   integral_image.at<double>(feature->y2, second_x);
	}
	// Third rectangle for vertical 3
	if(feature->type == THREE_REC_VERT) {
		int second_y = feature->y2 + (feature->y2 - feature->y1);
		int third_y = feature->y2 + 2*(feature->y2 - feature->y1);
		current_sum += integral_image.at<double>(second_y, feature->x1) +
					   integral_image.at<double>(third_y, feature->x2) -
					   integral_image.at<double>(second_y, feature->x2) -
					   integral_image.at<double>(third_y, feature->x1);
	}
	// Fourth rectangle for 4
	if(feature->type == FOUR_REC) {
		int x1 = feature->x2;
		int y1 = feature->y2;
		int x2 = feature->x2 + (feature->x2 - feature->x1);
		int y2 = feature->y2 + (feature->y2 - feature->y1);
		current_sum += integral_image.at<double>(y1, x1) +
					   integral_image.at<double>(y2, x2) -
					   integral_image.at<double>(y1, x2) -
					   integral_image.at<double>(y2, x1);
	}

	return (int)current_sum;
}

