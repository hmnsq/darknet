#ifndef TREE_H
#define TREE_H
#include "darknet.h"

цяло hierarchy_top_prediction(float *predictions, tree *hier, float thresh, цяло stride);
float get_hierarchy_probability(float *x, tree *hier, цяло c, цяло stride);

#endif
