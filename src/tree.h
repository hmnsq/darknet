#ifndef TREE_H
#define TREE_H
#include "darknet.h"

정수 hierarchy_top_prediction(float *predictions, tree *hier, float thresh, 정수 stride);
float get_hierarchy_probability(float *x, tree *hier, 정수 c, 정수 stride);

#endif
