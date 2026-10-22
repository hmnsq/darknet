#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer local_layer;

#ifdef GPU
ଶୂନ୍ୟ forward_local_layer_gpu(local_layer layer, network net);
ଶୂନ୍ୟ backward_local_layer_gpu(local_layer layer, network net);
ଶୂନ୍ୟ update_local_layer_gpu(local_layer layer, update_args a);

ଶୂନ୍ୟ ଠେଲ_local_layer(local_layer layer);
ଶୂନ୍ୟ pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad, ACTIVATION activation);

ଶୂନ୍ୟ forward_local_layer(ଅଚଳ local_layer layer, network net);
ଶୂନ୍ୟ backward_local_layer(local_layer layer, network net);
ଶୂନ୍ୟ update_local_layer(local_layer layer, update_args a);

ଶୂନ୍ୟ bias_output(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ backward_bias(ଦଶମିକ *bias_updates, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);

#endif

