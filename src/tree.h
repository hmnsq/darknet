#ifndef TREE_H
#define TREE_H
#include "darknet.h"

පූර්ණ_සංඛ්‍යා hierarchy_top_prediction(float *predictions, tree *hier, float thresh, පූර්ණ_සංඛ්‍යා stride);
float get_hierarchy_probability(float *x, tree *hier, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා stride);

#endif
