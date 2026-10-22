#ifndef TREE_H
#define TREE_H
#include "darknet.h"

số_nguyên hierarchy_top_prediction(float *predictions, tree *hier, float thresh, số_nguyên stride);
float get_hierarchy_probability(float *x, tree *hier, số_nguyên c, số_nguyên stride);

#endif
