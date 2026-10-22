#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_normalization_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ alpha, ଦଶମିକ beta, ଦଶମିକ kappa);
ଶୂନ୍ୟ resize_normalization_layer(layer *layer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w);
ଶୂନ୍ୟ forward_normalization_layer(ଅଚଳ layer layer, network net);
ଶୂନ୍ୟ backward_normalization_layer(ଅଚଳ layer layer, network net);
ଶୂନ୍ୟ visualize_normalization_layer(layer layer, ଅକ୍ଷର *window);

#ifdef GPU
ଶୂନ୍ୟ forward_normalization_layer_gpu(ଅଚଳ layer layer, network net);
ଶୂନ୍ୟ backward_normalization_layer_gpu(ଅଚଳ layer layer, network net);
#endif

#endif
