#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer convolutional_layer;

#ifdef GPU
ଶୂନ୍ୟ forward_convolutional_layer_gpu(convolutional_layer layer, network net);
ଶୂନ୍ୟ backward_convolutional_layer_gpu(convolutional_layer layer, network net);
ଶୂନ୍ୟ update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

ଶୂନ୍ୟ ଠେଲ_convolutional_layer(convolutional_layer layer);
ଶୂନ୍ୟ pull_convolutional_layer(convolutional_layer layer);

ଶୂନ୍ୟ add_bias_gpu(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ backward_bias_gpu(ଦଶମିକ *bias_updates, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ adam_update_gpu(ଦଶମିକ *w, ଦଶମିକ *d, ଦଶମିକ *m, ଦଶମିକ *v, ଦଶମିକ B1, ଦଶମିକ B2, ଦଶମିକ eps, ଦଶମିକ decay, ଦଶମିକ rate, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା t);
#ifdef CUDNN
ଶୂନ୍ୟ cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା xnor, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam);
ଶୂନ୍ୟ resize_convolutional_layer(convolutional_layer *layer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ଶୂନ୍ୟ forward_convolutional_layer(ଅଚଳ convolutional_layer layer, network net);
ଶୂନ୍ୟ update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, ଅକ୍ଷର *window, image *prev_weights);
ଶୂନ୍ୟ binarize_weights(ଦଶମିକ *weights, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ *ଦ୍ୱିମିକ);
ଶୂନ୍ୟ swap_ଦ୍ୱିମିକ(convolutional_layer *l);
ଶୂନ୍ୟ binarize_weights2(ଦଶମିକ *weights, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଅକ୍ଷର *ଦ୍ୱିମିକ, ଦଶମିକ *scales);

ଶୂନ୍ୟ backward_convolutional_layer(convolutional_layer layer, network net);

ଶୂନ୍ୟ add_bias(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);
ଶୂନ୍ୟ backward_bias(ଦଶମିକ *bias_updates, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା convolutional_out_height(convolutional_layer layer);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା convolutional_out_width(convolutional_layer layer);

#endif

