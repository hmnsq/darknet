#ifndef पुनो_H
#define पुनो_H
#include "darknet.h"

int hierarchy_इताअस_prediction(float *predictions, पुनो *hier, float thresh, int stride);
float get_hierarchy_probability(float *x, पुनो *hier, int c, int stride);

#endif
