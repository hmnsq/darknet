#ifndef TREE_H
#define TREE_H
#include "darknet.h"

ចំនួនគត់ hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ចំនួនគត់ stride);
float get_hierarchy_probability(float *x, tree *hier, ចំនួនគត់ c, ចំនួនគត់ stride);

#endif
