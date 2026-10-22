#ifndef TREE_H
#define TREE_H
#include "darknet.h"

целое hierarchy_top_prediction(float *predictions, tree *hier, float thresh, целое stride);
float get_hierarchy_probability(float *x, tree *hier, целое c, целое stride);

#endif
