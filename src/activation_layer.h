#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_activation_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ACTIVATION activation);

ଶୂନ୍ୟ forward_activation_layer(layer l, network net);
ଶୂନ୍ୟ backward_activation_layer(layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_activation_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_activation_layer_gpu(layer l, network net);
#endif

#endif

