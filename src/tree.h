#ifndef TREE_H
#define TREE_H
#include "darknet.h"

цео hierarchy_top_prediction(float *predictions, tree *hier, float thresh, цео stride);
float get_hierarchy_probability(float *x, tree *hier, цео c, цео stride);

#endif
