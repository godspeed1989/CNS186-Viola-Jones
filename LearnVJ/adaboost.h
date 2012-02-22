#ifndef LEARNVJ_ADABOOST_H
#define LEARNVJ_ADABOOST_H

#include "cv.h"

#include <pair>
#include <set>
#include <vector>

using namespace cv;
using namespace std;

struct Feature;

struct AdaBoostFeature {
    Feature* feature;
    int threshold;
    int polarity; // -1 or 1
    double beta_t; 
};

bool operator< (const AdaboostFeature& left, const AdaBoostFeature& right) {
    if(left->feature < right->feature) { return true; } if(right->feature < left->feature) { return false; }
    if(left->threshold < right->threshold) { return true; } if(right->threshold < left->threshold) { return false; }
    if(left->polarity < right->polarity) { return true; } if(right->polarity < left->polarity) { return false; }
    return false;
}

// Runs the overall adaboost algorithm, which_faces is the last face in the training set, which_not_faces is the last face in the 
// negative training set, how_many is the number of features to return, total_set is how many to randomly generate.
vector<AdaBoostFeature*> RunAdaBoost(int which_faces, int which_not_faces, int how_many, int total_set);

// Runs one round of the adaboost algorithm (calculates errors, finds best features, returns thresh, feat, pol). 
// Also updates weightings. Modifies weightings correctly and removes the selected feature from the feature set.
AdaBoostFeature* RunAdaBoostRound(const vector<Mat> pos_iis, const vector<Mat> neg_iis, vector<double>* pos_weights, vector<double>* neg_weights, 
                                  set<Feature*>* feature_set);

//Given a set of positive and negative values of a particular feature, finds best threshold and polarity also returns error. 
Pair< Pair<int, int>, double> FindThresholdAndPolarity(vector<int> positive_examples, vector<int> negative_examples, vector<double> pos_weights, 
                                                       vector<double> neg_weights, int feature_value);

void SaveAdaBoost(vector<AdaBoostFeature*> to_save); 

#endif