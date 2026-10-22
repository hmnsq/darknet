#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer softmax_layer;

ଶୂନ୍ୟ softmax_array(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ temp, ଦଶମିକ *output);
softmax_layer make_softmax_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups);
ଶୂନ୍ୟ forward_softmax_layer(ଅଚଳ softmax_layer l, network net);
ଶୂନ୍ୟ backward_softmax_layer(ଅଚଳ softmax_layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ pull_softmax_layer_output(ଅଚଳ softmax_layer l);
ଶୂନ୍ୟ forward_softmax_layer_gpu(ଅଚଳ softmax_layer l, network net);
ଶୂନ୍ୟ backward_softmax_layer_gpu(ଅଚଳ softmax_layer l, network net);
#endif

#endif
