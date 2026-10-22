#ifndef TREE_H
#define TREE_H
#include "darknet.h"

ціле hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ціле stride);
float get_hierarchy_probability(float *x, tree *hier, ціле c, ціле stride);

#endif
