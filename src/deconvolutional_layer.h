#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

#ifdef GPU
ଶୂନ୍ୟ forward_deconvolutional_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_deconvolutional_layer_gpu(layer l, network net);
ଶୂନ୍ୟ update_deconvolutional_layer_gpu(layer l, update_args a);
ଶୂନ୍ୟ ଠେଲ_deconvolutional_layer(layer l);
ଶୂନ୍ୟ pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam);
ଶୂନ୍ୟ resize_deconvolutional_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w);
ଶୂନ୍ୟ forward_deconvolutional_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ update_deconvolutional_layer(layer l, update_args a);
ଶୂନ୍ୟ backward_deconvolutional_layer(layer l, network net);

#endif

