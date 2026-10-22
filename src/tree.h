#ifndef TREE_H
#define TREE_H
#include "darknet.h"

整数 hierarchy_top_prediction(float *predictions, tree *hier, float thresh, 整数 stride);
float get_hierarchy_probability(float *x, tree *hier, 整数 c, 整数 stride);

#endif
