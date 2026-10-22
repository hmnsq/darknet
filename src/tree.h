#ifndef TREE_H
#define TREE_H
#include "darknet.h"

จำนวนเต็ม hierarchy_top_prediction(float *predictions, tree *hier, float thresh, จำนวนเต็ม stride);
float get_hierarchy_probability(float *x, tree *hier, จำนวนเต็ม c, จำนวนเต็ม stride);

#endif
