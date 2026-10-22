#ifndef TREE_H
#define TREE_H
#include "darknet.h"

ຈຳນວນເຕັມ hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ຈຳນວນເຕັມ stride);
float get_hierarchy_probability(float *x, tree *hier, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ stride);

#endif
