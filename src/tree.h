#ifndef TREE_H
#define TREE_H
#include "darknet.h"

ကိန်းပြည့် hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ကိန်းပြည့် stride);
float get_hierarchy_probability(float *x, tree *hier, ကိန်းပြည့် c, ကိန်းပြည့် stride);

#endif
