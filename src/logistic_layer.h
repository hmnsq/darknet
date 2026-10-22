#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_logistic_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs);
ଶୂନ୍ୟ forward_logistic_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_logistic_layer(ଅଚଳ layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_logistic_layer_gpu(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_logistic_layer_gpu(ଅଚଳ layer l, network net);
#endif

#endif
