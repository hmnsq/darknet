#ifndef درەخت_H
#define درەخت_H
#ضمن "darknet.h"

صحيح hierarchy_لووتکە_prediction(عائم *predictions, درەخت *hier, عائم thresh, صحيح stride);
عائم get_hierarchy_probability(عائم *x, درەخت *hier, صحيح c, صحيح stride);

#endif
