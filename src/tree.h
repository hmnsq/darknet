#ifndef TREE_H
#define TREE_H
#include "darknet.h"

ཧྲིལ hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ཧྲིལ stride);
float get_hierarchy_probability(float *x, tree *hier, ཧྲིལ c, ཧྲིལ stride);

#endif
