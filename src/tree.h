#ifndef TREE_H
#define TREE_H
#include "darknet.h"

puur_naa_nka hierarchy_top_prediction(float *predictions, tree *hier, float thresh, puur_naa_nka stride);
float get_hierarchy_probability(float *x, tree *hier, puur_naa_nka c, puur_naa_nka stride);

#endif
