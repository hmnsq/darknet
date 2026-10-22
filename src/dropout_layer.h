#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer dropout_layer;

dropout_layer make_dropout_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ଦଶମିକ probability);

ଶୂନ୍ୟ forward_dropout_layer(dropout_layer l, network net);
ଶୂନ୍ୟ backward_dropout_layer(dropout_layer l, network net);
ଶୂନ୍ୟ resize_dropout_layer(dropout_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs);

#ifdef GPU
ଶୂନ୍ୟ forward_dropout_layer_gpu(dropout_layer l, network net);
ଶୂନ୍ୟ backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
